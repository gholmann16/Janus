#include <libintl.h>
#define _(STRING) gettext(STRING)

struct Document {
    char * path;
    char * font;
    GtkTextBuffer * buffer;
    GtkWidget * view;
    GtkWindow * window;
    GtkTextMark * search_start;
    GtkTextMark * search_end;
    GtkSourceSearchContext * context;
    _Bool binary;
    _Bool wrap;
    _Bool line_numbers;
    _Bool syntax;
};
