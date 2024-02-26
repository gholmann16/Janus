SOURCES := $(wildcard *.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))

PSOURCES := $(wildcard po/*.po)
MSOURCES := $(wildcard po/*.mo)
MOBJECTS := $(patsubst po/%.po, po/%.mo, $(PSOURCES))

CFLAGS := `pkg-config --cflags gtksourceview-4`
LDLIBS := `pkg-config --libs gtksourceview-4`

debug: CFLAGS += -g -Og -Wall
debug: notes

release: CFLAGS += -O3
release: notes
	strip $<

notes: $(OBJECTS) $(MOBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDLIBS) $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@
%.mo: %.po
	msgfmt -o $@ $<

update:
	xgettext *.c --keyword=_ -o po/notes.pot --package-name=Notes --msgid-bugs-address=gholmann@neptune.cx
	$(foreach po, $(PSOURCES), msgmerge --update $(po) po/notes.pot;)
	rm -f po/*.po~

clean:
	rm -f *.o
	rm -f po/*.mo
	rm -f notes

install: release
	install -Dm755 notes /usr/bin/notes
	install -Dm644 data/notes.png /usr/share/pixmaps/notes.png
	install -Dm644 data/notes.desktop /usr/share/applications/notes.desktop
	$(foreach object, $(MSOURCES), mkdir -p /usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES; install -Dm644 $(object) /usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES/notes.mo;)

uninstall:
	rm /usr/bin/notes
	rm /usr/share/pixmaps/notes.png
	rm /usr/share/applications/notes.desktop

appdir: $(MOBJECTS)
	mkdir -p appdir
	cp data/notes.desktop appdir/
	mkdir -p appdir/usr/share/pixmaps
	cp data/notes.png appdir/usr/share/pixmaps/notes.png
	ln -fs usr/share/pixmaps/notes.png appdir/notes.png
	$(foreach object, $(MSOURCES), mkdir -p appdir/usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES; cp $(object) appdir/usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES/notes.mo;)

appimage: release appdir
	mv notes appdir/AppRun
	appimagetool appdir
