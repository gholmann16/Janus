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

    char * isuffix = "/usr/share/pixmaps/janus.png";
    char * lsuffix = "/usr/share/locale/";
    char icon_path[512];
    char locale_path[512];

    if (getenv("APPDIR")) {
        icon_path[0] = 0;
        locale_path[0] = 0;
        if (strlen(getenv("APPDIR")) < sizeof(icon_path) - strlen(isuffix)) {
            strcat(icon_path, getenv("APPDIR"));
            strcat(icon_path, isuffix);
        }
        if (strlen(getenv("APPDIR")) < sizeof(locale_path) - strlen(lsuffix)) {
            strcat(locale_path, getenv("APPDIR"));
            strcat(locale_path, lsuffix);
        }
    }
    else {
        strcat(icon_path, isuffix);
        strcat(locale_path, lsuffix);
    }

    setlocale(LC_ALL, "");
    bindtextdomain("janus", locale_path);
    bind_textdomain_codeset("janus", "utf-8");
    textdomain("janus");

    if (icon_path[0] != 0 && access(icon_path, F_OK) == 0) {
        GError * error = NULL;
        GdkPixbuf * icon = gdk_pixbuf_new_from_file(icon_path, &error);
        if (error != NULL) {
            puts(error->message);
            g_error_free(error);
        }
        gtk_window_set_icon(GTK_WINDOW(window), icon);
    }

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
    document.path = NULL;
    document.fontsize = 12;
    document.font = NULL;

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

    if (argc > 1) 
        open_file(argv[1], &document);

    gtk_main();

    gtk_source_finalize();
    free(document.path);

    return 0;
}
