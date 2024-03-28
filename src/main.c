#include <gtksourceview/gtksource.h>
#include "global.h"
#include "commands.h"
#include "start.h"
#include <locale.h>

int main(int argc, char * argv[]) {

    gtk_init(NULL, NULL);
    gtk_source_init();

    GtkWidget * window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Janus");

    // Create main text buffer
    GtkWidget * text = gtk_source_view_new();
    GtkWidget * view = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(view), text);
    GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));

    // Initiate serach functionality
    GtkSourceSearchContext * context = gtk_source_search_context_new (GTK_SOURCE_BUFFER(buffer), NULL);
    gtk_source_search_context_set_highlight(context, FALSE);
    gtk_source_search_settings_set_wrap_around(gtk_source_search_context_get_settings(context), TRUE);

    struct Document document = {
        .buffer = buffer,
        .view = view,
        .context = context,
        .window = GTK_WINDOW(window),
        .fontsize = 12,
        .wrap = DEFAULT_WRAP,
        .syntax = DEFAULT_SYNTAX
    };

    // Connect singals
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), &document);
    g_signal_connect(buffer, "modified-changed", G_CALLBACK(change_indicator), &document);

    if (argc > 1) 
        open_file(argv[1], &document);

    // Preferences setup
    init_app(GTK_WINDOW(window));
    init_preferences(&document);

    // Menu setup
    GtkAccelGroup * accel = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel);
    GtkWidget * bar = gtk_menu_bar_new();
    init_menu(bar, accel, &document);

    // Boxes
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(box), bar, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(box), view, 1, 1, 0);

    // Pack up app and run
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all (window);

    gtk_main();

    gtk_source_finalize();
    free(document.path);

    return 0;
}
