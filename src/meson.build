gtkdeps = [dependency('gtk+-3.0'), dependency('gtksourceview-4')]
src = ['main.c', 'menu.c', 'file.c', 'edit.c', 'search.c', 'options.c', 'about.c']

version_conf = configuration_data()
version_conf.set_quoted('VERSION_NUMBER', meson.project_version())
configure_file(output: 'version.h', configuration: version_conf)

executable('janus', src, dependencies : gtkdeps, install : true)
