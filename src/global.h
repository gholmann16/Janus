#include <libintl.h>
#define _(STRING) gettext(STRING)
#define VERSION_NUMBER "0.9.6"

extern GtkWindow * window;

struct Document {
    GFile * file;
    char * font;
    GtkTextBuffer * buffer;
    GtkWidget * view;
    GtkSourceRegion * last;
    GtkSourceSearchContext * context;
    gboolean binary;
};
