void open_command(GtkWidget * self, struct Document * document);

void new_command(void);

void save_command(GtkWidget * self, struct Document * document);

void save_as_command(GtkWidget * self, struct Document * document);

void exit_command(GtkWidget * self, struct Document * document);

gboolean delete_event(GtkWidget* self, GdkEvent* event, struct Document * document);

void undo_command(GtkWidget * self, struct Document * document);

void redo_command(GtkWidget * self, struct Document * document);

void cut_command(GtkWidget * self, struct Document * document);

void copy_command(GtkWidget * self, struct Document * document);

void paste_command(GtkWidget * self, struct Document * document);

void delete_command(GtkWidget * self, struct Document * document);

void select_all_command(GtkWidget * self, struct Document * document);

void search_command(GtkWidget * self, struct Document * document);

void about_command(GtkWidget * self, struct Document * document);