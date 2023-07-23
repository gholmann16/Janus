notes: commands.o menu.o
	cc main.c -g `pkg-config --cflags --libs gtksourceview-4` commands.o menu.o -o Notes
	rm *.o
commands.o:
	cc commands.c -c `pkg-config --cflags gtksourceview-4`
menu.o:
	cc menu.c -c `pkg-config --cflags gtk+-3.0`

clean:
	rm *.o

appimage: commands.o menu.o
	cc -O3 release/AppRun.c -o release/AppRun
	cc -O3 main.c `pkg-config --cflags --libs gtksourceview-4` commands.o menu.o -o Notes
	rm *.o
	strip release/AppRun
	strip Notes
	mkdir -p release/usr/lib
	mkdir -p release/usr/bin
	mv Notes release/usr/bin
	cp /usr/lib/libgtksourceview-4.so.0 release/usr/lib
	appimagetool --exclude-file AppRun.c release