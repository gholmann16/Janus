# Janus
Simple native linux notepad. Built using gtk for the ui, and gtksourceview for undo, redo, and printing. Similar keybinds to leafpad/windows notepad. Meant for simple UTF-8 text editing.

## Build
To build Janus, simply clone the github repository, then run:
```
make release
```

## Installation
Availible on the [AUR](https://aur.archlinux.org/packages/janus) or as an [AppImage](https://github.com/gholmann16/Janus/releases/latest).

To install it manually, run:
```
make release
sudo make install
```

## Dependencies
- Gtk3
- Gtksourceview4

## Screenshot
![Picture of app](data/screenshot.png)
