struct Document {
    char * path;
    GtkTextBuffer * buffer;
    GtkWidget * view;
    GtkWindow * window;
    GtkSourceSearchContext * context;
    GtkTextIter last;
    gboolean binary;
};
