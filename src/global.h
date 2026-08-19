#include <libintl.h>
#define _(STRING) gettext(STRING)

extern GtkWindow * window;

struct Document {
    GFile * file;
    PangoFontDescription * font;
    GtkTextBuffer * buffer;
    GtkWidget * view;
    gboolean binary;
};
