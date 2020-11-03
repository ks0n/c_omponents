project('c_omponents', 'c',
  default_options: [
    'warning_level=3',
    'debug=true',
    'b_sanitize=address',
    'c_std=c99',
    ])

inc = include_directories('include')

src = [
  'src/vec.c',
  ]

library('c_omponents', src, include_directories: inc)

unit_tests_src = [
  'tests/test-vec.c',
  ]

criterion = dependency('criterion')

unit_tests = executable('c_omponents-unit-tests',
  src,
  unit_tests_src,
  dependencies: [criterion],
  include_directories: inc)

test('libc_omponents Unit Tests', unit_tests)