#include <libintl.h>
#define _(STRING) gettext(STRING)

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
