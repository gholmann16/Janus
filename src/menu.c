#include <gtksourceview/gtksource.h>
#include "global.h"
#include "commands.h"

void init_menu(GtkWidget * bar, GtkAccelGroup * accel, struct Document * document) {

    // File menu

    GtkWidget * file = gtk_menu_item_new_with_label(_("File"));
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), file);
    
    GtkWidget * filemenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);

    GtkWidget * new = gtk_menu_item_new_with_label(_("New"));
    gtk_widget_add_accelerator(new, "activate", accel, GDK_KEY_N, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(new, "activate", G_CALLBACK(new_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);

    GtkWidget * open = gtk_menu_item_new_with_label(_("Open"));
    gtk_widget_add_accelerator(open, "activate", accel, GDK_KEY_O, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(open, "activate", G_CALLBACK(open_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);

    GtkWidget * save = gtk_menu_item_new_with_label(_("Save"));
    gtk_widget_add_accelerator(save, "activate", accel, GDK_KEY_S, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(save, "activate", G_CALLBACK(save_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save);

    GtkWidget * save_as = gtk_menu_item_new_with_label(_("Save as"));
    gtk_widget_add_accelerator(save_as, "activate", accel, GDK_KEY_S, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(save_as, "activate", G_CALLBACK(save_as_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save_as);

    GtkWidget * seperate = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), seperate);
    
    GtkWidget * print = gtk_menu_item_new_with_label(_("Print"));
    gtk_widget_add_accelerator(print, "activate", accel, GDK_KEY_P, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(print, "activate", G_CALLBACK(print_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), print);

    GtkWidget * preview = gtk_menu_item_new_with_label(_("Print Preview"));
    gtk_widget_add_accelerator(preview, "activate", accel, GDK_KEY_P, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(preview, "activate", G_CALLBACK(print_preview_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), preview);

    GtkWidget * seperate1 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), seperate1);

    GtkWidget * exit = gtk_menu_item_new_with_label(_("Exit"));
    gtk_widget_add_accelerator(exit, "activate", accel, GDK_KEY_Q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(exit, "activate", G_CALLBACK(exit_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), exit);

    // Edit menu

    GtkWidget * edit = gtk_menu_item_new_with_label(_("Edit"));
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), edit);

    GtkWidget * editmenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit), editmenu);

    GtkWidget * undo = gtk_menu_item_new_with_label(_("Undo"));
    gtk_widget_add_accelerator(undo, "activate", accel, GDK_KEY_Z, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(undo, "activate", G_CALLBACK(undo_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), undo);

    GtkWidget * redo = gtk_menu_item_new_with_label(_("Redo"));
    gtk_widget_add_accelerator(redo, "activate", accel, GDK_KEY_Y, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(redo, "activate", G_CALLBACK(redo_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), redo);

    GtkWidget * seperate2 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), seperate2);

    GtkWidget * cut = gtk_menu_item_new_with_label(_("Cut"));
    gtk_widget_add_accelerator(cut, "activate", accel, GDK_KEY_X, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(cut, "activate", G_CALLBACK(cut_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), cut);

    GtkWidget * copy = gtk_menu_item_new_with_label(_("Copy"));
    gtk_widget_add_accelerator(copy, "activate", accel, GDK_KEY_C, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(copy, "activate", G_CALLBACK(copy_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), copy);

    GtkWidget * paste = gtk_menu_item_new_with_label(_("Paste"));
    gtk_widget_add_accelerator(paste, "activate", accel, GDK_KEY_V, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(paste, "activate", G_CALLBACK(paste_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), paste);

    GtkWidget * delete = gtk_menu_item_new_with_label(_("Delete"));
    gtk_widget_add_accelerator(delete, "activate", accel, GDK_KEY_Delete, 0, GTK_ACCEL_VISIBLE);
    g_signal_connect(delete, "activate", G_CALLBACK(delete_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), delete);

    GtkWidget * seperate3 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), seperate3);

    GtkWidget * select_all = gtk_menu_item_new_with_label(_("Select all"));
    gtk_widget_add_accelerator(select_all, "activate", accel, GDK_KEY_A, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(select_all, "activate", G_CALLBACK(select_all_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(editmenu), select_all);

    GtkWidget * search = gtk_menu_item_new_with_label(_("Search"));
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), search);

    GtkWidget * searchmenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(search), searchmenu);

    GtkWidget * search_button = gtk_menu_item_new_with_label(_("Find"));
    gtk_widget_add_accelerator(search_button, "activate", accel, GDK_KEY_F, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(search_button, "activate", G_CALLBACK(search_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(searchmenu), search_button);
    
    GtkWidget * search_next = gtk_menu_item_new_with_label(_("Find next"));
    gtk_widget_add_accelerator(search_next, "activate", accel, GDK_KEY_G, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(search_next, "activate", G_CALLBACK(search_next_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(searchmenu), search_next);

    GtkWidget * search_previous = gtk_menu_item_new_with_label(_("Find previous"));
    gtk_widget_add_accelerator(search_previous, "activate", accel, GDK_KEY_G, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(search_previous, "activate", G_CALLBACK(search_previous_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(searchmenu), search_previous);

    GtkWidget * replace = gtk_menu_item_new_with_label(_("Replace"));
    gtk_widget_add_accelerator(replace, "activate", accel, GDK_KEY_H, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(replace, "activate", G_CALLBACK(replace_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(searchmenu), replace);

    GtkWidget * seperate4 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(searchmenu), seperate4);

    GtkWidget * go_to = gtk_menu_item_new_with_label(_("Go to"));
    gtk_widget_add_accelerator(go_to, "activate", accel, GDK_KEY_J, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(go_to, "activate", G_CALLBACK(go_to_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(searchmenu), go_to);

    GtkWidget * options = gtk_menu_item_new_with_label(_("Options"));
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), options);

    GtkWidget * optionsmenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(options), optionsmenu);

    GtkWidget * fonts = gtk_menu_item_new_with_label(_("Font"));
    g_signal_connect(fonts, "activate", G_CALLBACK(font_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(optionsmenu), fonts);

    GtkWidget * wrap = gtk_check_menu_item_new_with_label(_("Wrap line"));
    g_signal_connect(wrap, "activate", G_CALLBACK(wrap_command), document);
    if (document->wrap) {
        document->wrap = FALSE;
        gtk_widget_activate(wrap);
    }
    gtk_menu_shell_append(GTK_MENU_SHELL(optionsmenu), wrap);

    GtkWidget * seperate5 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(optionsmenu), seperate5);

    GtkWidget * syntax = gtk_check_menu_item_new_with_label(_("Syntax highlighting"));
    gtk_widget_add_accelerator(syntax, "activate", accel, GDK_KEY_H, GDK_CONTROL_MASK | GDK_SHIFT_MASK, GTK_ACCEL_VISIBLE);
    g_signal_connect(syntax, "activate", G_CALLBACK(syntax_command), document);
    if (document->syntax) {
        document->syntax = FALSE;
        gtk_widget_activate(syntax);
    }
    gtk_menu_shell_append(GTK_MENU_SHELL(optionsmenu), syntax);

    GtkWidget * help = gtk_menu_item_new_with_label(_("Help"));
    gtk_menu_shell_append(GTK_MENU_SHELL(bar), help);

    GtkWidget * helpmenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help), helpmenu);

    GtkWidget * about = gtk_menu_item_new_with_label(_("About"));
    g_signal_connect(about, "activate", G_CALLBACK(about_command), document);
    gtk_menu_shell_append(GTK_MENU_SHELL(helpmenu), about);
}
