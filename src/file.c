#include <gtksourceview/gtksource.h>
#include "global.h"
#include "config.h"

void change_indicator(GtkWidget * self, struct Document * document) {
    if (gtk_text_buffer_get_modified(GTK_TEXT_BUFFER(self))) {
        const char * current = gtk_window_get_title(window);
        char * prepend = "* ";
        char * title = malloc(strlen(current) + strlen(prepend) + 1);
        strcpy(title, prepend);
        strcat(title, current);
        gtk_window_set_title(window, title);
        free(title);
    }
    else {
        const char * current = gtk_window_get_title(window);
        gtk_window_set_title(window, current + 2);
    }
}

void warning_popup(char * text) {
    g_log(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "%s", text);
    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkWidget * dialog = gtk_message_dialog_new(window, flags, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, text, NULL);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void hide_info(GtkInfoBar * info) {
    gtk_info_bar_set_revealed(info, FALSE);
    gtk_widget_hide(GTK_WIDGET(info));
}

void set_file(struct Document * document, GFile * file) {    
    GFileInfo * info = NULL;
    GError * error = NULL;

    if (g_file_query_exists(file, NULL)) {
        info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME, G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, &error);

        if (error != NULL) {
            g_log(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "%s", error->message);
            g_error_free(error);
            return;
        }

        gtk_window_set_title(window, g_file_info_get_attribute_string(info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME));
        g_object_unref(info);
    }
    else {
        char * name = g_file_get_basename(file);

        if (name == NULL) {
            warning_popup(_("Could not open file."));
            return;
        }

        gtk_window_set_title(window, name);
        free(name);
    }

    const char * old = gtk_window_get_title(window);
    char * append = " - Janus";
    char * title = malloc(strlen(append) + strlen(old) + 1);

    strcpy(title, old);
    strcat(title, append);
    gtk_window_set_title(window, title);
    free(title);

    if (document->file)
        g_object_unref(document->file);
    document->file = file;
}

int select_file(struct Document * document, GtkFileChooserAction action) {
    static GtkFileChooserNative * dialog = NULL;

    if (dialog == NULL) {
        dialog = gtk_file_chooser_native_new((action == GTK_FILE_CHOOSER_ACTION_OPEN) ? _("Open File") : _("Save File"), 
                 window, action, (action == GTK_FILE_CHOOSER_ACTION_OPEN) ? _("Open") : _("Save"), _("Cancel"));
    }

    int res = gtk_native_dialog_run(GTK_NATIVE_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        set_file(document, gtk_file_chooser_get_file(chooser));
    }

    return res;
}

void syntax_update(struct Document * document, char * contents) {
    char * path = g_file_get_path(document->file);
    char * content_type = g_content_type_guess(path, (const unsigned char *)contents, strlen(contents), NULL);

    GtkSourceLanguageManager * manager = gtk_source_language_manager_get_default();
    GtkSourceLanguage * language = gtk_source_language_manager_guess_language (manager, NULL, content_type);
    gtk_source_buffer_set_language(GTK_SOURCE_BUFFER(document->buffer), language);

    g_free(content_type);
    g_free(path);

    gtk_source_buffer_set_highlight_syntax(GTK_SOURCE_BUFFER(document->buffer), gtk_source_buffer_get_highlight_syntax(GTK_SOURCE_BUFFER(document->buffer)));
}

void open_file(struct Document * document, GFile * file) {

    if (g_file_query_exists(file, NULL) == FALSE)
        return;

    char * contents;
    gsize len;
    GError * error = NULL;
    g_file_load_contents(file, NULL, &contents, &len, NULL, &error);
    if (error) {
        warning_popup(_("Could not open file."));
        g_error_free(error);
        return;
    }

    static GtkInfoBar * info = NULL;

    gtk_source_buffer_begin_not_undoable_action(GTK_SOURCE_BUFFER(document->buffer));
    g_signal_handlers_block_by_func(document->buffer, change_indicator, document);
    // Unreadable data
    if (g_utf8_validate(contents, len, NULL) == FALSE) {

        if (!info)  {
            info = GTK_INFO_BAR(gtk_info_bar_new());
            gtk_info_bar_set_revealed(info, FALSE);
            GtkWidget * box = gtk_widget_get_parent(gtk_widget_get_parent(document->view)); // embedded in scrollbar
            gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(info), 0, 0, 0);

            g_signal_connect(info, "close", G_CALLBACK(hide_info), NULL);
            g_signal_connect(info, "response", G_CALLBACK(hide_info), NULL);

            GtkWidget * content = gtk_info_bar_get_content_area(info);
            gtk_info_bar_set_show_close_button(info, TRUE);

            GtkWidget * label = gtk_label_new(NULL);
            gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
            gtk_container_add(GTK_CONTAINER(content), label);

            gtk_info_bar_set_message_type(info, GTK_MESSAGE_QUESTION);
            gtk_label_set_markup(GTK_LABEL(label), _("This file is not formatted properly, and will be edited using Janus' binary mode. For more information check out <a href=\"https://github.com/gholmann16/janus\">the wiki</a>."));
            gtk_widget_show_all(GTK_WIDGET(info));
        }

        gtk_widget_show(GTK_WIDGET(info));
        gtk_info_bar_set_revealed(info, TRUE);

        document->binary = TRUE;

        // Convert to utf8, and then transform characters such as null to glib approriate characters
        // Ascii characters map normally with the exception of NUL (thanks glib)
        // Characters above 127 map to the unicode U+xxxx counterpart for simplicity without choosing any codepage
        size_t newsize = len;
        for (size_t i = 0; i < len; i++) {
            if (!contents[i]) {
                newsize += 2;
            }
            else if (contents[i] < 0) {
                newsize += 1;
            }
        }

        char * binary_string = malloc(newsize + 1);
        size_t iter = 0;

        for (size_t i = 0; i < len; i++) {
            if (contents[i] > 0) {
                binary_string[iter] = contents[i];
                iter += 1;
            }
            else if (contents[i] < 0){
                binary_string[iter+0] = 0xC0 + (2 * (contents[i] & 0x80 && TRUE)) + (1 * (contents[i] & 0x40 && TRUE));
                binary_string[iter+1] = 0x80 + (unsigned char)contents[i] % 64;
                iter += 2;
            }
            else {
                binary_string[iter+0] = 0xE2;
                binary_string[iter+1] = 0x90;
                binary_string[iter+2] = 0x80;
                iter += 3;
            }
        }
        binary_string[iter] = 0;
        gtk_text_buffer_set_text(document->buffer, binary_string, -1);
        free(binary_string);
    }
    // Readable data
    else {
        if (info)
            gtk_info_bar_set_revealed(info, FALSE);
        document->binary = FALSE;
        gtk_text_buffer_set_text(document->buffer, contents, -1);
        syntax_update(document, contents);
    }

    gtk_text_buffer_set_modified(document->buffer, FALSE);
    g_signal_handlers_unblock_by_func(document->buffer, change_indicator, document);
    gtk_source_buffer_end_not_undoable_action(GTK_SOURCE_BUFFER(document->buffer));

    g_free(contents);
}

void open_command(GtkWidget * self, struct Document * document) {
    if(select_file(document, GTK_FILE_CHOOSER_ACTION_OPEN) == GTK_RESPONSE_ACCEPT)
        open_file(document, document->file);
}

void new_command(void) {
    GError * error = NULL;
    char * argv[] = {"/proc/self/exe", NULL};
    g_spawn_async(NULL, argv, NULL, G_SPAWN_DEFAULT, NULL, NULL, NULL, &error);
    if (error != NULL) {
        g_log(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "%s", error->message);
        g_error_free(error);
    }
}

void save(struct Document * document) {

    // Collect all text
    GtkTextIter start;
    GtkTextIter end;

    gtk_text_buffer_get_start_iter(document->buffer, &start);
    gtk_text_buffer_get_end_iter(document->buffer, &end);

    unsigned char * text = (unsigned char *)gtk_text_buffer_get_text(document->buffer, &start, &end, TRUE);
    size_t len;

    if (document->binary) {
        size_t iter = 0;
        len = 0;
        while(text[iter]) {
            if (text[iter] > 127) {
                switch (text[iter]) {
                    case 0xC2:
                        text[len] = text[iter+1];
                        iter++;
                        break;
                    case 0xC3:
                        text[len] = text[iter+1] + 64;
                        iter++;
                        break;
                    case 0xE2:
                        if (text[iter+1] == 0x90 && text[iter+2] == 0x80) {
                            text[len] = 0;
                            iter+=2;
                            break;
                        }
                    default:
                        warning_popup(_("File contains non binary characters. Make sure all inserted characters are between U+0000 and U+00FF with exception U+2400."));
                        free(text);
                        return;
                }
            }
            else
                text[len] = text[iter];
            iter++;
            len++;
        }
        text[len] = 0;
    }
    else
        len = strlen((char *)text);

    GError * error = NULL;
    g_file_replace_contents(document->file, (char *)text, len, NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL, &error);
    if (error) {
        warning_popup(error->message);
        g_error_free(error);
        free(text);
        return;
    }

    if (document->binary == FALSE)
        syntax_update(document, (char *)text);

    gtk_text_buffer_set_modified(document->buffer, FALSE);
    free(text);
}

void save_as_command(GtkWidget * self, struct Document * document) {
    if(select_file(document, GTK_FILE_CHOOSER_ACTION_SAVE) == GTK_RESPONSE_ACCEPT) {
        g_signal_handlers_block_by_func(document->buffer, change_indicator, document);
        save(document);
        g_signal_handlers_unblock_by_func(document->buffer, change_indicator, document);
    }
}

void save_command(GtkWidget * self, struct Document * document) {

    if (document->file == NULL) {
        save_as_command(self, document);
        return;
    }

    if (gtk_text_buffer_get_modified(document->buffer) == FALSE)
        return;

    save(document);

}

void draw_page (GtkPrintOperation* self, GtkPrintContext* context, gint page_nr, GtkSourcePrintCompositor *compositor) {
    gtk_source_print_compositor_draw_page(compositor, context, page_nr);
}

static gboolean paginate (GtkPrintOperation *operation, GtkPrintContext *context, GtkSourcePrintCompositor *compositor) {
    if (gtk_source_print_compositor_paginate(compositor, context)) {
        gtk_print_operation_set_n_pages(operation, gtk_source_print_compositor_get_n_pages (compositor));
        return TRUE;
    }
    return FALSE;
}

void done(GtkPrintOperation * self, GtkPrintOperationResult result, GtkPrintSettings **settings) {
    switch (result) {
        case GTK_PRINT_OPERATION_RESULT_ERROR:
            warning_popup(_("Something went wrong while printing."));
            break;
        case GTK_PRINT_OPERATION_RESULT_APPLY:
            if (*settings != NULL)
                g_object_unref(*settings);
            *settings = g_object_ref(gtk_print_operation_get_print_settings(self));
            break;
        case GTK_PRINT_OPERATION_RESULT_CANCEL:
            break;
        case GTK_PRINT_OPERATION_RESULT_IN_PROGRESS:
            break;
    }

    g_object_unref(self);
}

void print_command(GtkWidget * self, struct Document * document) {
    static GtkPrintSettings * settings = NULL;
    static GtkSourcePrintCompositor * compositor = NULL;

    GtkPrintOperation * print = gtk_print_operation_new();
    gtk_print_operation_set_allow_async(print, TRUE);

    if (compositor == NULL)
        compositor = gtk_source_print_compositor_new(GTK_SOURCE_BUFFER(document->buffer));
    
    if (settings != NULL)
        gtk_print_operation_set_print_settings (print, settings);

    gtk_source_print_compositor_set_print_line_numbers(compositor, gtk_source_view_get_show_line_numbers(GTK_SOURCE_VIEW(document->view)));
    g_signal_connect(print, "draw-page", G_CALLBACK (draw_page), compositor);
    g_signal_connect(print, "paginate", G_CALLBACK (paginate), compositor);
    g_signal_connect(print, "done", G_CALLBACK(done), &settings);

    gtk_print_operation_run(print, GTK_PRINT_OPERATION_ACTION_PRINT_DIALOG, window, NULL);
}

void quit(struct Document * document) {
    GKeyFile * config = g_key_file_new();

    int width, height;
    gtk_window_get_size(window, &width, &height);
    g_key_file_set_integer(config, GROUP_KEY, "width", width);
    g_key_file_set_integer(config, GROUP_KEY, "height", height);

    if (document->font)
        g_key_file_set_string(config, GROUP_KEY, "font", document->font);

    g_key_file_set_boolean(config, GROUP_KEY, "wrap", (gboolean)((gtk_text_view_get_wrap_mode(GTK_TEXT_VIEW(document->view)) == GTK_WRAP_NONE) ? FALSE : TRUE));
    g_key_file_set_boolean(config, GROUP_KEY, "syntax", gtk_source_buffer_get_highlight_syntax(GTK_SOURCE_BUFFER(document->buffer)));
    g_key_file_set_boolean(config, GROUP_KEY, "numbered", gtk_source_view_get_show_line_numbers(GTK_SOURCE_VIEW(document->view)));

    char path[PATH_MAX];
    strcpy(path, (strlen(g_get_user_config_dir()) < PATH_MAX - strlen(CONFIG_FILE)) ? g_get_user_config_dir() : "~/.config");
    strcat(path, CONFIG_FILE);

    GError * error = NULL;
    char * data = g_key_file_to_data(config, NULL, &error);
    if (error) {
        g_log(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "%s", error->message);
        g_error_free(error);
        g_key_file_free(config);
        gtk_main_quit();
    }

    g_file_set_contents(path, data, -1, &error);
    if (error) {
        g_log(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "%s", error->message);
        g_error_free(error);
    }
    g_key_file_free(config);
    gtk_main_quit();
}

void exit_command(GtkWidget * self, struct Document * document) {
    
    if (gtk_text_buffer_get_modified(document->buffer) == FALSE) {
        quit(document);
        return;
    }

    GtkWidget * close = gtk_dialog_new_with_buttons("Janus", window, GTK_DIALOG_MODAL, _("No"), 0, _("Cancel"), 1, _("Yes"), 2, NULL);
    GtkWidget * content = gtk_dialog_get_content_area(GTK_DIALOG(close));
    gtk_box_set_spacing(GTK_BOX(content), 10);
    gtk_container_set_border_width(GTK_CONTAINER(content), 10);
    GtkWidget * message = gtk_label_new(_("Would you like to save?"));

    gtk_container_add(GTK_CONTAINER(content), message);
    gtk_widget_show_all(content);

    int res = gtk_dialog_run (GTK_DIALOG (close));
    gtk_widget_destroy (close);

    switch (res) {
        case 0:
            quit(document);
            break;
        case 1:
            return;
        case 2:
            save_command(GTK_WIDGET(self), document);
            quit(document);
            break;
    }
}

gboolean delete_event(GtkWidget* self, GdkEvent* event, struct Document * document) {
    exit_command(self, document);
    return TRUE;
}
