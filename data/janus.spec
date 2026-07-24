Name: janus-notepad
Version: 0.9.7
Release: 1
Summary: Simple linux notepad.
License: GPL-3.0-only
URL: https://github.com/gholmann16/janus
ExclusiveArch: x86_64
Conflicts: janus

Requires: gtk3, gtksourceview4

%description
Simple linux notepad.

%build

%install

%files
/usr/bin/janus
/usr/share/applications/dev.pantheum.janus.desktop
/usr/share/icons/hicolor/256x256/apps/dev.pantheum.janus.png
/usr/share/metainfo/dev.pantheum.janus.metainfo.xml
/usr/share/locale/*/LC_MESSAGES/janus.mo
