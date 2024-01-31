INC_FLAGS := `pkg-config --cflags gtksourceview-4`
CFLAGS := -c -g

notes: commands.o menu.o main.o
	gcc main.o commands.o menu.o `pkg-config --libs gtksourceview-4` -o notes
main.o: main.c
	cc main.c $(CFLAGS) $(INC_FLAGS)
commands.o: commands.c
	cc commands.c $(CFLAGS) $(INC_FLAGS)
menu.o: menu.c
	cc menu.c $(CFLAGS) $(INC_FLAGS)

clean:
	rm -f *.o

release: CFLAGS += -O3
release: clean notes
	strip notes

install: release
	install -Dm755 notes /usr/bin/notes
	install -Dm644 data/notes.png /usr/share/pixmaps/notes.png
	install -Dm644 data/notes.desktop /usr/share/applications/notes.desktop

uninstall:
	rm /usr/bin/notes
	rm /usr/share/pixmaps/notes.png
	rm /usr/share/applications/notes.desktop

appdir:
	mkdir appdir
	cp data/notes.desktop appdir/
	cp data/notes.png appdir/notes.png

appimage: release appdir
	mv notes appdir/AppRun
	appimagetool appdir