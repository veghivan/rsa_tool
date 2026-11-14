project('rsa_tool', 'c',
  version : '0.1',
  default_options : ['warning_level=3', 'werror=true', 'c_std=c99'])

inc_dir = include_directories('include')

sources = [
  'src/main.c',
  'src/bn.c'
]

executable('rsa_tool',
  sources,
  include_directories : inc_dir,
  install : true)
