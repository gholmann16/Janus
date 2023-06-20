#include <gtk/gtk.h>
#include "commands.h"

void button_selected(GtkWidget * item, GtkWidget * menu[5]) {

    char * names[5] = {"File", "Edit", "Search", "View", "About"};
    int num = 0;
    for (int x = 0; x < 5; x++) {
        gtk_menu_popdown(GTK_MENU(menu[x]));
        if (strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(item)), names[x]) == 0)
            num = x;
    }

    gtk_menu_popup_at_widget(GTK_MENU(menu[num]), item, GDK_GRAVITY_SOUTH_WEST, GDK_GRAVITY_NORTH_WEST, NULL);
}

int file_button(GtkWidget * bar, GtkWidget * menu[5], GtkWidget * window) {

    GtkWidget * file = gtk_menu_item_new_with_label("File");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), file);

    GtkWidget * new = gtk_menu_item_new_with_label("New");
    GtkWidget * new_label = gtk_bin_get_child(GTK_BIN(new));
    gtk_accel_label_set_accel(GTK_ACCEL_LABEL(new_label), GDK_KEY_N, GDK_CONTROL_MASK);
    g_signal_connect(new, "activate", G_CALLBACK(ctrl_n), NULL);
    gtk_menu_attach(GTK_MENU(menu[0]), new, 0, 1, 0, 1);

    GtkWidget * open = gtk_menu_item_new_with_label("Open");
    GtkWidget * open_label = gtk_bin_get_child(GTK_BIN (open));
    gtk_accel_label_set_accel(GTK_ACCEL_LABEL(open_label), GDK_KEY_O, GDK_CONTROL_MASK);
    g_signal_connect(open, "activate", G_CALLBACK(ctrl_o), window);
    gtk_menu_attach(GTK_MENU(menu[0]), open, 0, 1, 1, 2);

    GtkWidget * save = gtk_menu_item_new_with_label("Save");
    GtkWidget * save_label = gtk_bin_get_child(GTK_BIN(save));
    gtk_accel_label_set_accel(GTK_ACCEL_LABEL(save_label), GDK_KEY_S, GDK_CONTROL_MASK);
    g_signal_connect(save, "activate", G_CALLBACK(ctrl_s), window);
    gtk_menu_attach(GTK_MENU(menu[0]), save, 0, 1, 2, 3);

    /*
    GtkWidget * save_as = gtk_menu_item_new_with_label("Save as...");
    gtk_menu_attach(GTK_MENU(menu[0]), save_as, 0, 1, 3, 4);

    GtkWidget * seperate = gtk_separator_menu_item_new();
    gtk_menu_attach(GTK_MENU(menu[0]), seperate, 0, 1, 4, 5);

    GtkWidget * print = gtk_menu_item_new_with_label("Print");
    gtk_menu_attach(GTK_MENU(menu[0]), print, 0, 1, 5, 6);

    GtkWidget * print_setup = gtk_menu_item_new_with_label("Print setup...");
    gtk_menu_attach(GTK_MENU(menu[0]), print_setup, 0, 1, 6, 7);

    GtkWidget * seperate2 = gtk_separator_menu_item_new();
    gtk_menu_attach(GTK_MENU(menu[0]), seperate2, 0, 1, 7, 8);

    GtkWidget * exit = gtk_menu_item_new_with_label("Exit");
    gtk_menu_attach(GTK_MENU(menu[0]), exit, 0, 1, 8, 9);
    */

    gtk_widget_show_all(menu[0]);

    g_signal_connect(file, "select", G_CALLBACK(button_selected), menu);

    return 0;
}
/*
int edit_button(GtkWidget * bar, GtkWidget * menu[5]) {

    GtkWidget * edit = gtk_menu_item_new_with_label("Edit");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), edit);

    GtkWidget * new = gtk_menu_item_new_with_label("Undo");
    gtk_menu_attach(GTK_MENU(menu[1]), new, 0, 1, 0, 1);
    gtk_widget_show(new);

    g_signal_connect(edit, "select", G_CALLBACK(button_selected), menu);

    return 0;
}

int search_button(GtkWidget * bar, GtkWidget * menu[5]) {

    GtkWidget * search = gtk_menu_item_new_with_label("Search");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), search);

    GtkWidget * new = gtk_menu_item_new_with_label("Search");
    gtk_menu_attach(GTK_MENU(menu[2]), new, 0, 1, 0, 1);
    gtk_widget_show(new);

    g_signal_connect(search, "select", G_CALLBACK(button_selected), menu);

    return 0;
}

int view_button(GtkWidget * bar) {

}

int help_button(GtkWidget * bar) {

}
*/