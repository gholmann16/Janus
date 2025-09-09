# Janus
Janus is a simple, native linux text editor. Supports UTF-8 and binary data through it's binary editing mode. Spiritual successor to leafpad.

## Features
- Undo/redo
- Similar keyinds to other text editors
- Spanish/French/German Translations
- Printing capabilities
- Optional syntax highlighting
- Binary editing
- GVFS connectivity

## Building

To build and install locally run:
```
meson build --buildtype release
ninja -C build
meson install -C build
```

To uninstall it run: (if ninja has such functionality on your system)
```
sudo ninja -C build uninstall
```

To build as a deb package run:
```
meson build --buildtype release --prefix /usr
ninja -C build
DESTDIR=../janus-notepad_0.9.7-1 meson install -C build
install -Dm644 data/control janus-notepad_0.9.7-1/DEBIAN/control
dpkg-deb --root-owner-group -b janus-notepad_0.9.7-1
```

To build as AppImage run: (Requires most recent commit version of appimage-builder for zstd)
```
meson build --buildtype release --prefix /usr
ninja -C build
DESTDIR=../AppDir meson install -C build
appimage-builder --recipe data/AppImageBuilder.yml --appdir AppDir --build-dir /tmp
```

To build as Flatpak run:
```
flatpak-builder --user --install --force-clean flatpak/ data/dev.pantheum.janus.yml
```

Janus depends on the following libraries for both building and running:
- Gtk3
- Gtksourceview4

## Installation

- [Arch Linux](https://aur.archlinux.org/packages/janus)
- [Debian](https://github.com/gholmann16/Janus/releases/latest)
- [AppImage](https://https://appimage.github.io/janus)
- [Flatpak](https://flathub.org/apps/dev.pantheum.janus)

## Screenshot
![Picture of app](data/screenshot.png)

## Binary modification

Unlike most other simple text editors, binary editing is supported directly, rather than dealing with a codepage. When opened, Janus will display nondisplayable bytes as there unicode equivalent: for example 0xe3 will map to U+e3. To edit binaries, simply open them in Janus, then use Control + Shift + U to enter in new characters (From 0x01 to 0xFF). To enter a null character use U+2400. Once you save the file, Janus will convert these raw characters into hex data.

## Binary mode demo
![Gif showcasing binary editing in Janus](data/demo.gif)
