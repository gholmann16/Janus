# Janus
Janus is a simple, native linux text editor. Supports UTF-8 and binary data through its fallback binary editing mode. Spiritual successor to leafpad. Intended to be as small and efficient as possible, trading features for a smaller impact on cpu.

## Features
- Undo/redo
- Similar keybinds to other text editors
- Spanish/French/German/Turkish Translations
- Printing capabilities
- Optional syntax highlighting
- Fallback binary editing
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

To build as an rpm package run:
```
mkdir -p ~/rpmbuild/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
echo '%_topdir %(echo $HOME)/rpmbuild' > ~/.rpmmacros
cp data/janus.spec ~/rpmbuild/SPECS/
git archive --output="$HOME/rpmbuild/SOURCES/janus-notepad-0.9.7.tar.gz" --prefix=janus-notepad-0.9.7/ HEAD
rpmbuild -bb ~/rpmbuild/SPECS/janus.spec
```

To build as AppImage run: (requires [go-appimage](https://github.com/probonopd/go-appimage)'s appimagetool)
```
meson build --buildtype release --prefix /usr
ninja -C build
DESTDIR=../AppDir meson install -C build
appimagetool deploy AppDir/usr/share/applications/*.desktop
# Keep only GtkSourceView4 bundled - everything else deploy pulled in (GTK
# core, immodules, print backends, ...) is assumed already on the host.
GSV_LIB=$(find AppDir -name 'libgtksourceview-4.so*' | head -1)
LIBDIR=$(dirname "$GSV_LIB")
find "$LIBDIR" -mindepth 1 -maxdepth 1 -type d -exec rm -rf {} +
find "$LIBDIR" -maxdepth 1 -type f ! -name 'libgtksourceview-4.so*' -delete
install -d AppDir/usr/share/gtksourceview-4
cp -a /usr/share/gtksourceview-4/language-specs /usr/share/gtksourceview-4/styles AppDir/usr/share/gtksourceview-4/
ln -sf usr/share/icons/hicolor/256x256/apps/dev.pantheum.janus.png AppDir/dev.pantheum.janus.png
ARCH=x86_64 VERSION=0.9.7 appimagetool AppDir
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
- [AppImage](https://appimage.github.io/janus)
- [Flatpak](https://flathub.org/apps/dev.pantheum.janus)

## Screenshot
![Picture of app](data/screenshot.png)

## Binary modification

Unlike most other simple text editors, binary editing is supported directly as a fallback, rather than dealing with a codepage. When opened, Janus will display nondisplayable bytes as there unicode equivalent: for example 0xe3 will map to U+e3. To edit binaries, simply open them in Janus, then use Control + Shift + U to enter in new characters (From 0x01 to 0xFF). To enter a null character use U+2400. Once you save the file, Janus will convert these raw characters into hex data.

## Binary mode demo
![Gif showcasing binary editing in Janus](data/demo.gif)
