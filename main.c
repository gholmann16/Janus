#include <gtk/gtk.h>
#include "global.h"
#include "menu.h"

int main(int argc, char * argv[]) {

    // Initalize 
    gtk_init(&argc, &argv);

    GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Notes");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    
    // Text part
    GtkWidget * text = gtk_text_view_new();
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text), GTK_WRAP_WORD);
    GtkTextBuffer * buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));

    struct Document doc;
    doc.name[0] = '\0';
    doc.buffer = buffer;

    // Menu setup
    GtkAccelGroup * accel = gtk_accel_group_new();
    gtk_window_add_accel_group(GTK_WINDOW(window), accel);

    GtkWidget * bar = gtk_menu_bar_new();
    init_menu(bar, accel, &doc);

    // Add two parts to box
    gtk_box_pack_start(GTK_BOX(box), bar, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(box), text, 1, 1, 0);

    // Pack up app and run
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all (window);

    gtk_main ();

    return 0;

}