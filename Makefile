SOURCES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%.c, src/%.o, $(SOURCES))

PSOURCES := $(wildcard po/*.po)
MOBJECTS := $(patsubst po/%.po, po/%.mo, $(PSOURCES))

CFLAGS := `pkg-config --cflags gtksourceview-4`
LDLIBS := `pkg-config --libs gtksourceview-4`

VERSION := 0.9.6

debug: CFLAGS += -g -Og -Wall
debug: janus

release: CFLAGS += -O3
release: janus
	strip $<

src/version.h:
	echo "#define VERSION_NUMBER \"$(VERSION)\"" > src/version.h

janus: src/version.h $(OBJECTS) $(MOBJECTS)
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
	rm -f src/version.h

install: release
	install -Dm755 janus $(DESTDIR)/usr/bin/janus
	install -Dm644 data/janus.png $(DESTDIR)/usr/share/icons/hicolor/256x256/apps/janus.png
	install -Dm644 data/janus.desktop $(DESTDIR)/usr/share/applications/janus.desktop
	install -Dm644 data/net.satyrn.janus.metainfo.xml $(DESTDIR)/usr/share/metainfo/net.satyrn.janus.metainfo.xml
	$(foreach object, $(MOBJECTS), install -Dm644 $(object) $(DESTDIR)/usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES/janus.mo;)

uninstall:
	rm /usr/bin/janus
	rm /usr/share/icons/hicolor/256x256/apps/janus.png
	rm /usr/share/applications/janus.desktop
	rm /usr/share/metainfo/net.satyrn.janus.metainfo.xml
	$(foreach object, $(MOBJECTS), rm /usr/share/locale/$(notdir $(basename $(object)))/LC_MESSAGES/janus.mo;)

appimage: DESTDIR += build/AppDir
appimage: install
	sed -Ei '/1$$/!s/version: .*/version: $(VERSION)/g' data/AppImageBuilder.yml
	cd build; appimage-builder --recipe ../data/AppImageBuilder.yml

deb: DESTDIR += build/janus-notepad_$(VERSION)-1
deb: install
	sed -Ei 's/Version:.*/Version: $(VERSION)-1/g' data/control
	install -Dm644 data/control $(DESTDIR)/DEBIAN/control
	sudo chown root:root -R $(DESTDIR)
	dpkg -b $(DESTDIR)
	sudo chown $(id -u):$(id -g) -R $(DESTDIR) 
