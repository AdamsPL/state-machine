project('state_machine', 'cpp', default_options: ['cpp_std=c++17', 'warning_level=3', 'werror=true', 'optimization=3'])

objdump = find_program('objdump')

parts = ['part1', 'part2', 'part3']

foreach part: parts
  optimized = executable(part + '-opt', part + '/example.cpp', include_directories: include_directories(part), override_options:['buildtype=release'])
  testable = executable(part, part + '/example.cpp', include_directories: include_directories(part), override_options:['buildtype=debug', 'b_sanitize=address,undefined'])
  run_target('run-' + part, command: testable.full_path() , depends: testable)
  run_target('dis-' + part, command: [objdump, '-dC', optimized.full_path(), '-M', 'intel'], depends: optimized)
endforeach
