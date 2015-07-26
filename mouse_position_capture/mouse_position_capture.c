#include <cairo.h>
#include <gtk/gtk.h>

gdouble coordx[100];
gdouble coordy[100];

static gboolean
on_expose_event(GtkWidget *widget,
                GdkEventExpose *event,
                gpointer data)
{
	cairo_t *cr;
	cr = gdk_cairo_create(gtk_widget_get_window(widget));
	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_paint (cr);
	cairo_stroke(cr);
	cairo_destroy(cr);
	return TRUE;
}

/* Redraw the screen from the surface */
static gboolean
draw_callback (GtkWidget *widget,
               cairo_t   *cr,
               gpointer   data)
{
	GtkAllocation allocation;
	gtk_widget_get_allocation (widget, &allocation);
    cr = gdk_cairo_create(gtk_widget_get_window(widget));
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_paint (cr);
    cairo_stroke(cr);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width (cr, 1.5);
    cairo_move_to(cr, coordx[0], 0);
    cairo_line_to(cr, coordx[0], allocation.height);
    cairo_move_to(cr, 0, coordy[0]);
    cairo_line_to(cr, allocation.width, coordy[0]);

    cairo_stroke(cr);
    cairo_destroy(cr);
  	return TRUE;
}

static gint motion_notify_event(GtkWidget * widget, GdkEventButton * event,gpointer data)         // 鼠标移动事件
{
	gchar a[255];
	sprintf(a, "Point(%i,%i)", (gint)event->x,(gint)event->y);

    if (event->button == 1)
    {
        coordx[0] = event->x;
        coordy[0] = event->y;
    	gtk_widget_queue_draw(widget);
    }

	gtk_window_set_title(widget,a);
	return TRUE;
}

gint
main (int argc, char *argv[])
{
        GtkWidget *window;
        gtk_init(&argc, &argv);

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_title(GTK_WINDOW(window), "lines");
        gtk_window_set_default_size(GTK_WINDOW(window), 700, 400);
        gtk_widget_queue_resize(window);
        //gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
        //gtk_widget_set_app_paintable(window, TRUE);
	    gtk_widget_set_events(window, GDK_EXPOSURE_MASK
		                            | GDK_LEAVE_NOTIFY_MASK
									| GDK_BUTTON_PRESS_MASK
									| GDK_BUTTON_RELEASE_MASK
									| GDK_POINTER_MOTION_MASK
									| GDK_POINTER_MOTION_HINT_MASK);

        g_signal_connect(G_OBJECT (window), "configure-event", G_CALLBACK(on_expose_event), NULL);
        g_signal_connect(G_OBJECT (window), "draw", G_CALLBACK(draw_callback), NULL);
        g_signal_connect(G_OBJECT (window), "destroy",G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(G_OBJECT (window), "motion_notify_event", G_CALLBACK(motion_notify_event),NULL);
        gtk_widget_show_all(window);

        gtk_main();

        return 0;
}
