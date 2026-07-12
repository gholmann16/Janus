#include <libintl.h>
#define _(STRING) gettext(STRING)
#define VERSION_NUMBER "0.9.7"

extern GtkWindow * window;

struct Document {
    GFile * file;
    PangoFontDescription * font;
    GtkTextBuffer * buffer;
    GtkWidget * view;
    gboolean binary;
};
