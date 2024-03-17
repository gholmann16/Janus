struct Document {
    char * path;
    GtkTextBuffer * buffer;
    GtkWidget * view;
    GtkWindow * window;
    GtkSourceSearchContext * context;
    GtkTextIter last;
    gboolean binary;
    char * font;
    int fontsize;
};

#include <libintl.h>
#define _(STRING) gettext(STRING)
