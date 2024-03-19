#include <gtksourceview/gtksource.h>
#include "global.h"
#include "commands.h"
#include "init.h"
#include <locale.h>

int main(int argc, char * argv[]) {

    struct Document document;

    gtk_init(NULL, NULL);
    gtk_source_init();

    GtkWidget * window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Janus");
    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), &document);

    char path[PATH_MAX] = "";
    if (getenv("APPDIR") && strlen(getenv("APPDIR")) < PATH_MAX - strlen("/usr/share/locale/")) {
        strcpy(path, getenv("APPDIR"));
        strcat(path, "/usr/share/icons");
        gtk_icon_theme_append_search_path(gtk_icon_theme_get_default(), path);
        strcpy(path, getenv("APPDIR"));
    }
    strcat(path, "/usr/share/locale/");

    setlocale(LC_ALL, "");
    bindtextdomain("janus", path);
    bind_textdomain_codeset("janus", "utf-8");
    textdomain("janus");

    gtk_window_set_icon_name(GTK_WINDOW(window), "janus");

    // Text part
    GtkWidget * text = gtk_source_view_new();

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
    document.path = NULL;
    document.fontsize = 12;
    document.font = NULL;
    document.wrap = DEFAULT_WRAP;
    document.syntax = DEFAULT_SYNTAX;

    if (argc > 1) 
        open_file(argv[1], &document);

    // Preferences setup
    init_preferences(&document);

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

    gtk_main();

    gtk_source_finalize();
    free(document.path);

    return 0;
}
