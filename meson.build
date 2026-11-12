project('rsa_tool', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=c11'])

inc_dir = include_directories('include')

sources = [
  'src/main.c'
]

executable('rsa_tool',
  sources,
  include_directories : inc_dir,
  install : true)
