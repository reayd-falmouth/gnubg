project(
    'gnubg',
    'c',
    version : 'modern',
    default_options : [
        'warning_level=2',
        'c_std=c99'
    ]
)

cc = meson.get_compiler('c')

inc = include_directories(
    '.',
    'lib',
)

sources = files(
    'analysis.c',
    'bearoff.c',
    'bearoffgammon.c',
    'boardpos.c',
    'dice.c',
    'eval.c',
    'format.c',
    'getopt.c',
    'getopt1.c',
    'gnubg.c',
    'matchid.c',
    'positionid.c',
    'record.c',
    'rollout.c',
    'timer.c',
    'lib/SFMT.c',
    'lib/cache.c',
    'lib/isaac.c',
    'lib/list.c',
    'lib/md5.c',
    'lib/neuralnet.c',
    'lib/neuralnetsse.c',
)

exe = executable(
    'gnubg',
    sources,
    include_directories : inc,
    install : true,
)
