#include <gtksourceview/gtksource.h>
#include "global.h"
#include "menu.h"

void activate(GtkApplication * app, struct Document * document) {

    GtkWidget * window = gtk_application_window_new (app);
    gtk_window_set_title(GTK_WINDOW(window), "Notes");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    GError *error = NULL;
    GdkPixbuf * icon = gdk_pixbuf_new_from_file("/usr/share/pixmaps/Notes.png", &error);
    if (error != NULL) {
        printf(error->message);
        g_clear_error (&error);
    }
    gtk_window_set_icon(GTK_WINDOW(window), icon);
    
    // Text part
    GtkWidget * text = gtk_source_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text), GTK_WRAP_WORD);
    
    GtkWidget * view = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(view), text);
    GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));

    GtkSourceSearchContext * context = gtk_source_search_context_new (GTK_SOURCE_BUFFER(buffer), NULL);
    gtk_source_search_context_set_highlight(context, FALSE);
    gtk_source_search_settings_set_wrap_around(gtk_source_search_context_get_settings(context), TRUE);

    document->buffer = buffer;
    document->view = view;
    document->context = context;
    document->window = GTK_WINDOW(window);

    // Menu setup
    GtkAccelGroup * accel = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel);

    GtkWidget * bar = gtk_menu_bar_new();
    init_menu(bar, accel, document);

    // Boxes
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Add two parts to box
    gtk_box_pack_start(GTK_BOX(box), bar, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(box), view, 1, 1, 0);

    // Pack up app and run
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all (window);

}

int main(int argc, char * argv[]) {

    GtkApplication * notes = gtk_application_new (NULL, G_APPLICATION_NON_UNIQUE);

    struct Document doc;
    doc.name[0] = '\0';

    g_signal_connect (notes, "activate", G_CALLBACK (activate), &doc);
    int status = g_application_run (G_APPLICATION (notes), argc, argv);
    g_object_unref (notes);

    return status;
}
