#include <libintl.h>
#define _(STRING) gettext(STRING)

struct Document {
    char * path;
    GtkTextBuffer * buffer;
    GtkWidget * view;
    GtkWindow * window;
    GtkSourceSearchContext * context;
    GtkTextIter last;
    _Bool binary;
    char * font;
    _Bool wrap;
    _Bool syntax;
};
