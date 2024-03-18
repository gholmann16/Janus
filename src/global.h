#include <stdbool.h>
#include <libintl.h>
#define _(STRING) gettext(STRING)

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
    bool wrap;
    bool syntax;
};

