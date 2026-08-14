install_data('dev.pantheum.janus.png', install_dir : get_option('datadir') / 'icons/hicolor/256x256/apps')

metainfo_file = i18n.merge_file(
  input : 'dev.pantheum.janus.metainfo.xml.in',
  output : 'dev.pantheum.janus.metainfo.xml',
  po_dir : '../po',
  install : true,
  install_dir : get_option('datadir') / 'metainfo',
)

appstreamcli = find_program('appstreamcli')

custom_target('desktop-file',
  input : metainfo_file,
  output : 'dev.pantheum.janus.desktop',
  command : [appstreamcli, 'make-desktop-file', '@INPUT@', '@OUTPUT@', '--exec=janus %f'],
  install : true,
  install_dir : get_option('datadir') / 'applications',
)
