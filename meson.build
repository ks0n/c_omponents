project('c_omponents', 'c',
  default_options: [
    'warning_level=3',
    'debug=true',
    'b_sanitize=address',
    'c_std=c99',
    ])

inc = include_directories('include')

src = [
  'src/hmap.c',
  'src/vec.c',
  'src/g_array.c',
  ]

c_omponents_dep = library('c_omponents', src, include_directories: inc)

unit_tests_src = [
  'tests/test-vec.c',
  ]

if get_option('enable_tests')
    criterion = dependency('criterion')

    unit_tests = executable('c_omponents-unit-tests',
      src,
      unit_tests_src,
      dependencies: [criterion],
      override_options: ['b_coverage=true'],
      include_directories: inc)

    test('libc_omponents Unit Tests', unit_tests)
else
    warning('Tests are disabled. Run ``meson -Denable_tests=true``')
endif
