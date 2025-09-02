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
    document->context = context;

    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(document->buffer, &start, &end);
    gtk_source_region_add_subregion(document->last, &start, &end);
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

    GtkTextIter match_start, match_end, last_start, last_end, last_search;
    
    if (!gtk_source_region_get_bounds(document->last, &last_start, &last_end))
        gtk_text_buffer_get_bounds(document->buffer, &last_start, &last_end);
    gboolean (* search_function)(GtkSourceSearchContext *search, const GtkTextIter *iter, GtkTextIter *match_start, GtkTextIter *match_end, gboolean *has_wrapped_around);

    if (pat == SEARCH_FORWARD) {
        search_function = &gtk_source_search_context_forward;
        last_search = last_end;
    }
    else {
        search_function = &gtk_source_search_context_backward;
        last_search = last_start;
    }
    if ((*search_function)(document->context, &last_search, &match_start, &match_end, NULL)) {
        gtk_text_buffer_select_range(document->buffer, &match_start, &match_end);

        gtk_source_region_subtract_subregion(document->last, &last_start, &last_end);
        gtk_source_region_add_subregion(document->last, &match_start, &match_end);

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

    GtkWidget * dialog = gtk_dialog_new_with_buttons(_("Find"), window, GTK_DIALOG_DESTROY_WITH_PARENT, _("Cancel"), 0, _("Find"), 1, NULL);
    GtkWidget * content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_box_set_spacing(GTK_BOX(content), 10);
    gtk_container_set_border_width(GTK_CONTAINER(content), 10);

    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);   
    GtkWidget * label = gtk_label_new(_("Find text:")); 
    GtkWidget * entry = gtk_entry_new();

    const gchar * text = gtk_source_search_settings_get_search_text(settings);
    if (text)
        gtk_entry_set_text(GTK_ENTRY(entry), text);

    gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
    gtk_box_pack_start(GTK_BOX(box), entry, 0, 0, 0);

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

    GtkTextIter start, end;
    if(gtk_source_region_get_bounds(model->document->last, &start, &end) == FALSE)
        return;

    gchar * text = gtk_text_buffer_get_text(model->document->buffer, &start, &end, TRUE);
    if (text && strcmp(text, search_text) == 0) {
        g_free(text);
        gtk_source_search_context_replace(model->document->context, &start, &end, replace_text, strlen(replace_text), NULL);
    }

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

    GtkWidget * dialog = gtk_dialog_new_with_buttons(_("Replace"), window, GTK_DIALOG_DESTROY_WITH_PARENT, NULL, NULL);
    GtkWidget * content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_box_set_spacing(GTK_BOX(content), 10);
    gtk_container_set_border_width(GTK_CONTAINER(content), 10);

    // Search
    GtkWidget * search_label = gtk_label_new(_("Find text:"));
    gtk_label_set_xalign(GTK_LABEL(search_label), 0.0);
    GtkWidget * search_entry = gtk_entry_new();
    GtkWidget * search_button = gtk_button_new_with_label(_("Find"));

    // Replace
    GtkWidget * replace_label = gtk_label_new(_("Replace text"));
    gtk_label_set_xalign(GTK_LABEL(replace_label), 0.0);
    GtkWidget * replace_entry = gtk_entry_new();
    GtkWidget * replace_button = gtk_button_new_with_label(_("Replace"));

    const gchar * text = gtk_source_search_settings_get_search_text(settings);
    if (text)
        gtk_entry_set_text(GTK_ENTRY(search_entry), text);

    // Grid
    GtkGrid * grid = GTK_GRID(gtk_grid_new());
    gtk_grid_set_row_spacing(grid, 10);
    gtk_grid_set_column_spacing(grid, 10);

    gtk_grid_attach(grid, search_label, 0, 0, 1, 1);
    gtk_grid_attach(grid, search_entry, 1, 0, 1, 1);
    gtk_grid_attach(grid, search_button, 2, 0, 1, 1);

    gtk_grid_attach(grid, replace_label, 0, 1, 1, 1);
    gtk_grid_attach(grid, replace_entry, 1, 1, 1, 1);
    gtk_grid_attach(grid, replace_button, 2, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(content), GTK_WIDGET(grid));

    struct SearchModel model = {.document = document, .settings = settings, .search_entry = search_entry, .replace_entry = replace_entry};

    g_signal_connect(search_entry, "activate", G_CALLBACK(search_entry_forward), &model);
    g_signal_connect(search_entry, "activate-backward", G_CALLBACK(search_entry_backward), &model);
    g_signal_connect(replace_entry, "activate", G_CALLBACK(replace_entry_forward), &model);
    g_signal_connect(replace_entry, "activate-backward", G_CALLBACK(replace_entry_backward), &model);

    GtkWidget * match_case_button = gtk_check_button_new_with_label(_("Match case"));
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(match_case_button), gtk_source_search_settings_get_case_sensitive(settings));
    GtkWidget * replace_all_button = gtk_check_button_new_with_label(_("Replace all"));
    GtkWidget * cancel = gtk_button_new_with_label(_("Cancel"));

    gtk_grid_attach(grid, match_case_button, 0, 2, 1, 1);
    gtk_grid_attach(grid, replace_all_button, 0, 3, 1, 1);
    gtk_grid_attach(grid, cancel, 2, 3, 1, 1);

    g_signal_connect(match_case_button, "toggled", G_CALLBACK(match_case), &model);
    g_signal_connect(replace_all_button, "toggled", G_CALLBACK(replace_all), &model);

    g_signal_connect(search_button, "clicked", G_CALLBACK(search_entry_forward), &model);
    g_signal_connect(replace_button, "clicked", G_CALLBACK(replace_entry_forward), &model);

    gtk_source_search_context_set_highlight(document->context, TRUE);
    gtk_widget_show_all(content);

    if(gtk_source_region_get_bounds(document->last, NULL, NULL) == FALSE)
        search(document, SEARCH_FORWARD);

    gtk_dialog_run (GTK_DIALOG (dialog));

    gtk_widget_destroy (dialog);
    gtk_source_search_context_set_highlight(document->context, FALSE);
}

void close_go_to(GtkEntry * self, GtkDialog * dialog) {
    gtk_dialog_response(dialog, 0);
}

void go_to_command(GtkWidget * self, struct Document * document) {
    GtkWidget * dialog = gtk_dialog_new_with_buttons(_("Go to"), window, GTK_DIALOG_DESTROY_WITH_PARENT, _("Go to"), 0, _("Cancel"), 1, NULL);
    GtkWidget * content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gtk_box_set_spacing(GTK_BOX(content), 10);
    gtk_container_set_border_width(GTK_CONTAINER(content), 10);

    GtkWidget * line = gtk_label_new(_("Line number:"));

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(document->buffer, &end);

    GtkWidget * spin = gtk_spin_button_new_with_range(1, gtk_text_iter_get_line(&end) + 1, 1);
    
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_container_add(GTK_CONTAINER(box), line);
    gtk_container_add(GTK_CONTAINER(box), spin);

    gtk_container_add(GTK_CONTAINER(content), box);
    gtk_widget_show_all(content);

    g_signal_connect(spin, "activate", G_CALLBACK(close_go_to), dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == 0) {
        GtkTextIter jump;
        int value = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin)) - 1;
        gtk_text_buffer_get_iter_at_line(document->buffer, &jump, value);
        gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(document->view), &jump, 0.0, TRUE, 0.0, 0.15);
        gtk_text_buffer_place_cursor(document->buffer, &jump);
    }

    gtk_widget_destroy(dialog);
}
