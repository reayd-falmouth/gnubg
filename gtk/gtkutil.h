/*
 * Copyright (C) 2026 Nikolas Nyby <nikolas@gnu.org>
 * Copyright (C) 2026 the AUTHORS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef GTKUTIL_H
#define GTKUTIL_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

void box_append_compat(
    GtkBox *box, GtkWidget *child, gboolean expand,
    gboolean fill, guint padding);

void editable_set_text_compat(GtkEntry* entry, const gchar* text);
void widget_set_visible_compat(GtkWidget *widget);

G_END_DECLS

#endif /* GTKUTIL_H */
