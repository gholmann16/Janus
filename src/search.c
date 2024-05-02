#include <gtksourceview/gtksource.h>
#include "global.h"

void search_init(struct Document * document) {
    // Set up signals for search's
    g_signal_new("activate-backward", GTK_TYPE_ENTRY, G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION, 0, NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
    GObjectClass * entry_class = g_type_class_ref (GTK_TYPE_ENTRY);
    GtkBindingSet * binding_set = gtk_binding_set_by_class (entry_class);
    gtk_binding_entry_add_signal(binding_set, GDK_KEY_KP_Enter, GDK_SHIFT_MASK, "activate-backward", 0);
    gtk_binding_entry_add_signal(binding_set, GDK_KEY_Return, GDK_SHIFT_MASK, "activate-backward", 0);

    // Initiate search functionality
    GtkSourceSearchContext * context = gtk_source_search_context_new (GTK_SOURCE_BUFFER(document->buffer), NULL);
    gtk_source_search_context_set_highlight(context, FALSE);
    gtk_source_search_settings_set_wrap_around(gtk_source_search_context_get_settings(context), TRUE);
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(document->buffer, &start, &end);

    document->context = context;
    document->search_start = gtk_text_buffer_create_mark(document->buffer, NULL, &start, FALSE);
    document->search_end = gtk_text_buffer_create_mark(document->buffer, NULL, &end, FALSE);
}

enum SearchPattern {
    SEARCH_FORWARD,
    SEARCH_BACKWARD,
};

struct SearchModel {
    struct Document * document;
    GtkSourceSearchSettings * settings;
    GtkWidget * search_entry;
    GtkWidget * replace_entry;
    _Bool all;
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

void search_entry_forward(GtkWidget * entry, struct SearchModel * model) {
    gtk_source_search_settings_set_search_text(model->settings, gtk_entry_get_text(GTK_ENTRY(model->search_entry)));
    search(model->document, SEARCH_FORWARD);
}

void search_entry_backward(GtkWidget * entry, struct SearchModel * model) {
    gtk_source_search_settings_set_search_text(model->settings, gtk_entry_get_text(GTK_ENTRY(model->search_entry)));
    search(model->document, SEARCH_BACKWARD);
}

void match_case(GtkWidget * self, GtkSourceSearchSettings * settings) {
    gtk_source_search_settings_set_case_sensitive(settings, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self)));
}

void search_command(GtkWidget * self, struct Document * document) {

    GtkSourceSearchSettings * settings = gtk_source_search_context_get_settings(document->context);

    GtkWidget * dialog = gtk_dialog_new_with_buttons(_("Find"), document->window, GTK_DIALOG_DESTROY_WITH_PARENT, _("Cancel"), 0, _("Find"), 1, NULL);
    GtkWidget * content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);   
    GtkWidget * label = gtk_label_new(_("Find text:")); 
    GtkWidget * entry = gtk_entry_new();

    const gchar * text = gtk_source_search_settings_get_search_text(settings);
    if (text)
        gtk_entry_set_text(GTK_ENTRY(entry), text);

    gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
    gtk_box_pack_end(GTK_BOX(box), entry, 0, 0, 0);

    struct SearchModel model = {.document = document, .settings = settings, .search_entry = entry};

    g_signal_connect(entry, "activate", G_CALLBACK(search_entry_forward), &model);
    g_signal_connect(entry, "activate-backward", G_CALLBACK(search_entry_backward), &model);

    GtkWidget * match_case_button = gtk_check_button_new_with_label(_("Match case"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(match_case_button), gtk_source_search_settings_get_case_sensitive(settings));
    g_signal_connect(match_case_button, "toggled", G_CALLBACK(match_case), settings);

    gtk_container_add(GTK_CONTAINER(content), box);
    gtk_container_add(GTK_CONTAINER(content), match_case_button);
    gtk_widget_show_all(content);

    gtk_source_search_context_set_highlight(document->context, TRUE);
    int res = gtk_dialog_run (GTK_DIALOG (dialog));

    if (res == 1) 
        search(document, SEARCH_FORWARD);

    gtk_widget_destroy (dialog);
    gtk_source_search_context_set_highlight(document->context, FALSE);
}

void search_next_command(GtkWidget * self, struct Document * document) {
    search(document, SEARCH_FORWARD);
}

void search_previous_command(GtkWidget * self, struct Document * document) {
    search(document, SEARCH_BACKWARD);
}

void replace(struct SearchModel * model, enum SearchPattern direction) {

    const char * replace_text = gtk_entry_get_text(GTK_ENTRY(model->replace_entry));
    const char * search_text = gtk_entry_get_text(GTK_ENTRY(model->search_entry));
    gtk_source_search_settings_set_search_text(model->settings, search_text);
    
    if (model->all) {
        gtk_source_search_context_replace_all(model->document->context, replace_text, strlen(replace_text), NULL);
        return;
    }

    GtkTextIter start;
    GtkTextIter end;
    gtk_text_buffer_get_iter_at_mark(model->document->buffer, &start, model->document->search_start);
    gtk_text_buffer_get_iter_at_mark(model->document->buffer, &end, model->document->search_end);

    if (strcmp(gtk_text_buffer_get_text(model->document->buffer, &start, &end, FALSE), search_text)) {
        search(model->document, direction);
        gtk_text_buffer_get_iter_at_mark(model->document->buffer, &start, model->document->search_start);
        gtk_text_buffer_get_iter_at_mark(model->document->buffer, &end, model->document->search_end);
    }

    gtk_source_search_context_replace(model->document->context, &start, &end, replace_text, strlen(replace_text), NULL);

    search(model->document, direction);
}

void replace_entry_forward(GtkWidget * self, struct SearchModel * model) {
    replace(model, SEARCH_FORWARD);
}

void replace_entry_backward(GtkWidget * self, struct SearchModel * model) {
    replace(model, SEARCH_BACKWARD);
}

void replace_all (GtkWidget * self, struct SearchModel * model) {
    model->all = !model->all;
}

void replace_command(GtkWidget * self, struct Document * document) {

    GtkSourceSearchSettings * settings = gtk_source_search_context_get_settings(document->context);

    GtkWidget * dialog = gtk_dialog_new_with_buttons(_("Replace"), document->window, GTK_DIALOG_DESTROY_WITH_PARENT, _("Cancel"), 0, NULL);
    GtkWidget * content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Search
    GtkWidget * search_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget * search_label = gtk_label_new(_("Find text:"));
    GtkWidget * search_entry = gtk_entry_new();
    GtkWidget * search_button = gtk_button_new_with_label(_("Find"));

    // Replace
    GtkWidget * replace_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    GtkWidget * replace_label = gtk_label_new(_("Replace text"));
    GtkWidget * replace_entry = gtk_entry_new();
    GtkWidget * replace_button = gtk_button_new_with_label(_("Replace"));

    const gchar * text = gtk_source_search_settings_get_search_text(settings);
    if (text)
        gtk_entry_set_text(GTK_ENTRY(search_entry), text);

    gtk_box_pack_start(GTK_BOX(search_box), search_label, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(search_box), search_entry, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(search_box), search_button, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(content), search_box);

    gtk_box_pack_start(GTK_BOX(replace_box), replace_label, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(replace_box), replace_entry, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(replace_box), replace_button, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(content), replace_box);

    struct SearchModel model = {.document = document, .settings = settings, .search_entry = search_entry, .replace_entry = replace_entry};

    g_signal_connect(search_entry, "activate", G_CALLBACK(search_entry_forward), &model);
    g_signal_connect(search_entry, "activate-backward", G_CALLBACK(search_entry_backward), &model);
    g_signal_connect(replace_entry, "activate", G_CALLBACK(replace_entry_forward), &model);
    g_signal_connect(replace_entry, "activate-backward", G_CALLBACK(replace_entry_backward), &model);

    GtkWidget * match_case_button = gtk_check_button_new_with_label(_("Match case"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(match_case_button), gtk_source_search_settings_get_case_sensitive(settings));
    GtkWidget * replace_all_button = gtk_check_button_new_with_label(_("Replace all"));

    gtk_container_add(GTK_CONTAINER(content), match_case_button);
    gtk_container_add(GTK_CONTAINER(content), replace_all_button);

    g_signal_connect(match_case_button, "toggled", G_CALLBACK(match_case), &model);
    g_signal_connect(replace_all_button, "toggled", G_CALLBACK(replace_all), &model);

    g_signal_connect(search_button, "clicked", G_CALLBACK(search_entry_forward), &model);
    g_signal_connect(replace_button, "clicked", G_CALLBACK(replace_entry_forward), &model);

    gtk_source_search_context_set_highlight(document->context, TRUE);
    gtk_widget_show_all(content);
    gtk_dialog_run (GTK_DIALOG (dialog));

    gtk_widget_destroy (dialog);
    gtk_source_search_context_set_highlight(document->context, FALSE);
}