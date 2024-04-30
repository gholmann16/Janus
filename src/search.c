#include <gtksourceview/gtksource.h>
#include "global.h"

enum SearchPattern {
    SEARCH_FORWARD,
    SEARCH_BACKWARD,
};

struct Replace {
    GtkSourceSearchSettings * settings;
    GtkWidget * search_entry;
    GtkWidget * replace_entry;
    _Bool all;
    struct Document * document;
};

void search(struct Document * document, enum SearchPattern pat) {

    GtkTextIter match_start;
    GtkTextIter match_end;
    GtkTextIter last_search;

    gboolean (* search_function)(GtkSourceSearchContext *search, const GtkTextIter *iter, GtkTextIter *match_start, GtkTextIter *match_end, gboolean *has_wrapped_around);

    if (pat == SEARCH_FORWARD) {
        search_function = &gtk_source_search_context_forward;
        gtk_text_buffer_get_iter_at_mark(document->buffer, &last_search, document->search_end);
    }
    else {
        search_function = &gtk_source_search_context_backward;
        gtk_text_buffer_get_iter_at_mark(document->buffer, &last_search, document->search_start);
    }

    if ((*search_function)(document->context, &last_search, &match_start, &match_end, NULL)) {
        gtk_text_buffer_select_range(document->buffer, &match_start, &match_end);

        gtk_text_buffer_move_mark(document->buffer, document->search_start, &match_start);
        gtk_text_buffer_move_mark(document->buffer, document->search_end, &match_end);

        gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(document->view), &match_end, 0.1, FALSE, 0, 0);
    }
}

void search_entry(GtkWidget * entry, struct Document * document) {
    GtkSourceSearchSettings * settings = gtk_source_search_context_get_settings(document->context);
    gtk_source_search_settings_set_search_text(settings, gtk_entry_get_text(GTK_ENTRY(entry)));

    search(document, SEARCH_FORWARD);
}

void match_case(GtkWidget * self, GtkSourceSearchSettings * settings) {
    gtk_source_search_settings_set_case_sensitive(settings, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)));
}

void search_command(GtkWidget * self, struct Document * document) {

    GtkSourceSearchSettings * settings = gtk_source_search_context_get_settings(document->context);

    GtkWidget * dialog = gtk_dialog_new_with_buttons(_("Find"), document->window, GTK_DIALOG_DESTROY_WITH_PARENT, _("Cancel"), 0, _("Find"), 1, NULL);
    GtkWidget * content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    GtkWidget * entry = gtk_entry_new();
    g_signal_connect(entry, "activate", G_CALLBACK(search_entry), document);
    const gchar * text = gtk_source_search_settings_get_search_text(settings);
    if (text)
        gtk_entry_set_text(GTK_ENTRY(entry), text);

    GtkWidget * label = gtk_label_new(_("Find text:"));
    GtkWidget * bubble = gtk_check_button_new_with_label(_("Match case"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bubble), gtk_source_search_settings_get_case_sensitive(settings));
    g_signal_connect(bubble, "toggled", G_CALLBACK(match_case), settings);

    gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
    gtk_box_pack_end(GTK_BOX(box), entry, 0, 0, 0);

    gtk_container_add(GTK_CONTAINER(content), box);
    gtk_container_add(GTK_CONTAINER(content), bubble);
    gtk_widget_show_all(content);

    gtk_source_search_context_set_highlight(document->context, TRUE);
    int res = gtk_dialog_run (GTK_DIALOG (dialog));

    if (res == 1) 
        search_entry(entry, document);

    gtk_widget_destroy (dialog);
    gtk_source_search_context_set_highlight(document->context, FALSE);
}

void search_next_command(GtkWidget * self, struct Document * document) {
    search(document, SEARCH_FORWARD);
}

void search_previous_command(GtkWidget * self, struct Document * document) {
    search(document, SEARCH_BACKWARD);
}

void search_to_replace(GtkWidget * self, struct Replace * replace) {
    search_entry(replace->search_entry, replace->document);
}

void replace(GtkWidget * self, struct Replace * replace) {

    const char * replace_text = gtk_entry_get_text(GTK_ENTRY(replace->replace_entry));
    const char * search_text = gtk_entry_get_text(GTK_ENTRY(replace->search_entry));
    gtk_source_search_settings_set_search_text(replace->settings, search_text);
    
    if (replace->all) {
        gtk_source_search_context_replace_all(replace->document->context, replace_text, strlen(replace_text), NULL);
        return;
    }

    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_iter_at_mark(replace->document->buffer, &start, replace->document->search_start);
    gtk_text_buffer_get_iter_at_mark(replace->document->buffer, &end, replace->document->search_end);

    if (strcmp(gtk_text_buffer_get_text(replace->document->buffer, &start, &end, FALSE), search_text)) {
        search(replace->document, SEARCH_FORWARD);
        gtk_text_buffer_get_iter_at_mark(replace->document->buffer, &start, replace->document->search_start);
        gtk_text_buffer_get_iter_at_mark(replace->document->buffer, &end, replace->document->search_end);
    }

    gtk_source_search_context_replace(replace->document->context, &start, &end, replace_text, strlen(replace_text), NULL);

    search(replace->document, SEARCH_FORWARD);
}

void replace_all (GtkWidget * self, struct Replace * rep) {
    rep->all = !rep->all;
}

void replace_command(GtkWidget * self, struct Document * document) {

    GtkSourceSearchSettings * settings = gtk_source_search_context_get_settings(document->context);

    GtkWidget * dialog = gtk_dialog_new_with_buttons(_("Replace"), document->window, GTK_DIALOG_DESTROY_WITH_PARENT, _("Cancel"), 0, NULL);
    GtkWidget * content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Search
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget * label = gtk_label_new(_("Find text:"));
    GtkWidget * entry = gtk_entry_new();
    g_signal_connect(entry, "activate", G_CALLBACK(search_entry), document);
    const gchar * text = gtk_source_search_settings_get_search_text(settings);
    if (text)
        gtk_entry_set_text(GTK_ENTRY(entry), text);
    GtkWidget * search_button = gtk_button_new_with_label(_("Find"));

    gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(box), search_button, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(content), box);

    // Replace
    GtkWidget * box2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget * label2 = gtk_label_new(_("Replace text"));
    GtkWidget * entry2 = gtk_entry_new();
    GtkWidget * replace_button = gtk_button_new_with_label(_("Replace"));

    struct Replace rep = {
        settings,
        entry,
        entry2,
        0,
        document
    };

    gtk_box_pack_start(GTK_BOX(box2), label2, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(box2), entry2, 0, 0, 0);
    g_signal_connect(entry2, "activate", G_CALLBACK(replace), &rep);
    gtk_box_pack_start(GTK_BOX(box2), replace_button, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(content), box2);

    GtkWidget * bubble = gtk_check_button_new_with_label(_("Match case"));
    GtkWidget * bubble2 = gtk_check_button_new_with_label(_("Replace all"));

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bubble), gtk_source_search_settings_get_case_sensitive(settings));
    g_signal_connect(bubble, "toggled", G_CALLBACK(match_case), settings);
    g_signal_connect(bubble2, "toggled", G_CALLBACK(replace_all), &rep);

    gtk_container_add(GTK_CONTAINER(content), bubble);
    gtk_container_add(GTK_CONTAINER(content), bubble2);

    g_signal_connect(search_button, "clicked", G_CALLBACK(search_to_replace), &rep);
    g_signal_connect(replace_button, "clicked", G_CALLBACK(replace), &rep);

    gtk_widget_show_all(content);

    gtk_source_search_context_set_highlight(document->context, TRUE);
    gtk_dialog_run (GTK_DIALOG (dialog));

    gtk_widget_destroy (dialog);
    gtk_source_search_context_set_highlight(document->context, FALSE);
}