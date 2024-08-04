desktop_sources = [
  'main.c',
  'desktop-window.c',
]

desktop_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk+-3.0', version: '>= 3.22'),
  dependency('glib-2.0'),
]

gnome = import('gnome')

desktop_sources += gnome.compile_resources('desktop-resources',
  'desktop.gresource.xml',
  c_name: 'desktop'
)

executable('desktop', desktop_sources,
  dependencies: desktop_deps,
  install: true,
)
