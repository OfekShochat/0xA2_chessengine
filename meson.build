project('0xA2_chessengine', 'cpp', 
    version : '0.0.6', 
    license : 'gnu public license v3',
    default_options : 
    [
        'cpp_std=c++17',
        'buildtype=release',
    ]
)

#subproject()

files = []
includes = []

files += [
    './src/uci.cpp',
    './src/chess/thc.cpp',
    './src/mcts/evaluate.cc',
    './src/mcts/node.cpp',
    './src/mcts/search.cpp',
    './src/mcts/Thread.cpp'
]

includes += include_directories('src')

cc = meson.get_compiler('cpp')
executable('0xA2', './src/engine.cpp', files, include_directories : includes)