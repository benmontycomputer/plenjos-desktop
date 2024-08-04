/* desktop-window.c
 *
 * Copyright 2023 Benjamin Montgomery
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "desktop-config.h"
#include "desktop-window.h"

struct _DesktopWindow
{
  GtkWindow  parent_instance;

  /* Template widgets */
  //GtkLabel            *label;
  GtkImage *image;
};

G_DEFINE_TYPE (DesktopWindow, desktop_window, GTK_TYPE_WINDOW)

static void
desktop_window_class_init (DesktopWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/com/plenjos/Desktop/desktop-window.ui");
  gtk_widget_class_bind_template_child (widget_class, DesktopWindow, image);
}

static void
desktop_window_init (DesktopWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  gtk_widget_show (&self->parent_instance);

  GdkRectangle monitor_geo;

  gdk_monitor_get_geometry (gdk_display_get_monitor_at_window (gdk_display_get_default (), gtk_widget_get_window (GTK_WIDGET (&self->parent_instance))), &monitor_geo);

  gtk_window_resize (&self->parent_instance, monitor_geo.width, monitor_geo.height);
  gtk_window_move (&self->parent_instance, 0, 0);

  // https://askubuntu.com/questions/66914/how-to-change-desktop-background-from-command-line-in-unity
  GSettings *s = g_settings_new ("org.gnome.desktop.interface");
  GSettings *s_bg = g_settings_new ("org.gnome.desktop.background");

  gchar *bg = NULL;
  if (!strcmp (g_settings_get_string (s, "color-scheme"), "prefer-dark")) {
    bg = g_settings_get_string (s_bg, "picture-uri-dark");
  } else {
    bg = g_settings_get_string (s_bg, "picture-uri");
  }

  if (strlen (bg) > 7) {
    printf ("%s\n", bg + 7);
    fflush (stdout);
    GdkPixbuf *pbuf = gdk_pixbuf_new_from_file (bg + 7, NULL);
    gint pbufw, pbufh;
    pbufw = gdk_pixbuf_get_width (pbuf);
    pbufh = gdk_pixbuf_get_height (pbuf);
    printf ("%d %d %d\n", pbufw, pbufh, monitor_geo.width);
    fflush (stdout);

    GdkPixbuf *pbuf2 = gdk_pixbuf_new (GDK_COLORSPACE_RGB, TRUE, gdk_pixbuf_get_bits_per_sample (pbuf), monitor_geo.width, monitor_geo.height);

    gdk_pixbuf_scale (pbuf, pbuf2, 0, 0,
                             monitor_geo.width,
                             monitor_geo.height,
                             0,
                             (monitor_geo.height - ((double)pbufh * (double)monitor_geo.width / (double)pbufw)) / 2,
                             (double)monitor_geo.width / (double)pbufw,
                             (double)monitor_geo.width / (double)pbufw,
                             GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf (self->image, pbuf2);
    //gtk_image_set_from_pixbuf (self->image, gdk_pixbuf_scale_simple (pbuf, monitor_geo.width, ((double)monitor_geo.width / (double)gdk_pixbuf_get_width (pbuf)) * (double)gdk_pixbuf_get_height (pbuf), GDK_INTERP_BILINEAR));
  } else {
    fprintf (stderr, "Error: couldn't determine background.\n");
    fflush (stderr);
  }
}
