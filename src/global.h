#include <libintl.h>
#define _(STRING) gettext(STRING)
#define VERSION_NUMBER "0.9.6"

struct Document {
    GFile * file;
    char * font;
    GtkTextBuffer * buffer;
    GtkWidget * view;
    GtkWindow * window;
    GtkSourceRegion * last;
    GtkSourceSearchContext * context;
    gboolean binary;
};
