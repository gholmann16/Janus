#include <gtksourceview/gtksource.h>
#include "global.h"

void set_font(struct Document * document) {
    if (document->font == NULL)
        return;

    PangoFontDescription * description = pango_font_description_from_string(document->font);
    if (description == NULL)
        return;
    PangoFontMask mask = pango_font_description_get_set_fields(description);

    char css[1024];
    strcpy(css, "textview {\n");

    if (mask & PANGO_FONT_MASK_STYLE) {
        strcat(css, "\tfont-style: ");
        switch (pango_font_description_get_style(description)) {
            case PANGO_STYLE_NORMAL:
                strcat(css, "normal");
                break;
            case PANGO_STYLE_OBLIQUE:
                strcat(css, "oblique");
                break;
            case PANGO_STYLE_ITALIC:
                strcat(css, "italic");
                break;
        }
        strcat(css, ";\n");
    }
    
    // only CSS2 values supported (For GTK3)
    if (mask & PANGO_FONT_MASK_VARIANT) {
        switch (pango_font_description_get_variant(description)) {
            case PANGO_VARIANT_NORMAL:
                strcat(css, "\tfont-variant: normal;\n");
                break;
            case PANGO_VARIANT_SMALL_CAPS:
                strcat(css, "\tfont-variant: small-caps;\n");
                break;
            default:
                break;
        }
    }

    if (mask & PANGO_FONT_MASK_WEIGHT) {
        strcat(css, "\tfont-weight: ");
        switch (pango_font_description_get_weight(description)) {
            case PANGO_WEIGHT_SEMILIGHT:
            case PANGO_WEIGHT_BOOK:
            case PANGO_WEIGHT_NORMAL:
                strcat(css, "normal");
                break;
            default:
                ; // Required for older c compilers
                char newweight[4] = "000";
                newweight[0] = pango_font_description_get_weight(description)/100 + 0x30;
                strcat(css, newweight);
        }
        strcat(css, ";\n");
    }

    if (mask & PANGO_FONT_MASK_STRETCH) {
        strcat(css, "\tfont-stretch: ");
        switch (pango_font_description_get_stretch(description)) {
            case PANGO_STRETCH_ULTRA_CONDENSED:
                strcat(css, "ultra-condensed");
                break;
            case PANGO_STRETCH_EXTRA_CONDENSED:
                strcat(css, "extra-condensed");
                break;
            case PANGO_STRETCH_CONDENSED:
                strcat(css, "condensed");
                break;
            case PANGO_STRETCH_SEMI_CONDENSED:
                strcat(css, "semi-condensed");
                break;
            case PANGO_STRETCH_NORMAL:
                strcat(css, "normal");
                break;
            case PANGO_STRETCH_SEMI_EXPANDED:
                strcat(css, "semi-expanded");
                break;
            case PANGO_STRETCH_EXPANDED:
                strcat(css, "expanded");
                break;
            case PANGO_STRETCH_EXTRA_EXPANDED:
                strcat(css, "extra-expanded");
                break;
            case PANGO_STRETCH_ULTRA_EXPANDED:
                strcat(css, "ultra-expanded");
                break;
        }
        strcat(css, ";\n");
    }

    if (mask & PANGO_FONT_MASK_SIZE) {
        char size[64];
        sprintf(size, "\tfont-size: %dpt;\n", pango_font_description_get_size (description) / PANGO_SCALE);
        strcat(css, size);
    }

    if (mask & PANGO_FONT_MASK_FAMILY) {
        strcat(css, "\tfont-family: \"");
        g_strlcat(css, pango_font_description_get_family(description), sizeof(css));
        g_strlcat(css, "\";\n", sizeof(css));
    }

    g_strlcat(css, "}\n", 1024);

    GtkCssProvider * cssProvider = gtk_css_provider_new();
    GError * error = NULL;
    gtk_css_provider_load_from_data (cssProvider, css, -1, &error);
    if (error) {
        g_log(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, "%s", error->message);
        g_error_free(error);
    }
    else {
        GtkStyleContext * context = gtk_widget_get_style_context(document->view);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

    g_object_unref(cssProvider);
    pango_font_description_free(description);
}

void font_selected(GtkDialog * dialog, int response_id, struct Document * document) {
    if (response_id != GTK_RESPONSE_OK) {
        return;
    }
    char * selected = gtk_font_chooser_get_font(GTK_FONT_CHOOSER(dialog));
    g_free(document->font);
    document->font = selected;
    set_font(document);
}

void font_command(GtkWidget * self, struct Document * document) {
    GtkWidget * dialog = gtk_font_chooser_dialog_new(_("Fonts"), window);
    g_signal_connect(dialog, "response", G_CALLBACK(font_selected), document);

    if (document->font)
        gtk_font_chooser_set_font(GTK_FONT_CHOOSER(dialog), document->font);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void wrap_command(GtkWidget * self, struct Document * document) {
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(document->view), (gtk_text_view_get_wrap_mode(GTK_TEXT_VIEW(document->view)) == GTK_WRAP_NONE) ? GTK_WRAP_WORD : GTK_WRAP_NONE);
}

void line_number_command(GtkWidget * self, struct Document * document) {
    gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(document->view), !gtk_source_view_get_show_line_numbers(GTK_SOURCE_VIEW(document->view)));
}

void syntax_command(GtkWidget * self, struct Document * document) {
    gtk_source_buffer_set_highlight_syntax(GTK_SOURCE_BUFFER(document->buffer), !gtk_source_buffer_get_highlight_syntax(GTK_SOURCE_BUFFER(document->buffer)));
}
