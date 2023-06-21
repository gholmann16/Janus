#include <gtk/gtk.h>
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
    printf("filename = %s\n", filename);
    free(contents);
    
    return 0;
}

void ctrl_n() {
    main(0, NULL);
}

void ctrl_o(GtkWidget * self, struct Document * document) {
    
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    GtkWidget *dialog = gtk_file_chooser_dialog_new ("Open File", GTK_WINDOW(gtk_widget_get_toplevel(self)), action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);

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

void ctrl_s(GtkWidget * self, struct Document * document) {

    if (gtk_text_buffer_get_modified(document->buffer) == FALSE)
        return;
    
    // Collect all text
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_start_iter(document->buffer, &start);
    gtk_text_buffer_get_end_iter(document->buffer, &end);

    char * text = gtk_text_buffer_get_text(document->buffer, &start, &end, 0);
    
    if (document->name[0] == '\0') {
        printf("You have not opened a file yet. %s\n", document->name);
        return;
    }
    
    FILE * f = fopen(document->name, "w");
    fprintf(f, text);
    fclose(f);

}