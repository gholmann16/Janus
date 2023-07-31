struct Document {
  char name[256];
  GtkTextBuffer * buffer;
  GtkWindow * window;
  GtkSourceSearchContext * context;
};