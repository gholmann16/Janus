#include <gtk/gtk.h>
#include "global.h"
#include "commands.h"

int init_menu(GtkWidget * bar, GtkAccelGroup * accel, struct Document * document) {

    // File menu

    GtkWidget * file = gtk_menu_item_new_with_label("File");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), file);
    
    GtkWidget * filemenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);

    GtkWidget * new = gtk_menu_item_new_with_label("New");
    gtk_widget_add_accelerator(new, "activate", accel, GDK_KEY_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(new, "activate", G_CALLBACK(new_command), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);

    GtkWidget * open = gtk_menu_item_new_with_label("Open");
    gtk_widget_add_accelerator(open, "activate", accel, GDK_KEY_O, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(open, "activate", G_CALLBACK(open_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);

    GtkWidget * save = gtk_menu_item_new_with_label("Save");
    gtk_widget_add_accelerator(save, "activate", accel, GDK_KEY_S, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(save, "activate", G_CALLBACK(save_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save);

    GtkWidget * save_as = gtk_menu_item_new_with_label("Save as...");
    g_signal_connect(save_as, "activate", G_CALLBACK(save_as_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save_as);

    GtkWidget * seperate = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), seperate);

    GtkWidget * exit = gtk_menu_item_new_with_label("Exit");
    g_signal_connect(exit, "activate", G_CALLBACK(exit_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), exit);
    g_signal_connect(gtk_widget_get_toplevel(exit), "destroy", G_CALLBACK(exit_command), document);
    g_signal_connect(gtk_widget_get_toplevel(exit), "delete-event", G_CALLBACK(exit_command), document);

    // Edit menu

    /*

    GtkWidget * edit = gtk_menu_item_new_with_label("Edit");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), edit);

    GtkWidget * editmenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), editmenu);

    GtkWidget * undo = gtk_menu_item_new_with_label("Undo");
    g_signal_connect(undo, "activate", G_CALLBACK(undo_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), undo);

    GtkWidget * search = gtk_menu_item_new_with_label("Search");
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), search);

    GtkWidget * searchmenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(search), searchmenu);

    GtkWidget * search = gtk_menu_item_new_with_label("Search");
    gtk_menu_shell_append(GTK_MNEU_SHELL(searchmenu), search);

    */
    return 0;
}