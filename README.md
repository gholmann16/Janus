# Janus
Janus is a simple native linux notepad meant for text editing. It supports advanced undo and redo, printing, similar keybinds to most other text editors. Janus also includes French and Spanish translations.

## Screenshot
![Picture of app](data/screenshot.png)

## Installation
- [Arch Linux](https://aur.archlinux.org/packages/janus)
- [Debian](https://github.com/gholmann16/Janus/releases/latest)
- [AppImage](https://github.com/gholmann16/Janus/releases/latest).


To build and install run:
```
make release
sudo make install
```
Janus depends on the following libraries for both building and running:
- Gtk3
- Gtksourceview4

## Advanced Features

Janus stands out among most text editors due to its simple design yet advanced capability. Advanced features include syntax highlighting, using GtkSourceView, and binary modification capabilities. To edit binaries, simply open them in Janus, then use Control + Shift + U to enter in new characters (From 0x01 to 0xFF). To enter a null character use U+2400. Once you save the file, Janus will convert these raw characters into hex data.

## Binary mode demo
![Gif showcasing binary editing in Janus](data/demo.gif)
