#include <gtksourceview/gtksource.h>
#include "global.h"

void about_command(GtkWidget * self, struct Document * document) {
    GtkWidget * about_dialog = gtk_about_dialog_new();
    GtkAboutDialog * about = GTK_ABOUT_DIALOG(about_dialog);

    gtk_about_dialog_set_program_name(about, "Janus");
    gtk_about_dialog_set_logo_icon_name(about, "dev.pantheum.janus");

    const char * authors[] = {"Gabriel Holmann <gholmann16@gmail.com>", NULL};
    gtk_about_dialog_set_authors(about, authors);

    gtk_about_dialog_set_license_type(about, GTK_LICENSE_GPL_3_0);

    const char * comments = _("Janus is a simple Linux notepad intended to be small and efficient.");
    gtk_about_dialog_set_comments(about, comments);

    gtk_about_dialog_set_version(about, VERSION_NUMBER);

    const char * website = "https://github.com/gholmann16/Janus";
    gtk_about_dialog_set_website(about, website);

    gtk_dialog_run(GTK_DIALOG(about_dialog));
    gtk_widget_destroy(about_dialog);
}
