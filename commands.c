#include <gtk/gtk.h>
#include "main.h"

int open_file(char * filename, GtkWidget * window) {
    // Find buffer
    GtkWidget * box = gtk_container_get_focus_child(GTK_CONTAINER(window));
    GList * list = gtk_container_get_children(GTK_CONTAINER(box));
    GtkWidget * view = list->next->data;
    GtkTextBuffer * buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    g_list_free(list);
    
    // Delete buffer
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_start_iter(buf, &start);
    gtk_text_buffer_get_end_iter(buf, &end);

    gtk_text_buffer_delete(buf, &start, &end);
    
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
    gtk_text_buffer_insert(buf, &start, contents, len);
    gtk_text_buffer_set_modified(buf, FALSE);
    gtk_widget_set_name(GTK_WIDGET(view), filename);
    printf("filename = %s\n", filename);
    free(contents);
    
    return 0;
}

void ctrl_n() {
    main(0, NULL);
}

void ctrl_o(GtkWidget * self, GtkWidget * window) {
    
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW(window), action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);

    gint res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        char * filename = gtk_file_chooser_get_filename (chooser);
        open_file(filename, window);
        g_free (filename);
    }

    gtk_widget_destroy (dialog);
}

void ctrl_s(GtkWidget * self, GtkWidget * window) {
    // Retrieve text buffer
    GtkWidget * box = gtk_container_get_focus_child(GTK_CONTAINER(window));
    GList * list = gtk_container_get_children(GTK_CONTAINER(box));
    GtkWidget * view = list->next->data;
    GtkTextBuffer * buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    g_list_free(list);

    if (gtk_text_buffer_get_modified(buf) == FALSE)
        return;
    
    // Collect all text
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_start_iter(buf, &start);
    gtk_text_buffer_get_end_iter(buf, &end);

    char * text = gtk_text_buffer_get_text(buf, &start, &end, 0);
    const char * name = gtk_widget_get_name(GTK_WIDGET(view));
    
    if (strcmp(name, "GtkTextView") == 0) {
        printf("You have not opened a file yet. %s\n", name);
        return;
    }

    FILE * f = fopen(name, "w");
    fprintf(f, text);
    fclose(f);

}

void key_press_handler(GtkEventControllerKey* self, guint keyval, guint keycode, GdkModifierType* state, short * control) {
    if (keycode == 37)
        *control = 1;
}

void key_release_handler(GtkEventControllerKey* self, guint keyval, guint keycode, GdkModifierType* state, short * control) {
    if (*control == 1) {
        GtkWidget * window = gtk_event_controller_get_widget(GTK_EVENT_CONTROLLER(self));
        switch (keycode) {
            case 57:
                ctrl_n();
                break;
            case 32:
                ctrl_o(NULL, window);
                break;
            case 37:
                ctrl_s(NULL, window);
                break;
        }
        *control = 0;
    }
}