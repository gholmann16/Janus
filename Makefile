INC_FLAGS := `pkg-config --cflags gtksourceview-4`

ifeq ($(RELEASE),yes)
	CFLAGS := -c -O3
else
	CFLAGS := -c -g
endif

ifeq ($(PREFIX),)
    PREFIX := /usr
endif

notes: commands.o menu.o main.o
	gcc main.o `pkg-config --libs gtksourceview-4` commands.o menu.o -o notes
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
	install -d $(PREFIX)/bin/
	install notes $(PREFIX)/bin/
	install -d $(PREFIX)/share/pixmaps/
	install assets/notes.png $(PREFIX)/share/pixmaps/
	install -d $(PREFIX)/share/applications/
	install release/notes.desktop $(PREFIX)/share/applications/

appimage: notes
	cc -O3 release/AppRun.c -o release/AppRun
	strip release/AppRun
	strip notes
	mkdir -p release/usr/lib
	mkdir -p release/usr/bin
	mv notes release/usr/bin
	cp assets/notes.png release
	sed -i -e 's#/usr#././#g' release/usr/bin/notes
	cp /usr/lib/libgtksourceview-4.so.0 release/usr/lib
	appimagetool release
