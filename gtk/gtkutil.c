/*
 * Copyright (C) 2026 Nikolas Nyby <nikolas@gnu.org>
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

#include "gtkutil.h"

/*
 * GTK4-compatible wrapper for gtk_box_pack_start()
 */
inline void
box_append_compat(
    GtkBox *box, GtkWidget *child, gboolean expand, gboolean fill,
    guint padding)
{
#if GTK_CHECK_VERSION(4, 0, 0)
    GtkOrientation o = gtk_orientable_get_orientation(GTK_ORIENTABLE(box));

    if (expand) {
        if (o == GTK_ORIENTATION_HORIZONTAL)
            gtk_widget_set_hexpand(child, TRUE);
        else
            gtk_widget_set_vexpand(child, TRUE);
    }

    if (padding) {
        gtk_widget_set_margin_start(child, padding);
        gtk_widget_set_margin_end(child, padding);
        gtk_widget_set_margin_top(child, padding);
        gtk_widget_set_margin_bottom(child, padding);
    }

    gtk_box_append(box, child);
#else
    gtk_box_pack_start(box, child, expand, fill, padding);
#endif
}

/*
 * GTK4-compatible wrapper for gtk_box_pack_start()
 */
inline void
editable_set_text_compat(GtkEntry* entry, const gchar* text)
{
#if GTK_CHECK_VERSION(4, 0, 0)
    gtk_editable_set_text(GTK_EDITABLE(entry), text)
#else
    gtk_entry_set_text(GTK_ENTRY(entry), text);
#endif
}

inline void
widget_set_visible_compat(GtkWidget *widget)
{
#if GTK_CHECK_VERSION(4, 0, 0)
    gtk_widget_set_visible(widget, TRUE);
#else
    gtk_widget_show_all(widget);
#endif
}
