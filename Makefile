SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))

CFLAGS := `pkg-config --cflags gtksourceview-4` -Wall
LDLIBS := `pkg-config --libs gtksourceview-4`

debug: CFLAGS += -g -Og
debug: notes

release: CFLAGS += -O3
release: notes
	strip $<

notes: $(OBJECTS)
	$(CC) $^ -o $@ $(LDLIBS) $(LDFLAGS) 
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

clean:
	rm -f *.o
distclean: clean
	rm -f notes

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