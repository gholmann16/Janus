notes: commands.o menu.o
	cc main.c -g `pkg-config --cflags --libs gtksourceview-4` commands.o menu.o -o Notes
	rm *.o
commands.o:
	cc commands.c -c `pkg-config --cflags gtksourceview-4`
menu.o:
	cc menu.c -c `pkg-config --cflags gtk+-3.0`

appimage:
	gcc release/AppRun.c -o release/AppRun
	strip release/AppRun
	strip Notes
	mkdir -p release/usr/lib
	mkdir -p release/usr/bin
	cp Notes release/usr/bin
	cp /usr/lib/libgtksourceview-4.so.0 release/usr/lib
	appimagetool --exclude-file AppRun.c release