SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c, src/%.o, $(SOURCES))

PSOURCES := $(wildcard po/*.po)
MOBJECTS := $(patsubst po/%.po, po/%.mo, $(PSOURCES))

CFLAGS := `pkg-config --cflags gtksourceview-4`
LDLIBS := `pkg-config --libs gtksourceview-4`

debug: CFLAGS += -g -Og -Wall
debug: janus

release: CFLAGS += -O3
release: janus
	strip $<

janus: $(OBJECTS) $(MOBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDLIBS) $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@
%.mo: %.po
	msgfmt -o $@ $<

update:
	xgettext $(SOURCES) --keyword=_ -o po/janus.pot
	$(foreach po, $(PSOURCES), msgmerge --update $(po) po/janus.pot;)
	rm -f po/*.po~

clean:
	rm -f $(OBJECTS)
	rm -f $(MOBJECTS)
	rm -f janus

install: release
	install -Dm755 janus $(DESTDIR)/usr/bin/janus
	install -Dm644 data/janus.png $(DESTDIR)/usr/share/pixmaps/janus.png
	install -Dm644 data/janus.desktop $(DESTDIR)/usr/share/applications/janus.desktop
	$(foreach object, $(MOBJECTS), install -Dm644 $(object) $(DESTDIR)/usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES/janus.mo;)

uninstall:
	rm /usr/bin/janus
	rm /usr/share/pixmaps/janus.png
	rm /usr/share/applications/janus.desktop
	$(foreach object, $(MOBJECTS), rm /usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES/janus.mo;)

appdir: $(MOBJECTS)
	mkdir -p appdir
	cp data/janus.desktop appdir/
	mkdir -p appdir/usr/share/pixmaps
	cp data/janus.png appdir/usr/share/pixmaps/janus.png
	ln -fs usr/share/pixmaps/janus.png appdir/janus.png
	$(foreach object, $(MOBJECTS), mkdir -p appdir/usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES; cp $(object) appdir/usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES/janus.mo;)

appimage: release appdir
	mv janus appdir/AppRun
	appimagetool appdir
