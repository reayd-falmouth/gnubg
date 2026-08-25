/*
 * Copyright (C) 2000 Jonathan Blandford
 * Copyright (C) 2006-2026 the AUTHORS
 *
 * License changed from the GNU LGPL to the GNU GPL (as permitted under
 * Term 3 of the GNU LGPL) by Gary Wong for distribution with GNU Backgammon.
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

#ifndef GTK_MULTIVIEW_H
#define GTK_MULTIVIEW_H

#include <gtk/gtk.h>

#if GTK_CHECK_VERSION(4, 0, 0)
#define GTK_MULTIVIEW_PARENT_TYPE  GTK_TYPE_WIDGET
#define GtkMultiviewParent         GtkWidget
#define GtkMultiviewParentClass    GtkWidgetClass
#else
#define GTK_MULTIVIEW_PARENT_TYPE  GTK_TYPE_CONTAINER
#define GtkMultiviewParent         GtkContainer
#define GtkMultiviewParentClass    GtkContainerClass
#endif

#define GTK_TYPE_MULTIVIEW			(gtk_multiview_get_type ())
#define GTK_MULTIVIEW(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_MULTIVIEW, GtkMultiview))
#define GTK_MULTIVIEW_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_MULTIVIEW, GtkMultiviewClass))
#define GTK_IS_MULTIVIEW(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_MULTIVIEW))
#define GTK_IS_MULTIVIEW_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE ((obj), GTK_TYPE_MULTIVIEW))
#define GTK_MULTIVIEW_GET_CLASS(obj)  		(G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_MULTIVIEW, GtkMultiviewClass))

typedef struct {
    GtkMultiviewParent parent;

    /*< private > */
    GtkWidget *current;
    GList *children;
} GtkMultiview;

typedef struct {
    GtkMultiviewParentClass parent_class;
} GtkMultiviewClass;

GType gtk_multiview_get_type(void);
GtkWidget *gtk_multiview_new(void);
void gtk_multiview_set_current(GtkMultiview * multiview, GtkWidget * child);

#endif  /* GTK_MULTIVIEW_H */
