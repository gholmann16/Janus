void change_indicator(GtkWidget * self, struct Document * editor);

void open_file(char * filename, struct Document * document);

void open_command(GtkWidget * self, struct Document * document);

void new_command(void);

void save_command(GtkWidget * self, struct Document * document);

void save_as_command(GtkWidget * self, struct Document * document);

void print_command(GtkWidget * self, struct Document * document);

void print_preview_command(GtkWidget * self, struct Document * document);

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

void search_next_command(GtkWidget * self, struct Document * document);

void replace_command(GtkWidget * self, struct Document * document);

void go_to_command(GtkWidget * self, struct Document * document);

void font_command(GtkWidget * self, struct Document * document);

void wrap_command(GtkWidget * self, struct Document * document);

void about_command(GtkWidget * self, struct Document * document);