INC_FLAGS := `pkg-config --cflags gtksourceview-4`
CFLAGS := -c -g

ifeq ($(RELEASE),yes)
	CFLAGS := $(CFLAGS) -O3
endif

notes: commands.o menu.o main.o
	gcc main.o commands.o menu.o `pkg-config --libs gtksourceview-4` -o notes
main.o: main.c
	cc main.c $(CFLAGS) $(INC_FLAGS)
commands.o: commands.c
	cc commands.c $(CFLAGS) $(INC_FLAGS) -Wno-deprecated-declarations
menu.o: menu.c
	cc menu.c $(CFLAGS) $(INC_FLAGS)

clean:
	rm *.o

install: notes
	strip notes
	install -d /usr/bin/
	install notes /usr/bin/
	install -d /usr/share/pixmaps/
	install data/notes.png /usr/share/pixmaps/
	install -d /usr/share/applications/
	install data/notes.desktop /usr/share/applications/

uninstall:
	rm /usr/bin/notes
	rm /usr/share/pixmaps/notes.png
	rm /usr/share/applications/notes.desktop

appdir:
	mkdir appdir
	cp data/notes.desktop appdir/
	cp data/notes.png appdir/notes.png

appimage: notes appdir
	mv notes appdir/AppRun
	appimagetool appdir