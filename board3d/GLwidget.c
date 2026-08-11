/*
 * Copyright (C) 2019-2021 Jon Kinsey <jonkinsey@gmail.com>
 * Copyright (C) 2026 the AUTHORS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gtk/gtk.h>

#if GTK_CHECK_VERSION(3,0,0)
#include <epoxy/gl.h>
#endif

#include "fun3d.h"
#include "util.h"
#include "common.h"

#include <cglm/affine.h>

typedef struct {
    RealizeCB realizeCB;
    ConfigureCB configureCB;
    ExposeCB exposeCB;
    void* cbData;
} GLWidgetData;

#if GTK_CHECK_VERSION(3,0,0)

typedef struct {
    guint shader;
    guint projection_location, modelView_location, textureMat_location;
} ShaderDetails;

static ShaderDetails mainShader, basicShader, *currentShader;

static guint pick_colour_location;
static gint materialDiffuse_location;

static guint light_ambient_location, light_diffuse_location, light_specModel_location, light_specular_location, light_shininess_location;
static guint light_dirLight_location, light_lightDirection_location, light_lightPos_location, light_viewPos_location;

void
setMaterial(const Material* pMat)
{
    if (pMat == NULL || pMat == currentMat)
        return;

    currentMat = pMat;

    if (currentShader != &mainShader)
        return;

    glUniform3fv(light_ambient_location, 1, pMat->ambientColour);
    glUniform3fv(light_diffuse_location, 1, pMat->diffuseColour);
    glUniform3fv(light_specular_location, 1, pMat->specularColour);
    glUniform1f(light_shininess_location, (float)pMat->shine);
    glUniform1i(light_specModel_location, 1);
    glUniform1i(light_dirLight_location, 0);

    if (pMat->pTexture) {
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(materialDiffuse_location, 0);
        glBindTexture(GL_TEXTURE_2D, pMat->pTexture->texID);
    }
}

void SetPickColour(float colour)
{
    if (currentShader != &basicShader)
        return;

    float value[3] = { 0 };
    value[0] = colour;
    glUniform3fv(pick_colour_location, 1, value);
}

static void GetViewPos(float* viewPos)
{
    mat4 mvMat, mvInv;
    vec3 origin;
    GetModelViewMatrixMat(mvMat);
    glm_mat4_inv(mvMat, mvInv);
    glm_vec3_zero(origin);
    glm_mat4_mulv3(mvInv, origin, 1, viewPos);
}

void SetViewPos(void)
{
    if (currentShader != &mainShader)
        return;

    float viewPos[3];
    GetViewPos(viewPos);
    glUniform3fv(light_viewPos_location, 1, viewPos);
}

void SetLightPos(float* lp)
{
    if (currentShader != &mainShader)
        return;

    glUniform3fv(light_lightPos_location, 1, lp);
}

void
setMaterialReset(const Material* pMat)
{
    currentMat = NULL;
    setMaterial(pMat);
}

void ModelManagerCopyModelToBuffer(ModelManager* modelHolder, int modelNumber)
{
    if (modelHolder->models[modelNumber].data == NULL)
        return;
    glBindBuffer(GL_ARRAY_BUFFER, modelHolder->buffer);	/* this is the VBO that holds the vertex data */
    glBufferSubData(
        GL_ARRAY_BUFFER,
        modelHolder->models[modelNumber].dataStart * sizeof(float),
        modelHolder->models[modelNumber].dataLength * sizeof(float),
        modelHolder->models[modelNumber].data);
#if !GTK_CHECK_VERSION(3,0,0)
    g_free(modelHolder->models[modelNumber].data);
    modelHolder->models[modelNumber].data = NULL;
#endif
}

void ModelManagerCreate(ModelManager* modelHolder)
{
    int model;
    gboolean need_upload = FALSE;

    if (modelHolder->vao == 0 || !glIsVertexArray(modelHolder->vao))
    {
        if (modelHolder->buffer && glIsBuffer(modelHolder->buffer))
            glDeleteBuffers(1, &modelHolder->buffer);
        if (modelHolder->vao && glIsVertexArray(modelHolder->vao))
            glDeleteVertexArrays(1, &modelHolder->vao);

        modelHolder->vao = 0;
        modelHolder->buffer = 0;
        modelHolder->allocNumVertices = 0;

        /* we need to create a VAO to store the other buffers */
        glGenVertexArrays(1, &modelHolder->vao);

        glBindVertexArray(modelHolder->vao);

        /* this is the VBO that holds the vertex data */
        glGenBuffers(1, &modelHolder->buffer);
        glBindBuffer(GL_ARRAY_BUFFER, modelHolder->buffer);

        /* get the location of the "position" and "color" attributes */
        GLint position_index = glGetAttribLocation(mainShader.shader, "positionAttrib");
        GLint texCoord_index = glGetAttribLocation(mainShader.shader, "texCoordAttrib");
        GLint normal_index = glGetAttribLocation(mainShader.shader, "normalAttrib");

        if (position_index < 0 || texCoord_index < 0 || normal_index < 0) {
            g_warning("Missing shader attribute: position=%d texCoord=%d normal=%d",
                      position_index, texCoord_index, normal_index);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            return;
        }

        int stride = VERTEX_STRIDE * sizeof(float);
        /* enable and set the attributes */
        glEnableVertexAttribArray(position_index);

        glVertexAttribPointer(position_index, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(5 * sizeof(float)));
        glEnableVertexAttribArray(texCoord_index);
        glVertexAttribPointer(texCoord_index, 2, GL_FLOAT, GL_FALSE, stride, 0);
        glEnableVertexAttribArray(normal_index);
        glVertexAttribPointer(normal_index, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(2 * sizeof(float)));

        need_upload = TRUE;
    }
    if (modelHolder->totalNumVertices > modelHolder->allocNumVertices)
    {
        glBindVertexArray(modelHolder->vao);
        glBindBuffer(GL_ARRAY_BUFFER, modelHolder->buffer);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * modelHolder->totalNumVertices, NULL, GL_STATIC_DRAW);
        modelHolder->allocNumVertices = modelHolder->totalNumVertices;

        need_upload = TRUE;
    }

    if (!need_upload)
        return;

    /* Copy data into gpu buffer */
    int vertexPos = 0;
    for (model = 0; model < modelHolder->numModels; model++)
    {
        modelHolder->models[model].dataStart = vertexPos;
        ModelManagerCopyModelToBuffer(modelHolder, model);
        vertexPos += modelHolder->models[model].dataLength;
    }
    g_assert(vertexPos == modelHolder->totalNumVertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GLWidgetMakeCurrent(GtkWidget* widget)
{
    gtk_gl_area_make_current(GTK_GL_AREA(widget));
}

static gboolean SelectProgram(ShaderDetails* pShader)
{
    if (pShader == NULL || pShader->shader == 0) {
        currentShader = NULL;
        return FALSE;
    }

    currentShader = pShader;
    glUseProgram(currentShader->shader);
    currentMat = NULL;
    return TRUE;
}

void SelectPickProgram(void)
{
    SelectProgram(&basicShader);
}

static const char* LoadFile(const char* filename)
{
    gchar *contents = NULL;
    gsize length;

    if (!g_file_get_contents(filename, &contents, &length, NULL)) {
        g_print(_("Failed to open %s!\n"), filename);
        return NULL;
    }

    return contents;
}

static guint CreateShader(int shader_type, const char* shader_name)
{
    int status;
    const char* source;
    char* pathname = BuildFilename(shader_name);
    char* filename;

    if (shader_type == GL_VERTEX_SHADER)
        filename = g_strdup_printf("%s-vertex.glsl", pathname);
    else
        filename = g_strdup_printf("%s-fragment.glsl", pathname);
    g_free(pathname);

    source = LoadFile(filename);
    if (source == NULL) {
        g_free(filename);
        return 0;
    }
    g_free(filename);

    guint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        int log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

        char* buffer = g_malloc(log_len + 1);
        glGetShaderInfoLog(shader, log_len, NULL, buffer);
        printf(_("Compilation failure in %s shader: %s"), shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment", buffer);
        g_free((gpointer)source);
        glDeleteShader(shader);

        g_free(buffer);
        return 0;
    }

    g_free((gpointer)source);
    return shader;
}

static guint
init_shaders(const char* shader_name)
{
    guint vertex = CreateShader(GL_VERTEX_SHADER, shader_name);
    guint fragment = CreateShader(GL_FRAGMENT_SHADER, shader_name);

    if (!vertex || !fragment) {
        if (vertex)
            glDeleteShader(vertex);
        if (fragment)
            glDeleteShader(fragment);
        return 0;
    }

    /* link the vertex and fragment shaders together */
    guint program = glCreateProgram();

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    int status = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        int log_len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);

        char* buffer = g_malloc(log_len + 1);
        glGetProgramInfoLog(program, log_len, NULL, buffer);

        printf(_("Linking failure in program: %s"), buffer);

        g_free(buffer);

        glDetachShader(program, vertex);
        glDetachShader(program, fragment);
        glDeleteProgram(program);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return FALSE;
    }

    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

static guint
LookupUniform(ShaderDetails *pShader, const char* name)
{
    GLint loc = glGetUniformLocation(pShader->shader, name);
    if (loc == -1)
    {
        printf("Didn't find loc %s\n", name);
    }
    return loc;
}

static gboolean
InitShaderPrograms(GtkWidget* widget)
{
    if (glIsProgram(basicShader.shader))
        glDeleteProgram(basicShader.shader);
    if (glIsProgram(mainShader.shader))
        glDeleteProgram(mainShader.shader);

    basicShader.shader = 0;
    mainShader.shader = 0;
    currentShader = NULL;
    currentMat = NULL;

    basicShader.shader = init_shaders("/Shaders/basic");
    mainShader.shader = init_shaders("/Shaders/main");

    if (!basicShader.shader || !mainShader.shader) {
        gtk_gl_area_set_error(
            GTK_GL_AREA(widget),
            g_error_new_literal(
                g_quark_from_static_string("gnubg-gl"),
                1,
                "Failed to initialize OpenGL shaders"));
        return FALSE;
    }

    basicShader.projection_location = LookupUniform(&basicShader, "projection");
    basicShader.modelView_location = LookupUniform(&basicShader, "modelView");
    basicShader.textureMat_location = GL_INVALID_VALUE;
    pick_colour_location = LookupUniform(&basicShader, "colour");

    mainShader.projection_location = LookupUniform(&mainShader, "projection");
    mainShader.modelView_location = LookupUniform(&mainShader, "modelView");
    mainShader.textureMat_location = LookupUniform(&mainShader, "textureMat");

    materialDiffuse_location = LookupUniform(&mainShader, "materialDiffuse");
    light_ambient_location = LookupUniform(&mainShader, "light.ambient");
    light_diffuse_location = LookupUniform(&mainShader, "light.diffuse");
    light_specular_location = LookupUniform(&mainShader, "light.specular");
    light_shininess_location = LookupUniform(&mainShader, "light.shininess");
    light_specModel_location = LookupUniform(&mainShader, "light.specModel");
    light_dirLight_location = LookupUniform(&mainShader, "light.dirLight");
    light_lightDirection_location = LookupUniform(&mainShader, "light.lightDirection");
    light_lightPos_location = LookupUniform(&mainShader, "light.lightPos");
    light_viewPos_location = LookupUniform(&mainShader, "light.viewPos");

    currentShader = NULL;
    currentMat = NULL;

    return TRUE;
}

static void
realize_event(GtkWidget* widget, const GLWidgetData* glwData)
{
    GLWidgetMakeCurrent(widget);

    if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL)
        return;


    if (!InitShaderPrograms(widget))
        return;

    SelectProgram(&mainShader);

    glwData->realizeCB(glwData->cbData);

    gtk_widget_queue_draw(widget);
}

static void
unrealize_event(GtkWidget* self, gpointer UNUSED(user_data))
{
    GLWidgetMakeCurrent(self);

    /*
     * Do not delete global shader programs here.
     * Other GtkGLArea instances may still be using them.
     *
     * TODO: Make shader/render state per GL widget context,
     * instead of global.
     */

    currentShader = NULL;
    currentMat = NULL;
}

static void
resize_event(GtkGLArea* widget, gint UNUSED(width), gint UNUSED(height), const GLWidgetData* glwData)
{
    GLWidgetMakeCurrent(GTK_WIDGET(widget));
    glwData->configureCB(GTK_WIDGET(widget), glwData->cbData);
}

void SetLineDrawingmode(int enable)
{
    (void)enable;
}

void OglModelDraw(const ModelManager* modelManager, int modelNumber, const Material* pMat)
{
    if (currentShader == NULL || currentShader->shader == 0)
        return;

    if (modelManager == NULL ||
        modelNumber < 0 ||
        modelNumber >= modelManager->numModels ||
        modelManager->vao == 0)
        return;

    if (modelManager->vao == 0 || !glIsVertexArray(modelManager->vao)) {
        g_warning("Invalid VAO %u in current context", modelManager->vao);
        return;
    }

    setMaterial(pMat);

    /* update the projection matrices we use in the shader */
    glUniformMatrix4fv(currentShader->projection_location, 1, GL_FALSE, GetProjectionMatrix());

    glUniformMatrix4fv(currentShader->modelView_location, 1, GL_FALSE, GetModelViewMatrix());

    if (currentShader->textureMat_location != GL_INVALID_VALUE)
    {
        mat3 textureMat;
        GetTextureMatrix(&textureMat);
        glUniformMatrix3fv(currentShader->textureMat_location, 1, GL_FALSE, (float*)textureMat);
    }

    /* use the buffers in the VAO */
    glBindVertexArray(modelManager->vao);

    /* draw the vertices in the model */
    glDrawArrays(
        GL_TRIANGLES,
        modelManager->models[modelNumber].dataStart / VERTEX_STRIDE,
        modelManager->models[modelNumber].dataLength / VERTEX_STRIDE);
}

gboolean GLWidgetRender(GtkWidget* widget, ExposeCB exposeCB, GdkEventExpose* eventDetails, void* data)
{
    if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL)
        return FALSE;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (mainShader.shader == 0) {
        return FALSE;
    }

    if (!SelectProgram(&mainShader))
        return FALSE;

    exposeCB(widget, eventDetails, data);

    return TRUE;
}

static gboolean
expose_event(GtkWidget* widget, GdkEventExpose* eventDetails, const GLWidgetData* glwData)
{
    return GLWidgetRender(widget, glwData->exposeCB, eventDetails, glwData->cbData);
}

GtkWidget* GLWidgetCreate(RealizeCB realizeCB, ConfigureCB configureCB, ExposeCB exposeCB, void* data)
{
    GtkWidget* pw = gtk_gl_area_new();

    if (pw == NULL) {
        g_print(_("Can't create OpenGL drawing widget\n"));
        return NULL;
    }

    gtk_gl_area_set_has_depth_buffer(GTK_GL_AREA(pw), TRUE);
    gtk_gl_area_set_has_stencil_buffer(GTK_GL_AREA(pw), TRUE);
    gtk_gl_area_set_auto_render(GTK_GL_AREA(pw), TRUE);

    GLWidgetData* glwData = g_malloc(sizeof(GLWidgetData));

    glwData->cbData = data;
    glwData->realizeCB = realizeCB;
    glwData->configureCB = configureCB;
    glwData->exposeCB = exposeCB;

    if (realizeCB != NULL) {
        g_signal_connect(G_OBJECT(pw), "realize", G_CALLBACK(realize_event), glwData);
        g_signal_connect(G_OBJECT(pw), "unrealize", G_CALLBACK(unrealize_event), NULL);
    }
    if (configureCB != NULL)
        g_signal_connect(G_OBJECT(pw), "resize", G_CALLBACK(resize_event), glwData);
    if (exposeCB != NULL)
        g_signal_connect(G_OBJECT(pw), "render", G_CALLBACK(expose_event), glwData);

    g_object_set_data_full(G_OBJECT(pw), "GLWidgetData", glwData, g_free);

    return pw;
}

gboolean GLInit(int* UNUSED(argc), char*** UNUSED(argv))
{
    return GL_TRUE;	// TODO: Anything to check here?
}

#else

#include <gtk/gtkgl.h>

void GLWidgetMakeCurrent(GtkWidget* widget)
{
    if (!gdk_gl_drawable_make_current(gtk_widget_get_gl_drawable(widget), gtk_widget_get_gl_context(widget)))
        g_print("gdk_gl_drawable_make_current failed!\n");
}

static GdkGLConfig*
getGlConfig(void)
{
    static GdkGLConfig* glconfig = NULL;
    if (!glconfig)
        glconfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE | GDK_GL_MODE_STENCIL);
    if (!glconfig) {
        glconfig = gdk_gl_config_new_by_mode(GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_DOUBLE);
        g_warning(_("Stencil buffer not available, no shadows\n"));
    }
    if (!glconfig) {
        g_warning(_("No appropriate OpenGL-capable visual found\n"));
    }
    return glconfig;
}

static void
realize_event(GtkWidget* widget, const GLWidgetData* glwData)
{
    GdkGLDrawable* gldrawable = gtk_widget_get_gl_drawable(widget);

    if (!gdk_gl_drawable_gl_begin(gldrawable, gtk_widget_get_gl_context(widget)))
        return;

    glwData->realizeCB(glwData->cbData);

    gdk_gl_drawable_gl_end(gldrawable);
}

static gboolean
configure_event(GtkWidget* widget, GdkEventConfigure* UNUSED(eventDetails), void* data)
{
    const GLWidgetData* glwData = (const GLWidgetData*)data;
    GdkGLDrawable* gldrawable = gtk_widget_get_gl_drawable(widget);

    if (!gdk_gl_drawable_gl_begin(gldrawable, gtk_widget_get_gl_context(widget)))
        return FALSE;

    glwData->configureCB(widget, glwData->cbData);

    gdk_gl_drawable_gl_end(gldrawable);

    return TRUE;
}

gboolean GLWidgetRender(GtkWidget* widget, ExposeCB exposeCB, GdkEventExpose* eventDetails, void* data)
{
    GdkGLDrawable* gldrawable = gtk_widget_get_gl_drawable(widget);

    if (!gdk_gl_drawable_gl_begin(gldrawable, gtk_widget_get_gl_context(widget)))
        return TRUE;
    CheckOpenglError();

    if (exposeCB(widget, eventDetails, data))
        gdk_gl_drawable_swap_buffers(gldrawable);

    gdk_gl_drawable_gl_end(gldrawable);

    return TRUE;
}

static gboolean
expose_event(GtkWidget* widget, GdkEventExpose* eventDetails, const GLWidgetData* glwData)
{
    return GLWidgetRender(widget, glwData->exposeCB, eventDetails, glwData->cbData);
}

GtkWidget*
GLWidgetCreate(RealizeCB realizeCB, ConfigureCB configureCB, ExposeCB exposeCB, void *data)
{
    GtkWidget* pw = gtk_drawing_area_new();
    GLWidgetData* glwData;

    if (pw == NULL) {
        g_print(_("Can't create OpenGL drawing widget\n"));
        return NULL;
    }

    /* Set OpenGL-capability to the widget - no list sharing */
    if (!gtk_widget_set_gl_capability(pw, getGlConfig(), NULL, TRUE, GDK_GL_RGBA_TYPE))
    {
        g_print(_("Can't create OpenGL capable widget\n"));
        return NULL;
    }

    glwData = g_malloc(sizeof(GLWidgetData));
    glwData->cbData = data;
    glwData->realizeCB = realizeCB;
    glwData->configureCB = configureCB;
    glwData->exposeCB = exposeCB;

    if (realizeCB != NULL)
        g_signal_connect(G_OBJECT(pw), "realize", G_CALLBACK(realize_event), glwData);
    if (configureCB != NULL)
        g_signal_connect(G_OBJECT(pw), "configure_event", G_CALLBACK(configure_event), glwData);
    if (exposeCB != NULL)
        g_signal_connect(G_OBJECT(pw), "expose_event", G_CALLBACK(expose_event), glwData);

    g_object_set_data_full(G_OBJECT(pw), "GLWidgetData", glwData, g_free);

    return pw;
}

gboolean GLInit(int* argc, char*** argv)
{
    return gtk_gl_init_check(argc, argv);
}

#endif
