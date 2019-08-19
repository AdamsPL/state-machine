project('state_machine', 'cpp', default_options : ['cpp_std=c++17'])

add_global_arguments(['-Wall', '-pedantic', '-Wextra', '-Werror'], language : 'cpp')
boost = dependency('boost')
executable('state_machine', 'example/main.cpp')

xelatex = find_program('xelatex')
find_program('pygmentize')

custom_target('article',
  output : 'article.pdf',
  input : 'article.tex',
  command : [xelatex, '-8bit', '--shell-escape', '@INPUT@'])
