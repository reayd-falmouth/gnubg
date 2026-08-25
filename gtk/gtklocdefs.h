/*
 * Copyright (C) 2011-2014 Michael Petch <mpetch@capp-sysware.com>
 * Copyright (C) 2011-2026 the AUTHORS
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

#ifndef GTKLOCDEFS_H
#define GTKLOCDEFS_H

#include "config.h"

#if defined(USE_GTK)
#include <gtk/gtk.h>

#if GTK_CHECK_VERSION(3,0,0)
typedef cairo_region_t gtk_locdef_region;
typedef cairo_rectangle_int_t gtk_locdef_rectangle;
typedef cairo_surface_t gtk_locdef_surface;
typedef const GdkRectangle gtk_locdef_cell_area;
#define gtk_locdef_surface_create(widget, width, height) cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height)
#define gtk_locdef_image_new_from_surface(s) gtk_image_new_from_surface(s)
#define gtk_locdef_cairo_create_from_surface(s) cairo_create(s)
#define gtk_locdef_create_rectangle(r) cairo_region_create_rectangle(r)
#define gtk_locdef_union_rectangle(pr, r) cairo_region_union_rectangle(pr, r)
#define gtk_locdef_region_destroy(pr) cairo_region_destroy(pr)
#define gtk_locdef_paint_box(style, window, cr, state_type, shadow_type, area, widget, detail, x, y, width, height) \
    gtk_paint_box(style, cr, state_type, shadow_type, widget, detail, x, y, width, height)
#define gdk_colormap_alloc_color(cm, c, w, bm)
#define gtk_statusbar_set_has_resize_grip(pw, grip)

#else // GTK2
typedef GdkRegion gtk_locdef_region;
typedef GdkRectangle gtk_locdef_rectangle;
typedef GdkPixmap gtk_locdef_surface;
typedef GdkRectangle gtk_locdef_cell_area;
typedef void *GtkCssProvider;
#define gtk_locdef_surface_create(widget, width, height) gdk_pixmap_new(NULL, width, height, gdk_visual_get_depth(gtk_widget_get_visual(widget)))
#define gtk_locdef_image_new_from_surface(s) gtk_image_new_from_pixmap(s, NULL)
#define gtk_locdef_cairo_create_from_surface(s) gdk_cairo_create(s)
#define gtk_locdef_create_rectangle(r) gdk_region_rectangle(r)
#define gtk_locdef_union_rectangle(pr, r) gdk_region_union_with_rect(pr, r)
#define gtk_locdef_region_destroy(pr) gdk_region_destroy(pr)
#define gtk_locdef_paint_box(style, window, cr, state_type, shadow_type, area, widget, detail, x, y, width, height) \
    gtk_paint_box(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height)
#endif

extern GtkWidget *get_statusbar_label(GtkStatusbar * statusbar);

#ifndef USE_GRESOURCE
#define gnubg_stock_register_resource()
extern GdkPixbuf *gdk_pixbuf_new_from_resource(const char *resource_path, GError **error);
#endif

#endif    /* USE_GTK */

#endif
