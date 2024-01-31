#include <gtksourceview/gtksource.h>
#include "global.h"
#include "commands.h"
#include "menu.h"

int main(int argc, char * argv[]) {

    struct Document document;
    document.name[0] = '\0';

    gtk_init(NULL, NULL);

    GtkWidget * window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Notes");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), &document);

    GError *error = NULL;
    GdkPixbuf * icon;
    if (getenv("APPDIR")) {
        char path[PATH_MAX];
        strcpy(path, getenv("APPDIR"));
        strcat(path, "/notes.png");
        icon = gdk_pixbuf_new_from_file(path, &error);
    }
    else 
        icon = gdk_pixbuf_new_from_file("/usr/share/pixmaps/notes.png", &error);

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
    g_signal_connect(buffer, "modified-changed", G_CALLBACK(change_indicator), &document);

    GtkSourceSearchContext * context = gtk_source_search_context_new (GTK_SOURCE_BUFFER(buffer), NULL);
    gtk_source_search_context_set_highlight(context, FALSE);
    gtk_source_search_settings_set_wrap_around(gtk_source_search_context_get_settings(context), TRUE);

    document.buffer = buffer;
    document.view = text;
    document.context = context;
    document.window = GTK_WINDOW(window);

    // Menu setup
    GtkAccelGroup * accel = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel);

    GtkWidget * bar = gtk_menu_bar_new();
    init_menu(bar, accel, &document);

    // Boxes
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Add two parts to box
    gtk_box_pack_start(GTK_BOX(box), bar, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(box), view, 1, 1, 0);

    // Pack up app and run
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all (window);

    if (argc > 1) {
        if (strlen(argv[1]) + strlen(document.name) < 256) {
            strcat(document.name, argv[1]);
            if (access(document.name, F_OK) == 0) {
                open_file(document.name, &document);
            }
        }
    }

    
    gtk_main();

    return 0;
}
