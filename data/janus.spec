Name: janus-notepad
Version: 0.9.7
Release: 1
Summary: Simple linux notepad.
License: GPL-3.0-only
URL: https://github.com/gholmann16/janus
ExclusiveArch: x86_64
Conflicts: janus
Source0: %{name}-%{version}.tar.gz

# Built with --buildtype release + strip=true (see meson.build) - there's no
# debug info to extract, so skip rpm's automatic debuginfo/debugsource split.
%global debug_package %{nil}

BuildRequires: meson, ninja-build, gcc, gettext-devel, gtk3-devel, gtksourceview4-devel, appstream
Requires: gtk3, gtksourceview4

%description
Simple linux notepad.

%prep
%setup -q

%build
meson setup build --buildtype release --prefix /usr
ninja -C build

%install
meson install -C build --destdir %{buildroot}

%files
/usr/bin/janus
/usr/share/applications/dev.pantheum.janus.desktop
/usr/share/icons/hicolor/256x256/apps/dev.pantheum.janus.png
/usr/share/metainfo/dev.pantheum.janus.metainfo.xml
/usr/share/locale/*/LC_MESSAGES/janus.mo
