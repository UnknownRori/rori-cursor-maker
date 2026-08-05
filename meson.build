project('rcur', 'c')

inc = include_directories('.')
nob = include_directories('./external/nob.h')
rcursor = include_directories('./external/rcursor.h')

main_src = files(
  'main.c',
)

executable('rcur', main_src, include_directories: [inc, nob, rcursor])
