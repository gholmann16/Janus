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

update: $(SOURCES)
	xgettext $(SOURCES) --keyword=_ --no-location -o po/temp.pot
	msgmerge --no-location --update --backup=off po/janus.pot po/temp.pot
	rm po/temp.pot
	$(foreach po, $(PSOURCES), msgmerge --no-location --update --backup=off $(po) po/janus.pot;)

.PHONY: update

clean:
	rm -f $(OBJECTS)
	rm -f $(MOBJECTS)
	rm -f janus

install: release
	install -Dm755 janus $(DESTDIR)/usr/bin/janus
	install -Dm644 data/janus.png $(DESTDIR)/usr/share/icons/hicolor/256x256/apps/janus.png
	install -Dm644 data/janus.desktop $(DESTDIR)/usr/share/applications/janus.desktop
	$(foreach object, $(MOBJECTS), install -Dm644 $(object) $(DESTDIR)/usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES/janus.mo;)

uninstall:
	rm /usr/bin/janus
	rm /usr/share/icons/hicolor/256x256/apps/janus.png
	rm /usr/share/applications/janus.desktop
	$(foreach object, $(MOBJECTS), rm /usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES/janus.mo;)

appimage: DESTDIR += AppDir
appimage: install
	appimage-builder --recipe data/AppImageBuilder.yml
