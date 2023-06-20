int open_file(char * filename, GtkWidget * window);
void ctrl_o(GtkWidget * self, GtkWidget * window);

void ctrl_n(void);

void ctrl_s(GtkWidget * self, GtkWidget * window);

void key_press_handler(GtkEventControllerKey* self, guint keyval, guint keycode, GdkModifierType* state, short * control);
void key_release_handler(GtkEventControllerKey* self, guint keyval, guint keycode, GdkModifierType* state, short * control);