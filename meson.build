project('rcur', 'c')

inc = include_directories('.')
nob = include_directories('./external/nob.h')
stb = include_directories('./external/stb')
rcursor = include_directories('./external/rcursor.h')

main_src = files(
  'src/cursor.c',
  'src/global.c',
  'src/nob.c',
  'src/png.c',
  'src/util.c',
  'src/stb_impl.c',
  'src/win.c',
  'src/x11.c',
  'config.c',
  'main.c',
)

executable('rcur', main_src, include_directories: [inc, nob, stb, rcursor])
