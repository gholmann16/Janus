#include <gtk/gtk.h>
#include "global.h"
#include "commands.h"

int file_button(GtkWidget * bar, GtkAccelGroup * accel, struct Document * document) {

    GtkWidget * file = gtk_menu_item_new_with_label("File");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), file);
    
    GtkWidget * filemenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);

    GtkWidget * new = gtk_menu_item_new_with_label("New");
    gtk_widget_add_accelerator(new, "activate", accel, GDK_KEY_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(new, "activate", G_CALLBACK(ctrl_n), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);

    GtkWidget * open = gtk_menu_item_new_with_label("Open");
    gtk_widget_add_accelerator(open, "activate", accel, GDK_KEY_O, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(open, "activate", G_CALLBACK(ctrl_o), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);

    GtkWidget * save = gtk_menu_item_new_with_label("Save");
    gtk_widget_add_accelerator(save, "activate", accel, GDK_KEY_S, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(save, "activate", G_CALLBACK(ctrl_s), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save);

    GtkWidget * save_as = gtk_menu_item_new_with_label("Save as...");
    g_signal_connect(save_as, "activate", G_CALLBACK(save_as_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save_as);

    GtkWidget * seperate = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), seperate);

    GtkWidget * exit = gtk_menu_item_new_with_label("Exit");
    g_signal_connect(exit, "activate", G_CALLBACK(exit_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), exit);

    gtk_widget_show_all(filemenu);


    return 0;
}
/*
int edit_button(GtkWidget * bar, GtkWidget * menu[5]) {

    GtkWidget * edit = gtk_menu_item_new_with_label("Edit");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), edit);

    GtkWidget * new = gtk_menu_item_new_with_label("Undo");
    gtk_menu_shell_append(GTK_MENU(menu[1]), new, 0, 1, 0, 1);
    gtk_widget_show(new);

    g_signal_connect(edit, "select", G_CALLBACK(button_selected), menu);

    return 0;
}

int search_button(GtkWidget * bar, GtkWidget * menu[5]) {

    GtkWidget * search = gtk_menu_item_new_with_label("Search");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), search);

    GtkWidget * new = gtk_menu_item_new_with_label("Search");
    gtk_menu_shell_append(GTK_MENU(menu[2]), new, 0, 1, 0, 1);
    gtk_widget_show(new);

    g_signal_connect(search, "select", G_CALLBACK(button_selected), menu);

    return 0;
}

int view_button(GtkWidget * bar) {

}

int help_button(GtkWidget * bar) {

}
*/
