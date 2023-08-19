struct Document {
  char name[256];
  GtkTextBuffer * buffer;
  GtkWidget * view;
  GtkWindow * window;
  GtkSourceSearchContext * context;
};
