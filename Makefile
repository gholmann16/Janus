notes: commands.o menu.o
	cc main.c -g `pkg-config --cflags --libs gtk+-3.0` commands.o menu.o -o Notes
	rm *.o
commands.o:
	cc commands.c -c `pkg-config --cflags --libs gtk+-3.0`
menu.o:
	cc menu.c -c `pkg-config --cflags --libs gtk+-3.0`

appimage: Notes
	strip Notes
	mv Notes release/AppRun
	appimagetool release