#include <gtk/gtk.h>
#include "commands.h"
#include "menu.h"

int main(int argc, char * argv[]) {

    // Initalize 
    gtk_init(&argc, &argv);

    GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Notes");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
    // Menu setup
    GtkWidget * bar = gtk_menu_bar_new();
    GtkWidget * menus[5];

    // Create 5 menus for the 5 future buttons
    for(int x = 0; x < 5; x++) {
        menus[x] = gtk_menu_new();
    }

    file_button(bar, menus, window);

    // Text part`
    GtkWidget * text = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text), GTK_WRAP_WORD);

    // Control key listener
    short control = 0;
    GtkEventController *event_controller = gtk_event_controller_key_new (window);
    g_signal_connect (event_controller, "key-pressed", G_CALLBACK (key_press_handler), &control);
    g_signal_connect (event_controller, "key-released", G_CALLBACK (key_release_handler), &control);

    // Add two parts to box
    gtk_box_pack_start(GTK_BOX(box), bar, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(box), text, 1, 1, 0);

    // Pack up app and run
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all (window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main ();

    return 0;

}