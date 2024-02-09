# Notes
Simple native linux notepad. Built using gtk for the ui, and gtksourceview for undo, redo, and printing. Similar keybinds to leafpad/windows notepad.

## Build
To build notes, simply clone the github repository, the run make.

## Installation
Availible on the [AUR](https://aur.archlinux.org/packages/notes-gtk) or as an [AppImage](https://github.com/gholmann16/Notes/releases/latest).

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
