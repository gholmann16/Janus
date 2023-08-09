#include <gtksourceview/gtksource.h>
#include "main.h"
#include "global.h"

int open_file(char * filename, struct Document * document) {
   
    // Delete buffer
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_start_iter(document->buffer, &start);
    gtk_text_buffer_get_end_iter(document->buffer, &end);

    gtk_text_buffer_delete(document->buffer, &start, &end);
    
    // Get file
    char * contents;
    gsize len;
    GError * err = NULL;
    g_file_get_contents(filename, &contents, &len, &err);

    if (err != NULL) {
        fprintf (stderr, "Unable to read file: %s\n", err->message);
        g_error_free (err);
        return 1;
    }
    
    // Insert file
    gtk_text_buffer_insert(document->buffer, &start, contents, len);
    gtk_text_buffer_set_modified(document->buffer, FALSE);
    strcpy(document->name, filename);
    free(contents);
    
    return 0;
}

void open_command(GtkWidget * self, struct Document * document) {
    
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open File", document->window, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);

    gint res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        char * filename = gtk_file_chooser_get_filename (chooser);
        open_file(filename, document);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

void new_command(void) {
    GError *err = NULL;
    char* argv[] = {"/proc/self/exe", NULL};
    g_spawn_async(NULL, argv, NULL, G_SPAWN_DEFAULT, NULL, NULL, NULL, &err);
    if (err != NULL) {
        fprintf (stderr, "Unable to new window: %s\n", err->message);
        g_error_free (err);
    }
}

int save_as_file(char * filename, struct Document * document) {
    
    strcpy(document->name, filename);

    // Collect all text
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_start_iter(document->buffer, &start);
    gtk_text_buffer_get_end_iter(document->buffer, &end);

    char * text = gtk_text_buffer_get_text(document->buffer, &start, &end, 0);

    FILE * f = fopen(document->name, "w");
    fprintf(f, text);
    fclose(f);

    return 0;
}

void save_as_command(GtkWidget * self, struct Document * document) {
    
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    GtkWidget *dialog = gtk_file_chooser_dialog_new ("Save File", document->window, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Save"), GTK_RESPONSE_ACCEPT, NULL);

    gint res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        char * filename = gtk_file_chooser_get_filename (chooser);
        save_as_file(filename, document);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

void save_command(GtkWidget * self, struct Document * document) {

    //If I want to save as file whenever someone presses control s, switch these two if statements around
    if (gtk_text_buffer_get_modified(document->buffer) == FALSE)
        return;

    if (document->name[0] == '\0') {
        save_as_command(self, document);
        return;
    }

    // Collect all text
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_start_iter(document->buffer, &start);
    gtk_text_buffer_get_end_iter(document->buffer, &end);

    char * text = gtk_text_buffer_get_text(document->buffer, &start, &end, 0);

    FILE * f = fopen(document->name, "w");
    fprintf(f, text);
    fclose(f);

}

void dialog_callback(GtkDialog * self, gint response, struct Document * document) {
    switch (response) {
        case 0:
            GApplication * app = G_APPLICATION(gtk_window_get_application(document->window));
            g_application_quit(app);
            break;
        case 1:
            return;
        case 2:
            save_as_command(GTK_WIDGET(self), document);
            break;
    }
}

void exit_command(GtkWidget * self, struct Document * document) {
    
    if (gtk_text_buffer_get_modified(document->buffer) == FALSE) {
        GApplication * app = G_APPLICATION(gtk_window_get_application(document->window));
        g_application_quit(app);
        return;
    }

    GtkWidget * close = gtk_message_dialog_new(document->window, GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_NONE, "Do you want to save your changes before closing this document?");
    
    gtk_dialog_add_buttons(GTK_DIALOG(close), "No", 0, "Cancel", 1, "Yes", 2, NULL);
    g_signal_connect(close, "response", G_CALLBACK(dialog_callback), document);
    
    gtk_dialog_run (GTK_DIALOG (close));
    gtk_widget_destroy (close);
}

gboolean delete_event(GtkWidget* self, GdkEvent* event, struct Document * document) {
    exit_command(self, document);
    return TRUE;
}

void undo_command(GtkWidget * self, struct Document * document) {
    if (gtk_source_buffer_can_undo(GTK_SOURCE_BUFFER(document->buffer)))
        gtk_source_buffer_undo(GTK_SOURCE_BUFFER(document->buffer));
}

void redo_command(GtkWidget * self, struct Document * document) {
    if (gtk_source_buffer_can_redo(GTK_SOURCE_BUFFER(document->buffer)))
    gtk_source_buffer_redo(GTK_SOURCE_BUFFER(document->buffer));
}

void cut_command(GtkWidget * self, struct Document * document) {
    gtk_text_buffer_cut_clipboard(document->buffer, gtk_clipboard_get_default(gdk_display_get_default()), TRUE);
}

void copy_command(GtkWidget * self, struct Document * document) {
    gtk_text_buffer_copy_clipboard(document->buffer, gtk_clipboard_get_default(gdk_display_get_default()));
}

void paste_command(GtkWidget * self, struct Document * document) {
    gtk_text_buffer_paste_clipboard(document->buffer, gtk_clipboard_get_default(gdk_display_get_default()), NULL, TRUE);
}

void delete_command(GtkWidget * self, struct Document * document) {
    gtk_text_buffer_delete_selection(document->buffer, TRUE, TRUE);
}

void select_all_command(GtkWidget * self, struct Document * document) {

    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_start_iter(document->buffer, &start);
    gtk_text_buffer_get_end_iter(document->buffer, &end);

    gtk_text_buffer_select_range(document->buffer, &start, &end);
}

void search_command(GtkWidget * self, struct Document * document) {

    GtkSourceSearchSettings * settings = gtk_source_search_context_get_settings(document->context);

    GtkWidget * dialog = gtk_dialog_new_with_buttons("Find", document->window, GTK_DIALOG_DESTROY_WITH_PARENT, "Cancel", 0, "Find", 1, NULL);
    GtkWidget * content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    
    GtkWidget * entry = gtk_entry_new();
    const gchar * text = gtk_source_search_settings_get_search_text(settings);
    if (text)
        gtk_entry_set_text(GTK_ENTRY(entry), text);

    GtkWidget * label = gtk_label_new("Find text:");
    GtkWidget * bubble = gtk_check_button_new_with_label("Match case");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(bubble), gtk_source_search_settings_get_case_sensitive(settings));


    gtk_box_pack_start(GTK_BOX(box), label, 0, 0, 0);
    gtk_box_pack_end(GTK_BOX(box), entry, 0, 0, 0);

    gtk_container_add(GTK_CONTAINER(content), box);
    gtk_container_add(GTK_CONTAINER(content), bubble);
    gtk_widget_show_all(content);

    GtkTextIter start_of_selection;
    GtkTextIter start;
    gtk_text_buffer_get_selection_bounds(document->buffer, &start_of_selection, &start);
    
    int res = gtk_dialog_run (GTK_DIALOG (dialog));

    gtk_source_search_settings_set_search_text(settings, gtk_entry_get_text(GTK_ENTRY(entry)));
    gtk_source_search_settings_set_case_sensitive(settings, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(bubble)));

    gtk_widget_destroy (dialog);

    if (res == 1) {        
        GtkTextIter match_start;
        GtkTextIter match_end;

        if(gtk_source_search_context_forward(document->context, &start, &match_start, &match_end, NULL))
            gtk_text_buffer_select_range(document->buffer, &match_start, &match_end);
    }

}