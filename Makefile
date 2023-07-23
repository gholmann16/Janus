notes: commands.o menu.o
	cc main.c -g `pkg-config --cflags --libs gtksourceview-4` commands.o menu.o -o Notes
	rm *.o
commands.o:
	cc commands.c -c `pkg-config --cflags gtksourceview-4`
menu.o:
	cc menu.c -c `pkg-config --cflags gtk+-3.0`

appimage: Notes
	strip Notes
	mv Notes release/AppRun
	cp /usr/lib/libgtksourceview-4.so.0 release/
	appimagetool release