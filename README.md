# Janus
Simple native linux notepad meant for simple UTF-8 text editing. 

## Features
- Binary view only mode
- (Optional) syntax highlighting
- Undo/redo capabilities
- Printing documents
- Similar keybinds to other text editors

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
