Notes: commands.o menu.o main.o
	cc main.o -g `pkg-config --libs gtksourceview-4` commands.o menu.o -o Notes
main.o: main.c
	cc main.c -c `pkg-config --cflags gtksourceview-4`
commands.o: commands.c
	cc commands.c -c `pkg-config --cflags gtksourceview-4` -Wno-deprecated-declarations
menu.o: menu.c
	cc menu.c -c `pkg-config --cflags gtksourceview-4`

clean:
	rm *.o

appimage: commands.o menu.o main.o
	cc -O3 release/AppRun.c -o release/AppRun
	cc -O3 main.o `pkg-config --libs gtksourceview-4` commands.o menu.o -o Notes
	strip release/AppRun
	strip Notes
	mkdir -p release/usr/lib
	mkdir -p release/usr/bin
	mv Notes release/usr/bin
	cp assets/Notes.png release
	sed -i -e 's#/usr#././#g' release/usr/bin/Notes
	cp /usr/lib/libgtksourceview-4.so.0 release/usr/lib
	appimagetool release
