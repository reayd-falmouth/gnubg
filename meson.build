project(
    'gnubg',
    'c',
    version : 'modern',
    default_options : [
        'warning_level=2',
        'c_std=c99',
    ]
)

# ---------------------------------------------------------------------------
# Modules & Dependencies
# ---------------------------------------------------------------------------
gnome = import('gnome')

glib_dep   = dependency('glib-2.0', version : '>=2.0')
gtk_dep    = dependency('gtk+-3.0')
python_dep = dependency('python3')

# ---------------------------------------------------------------------------
# Global compile definitions
# ---------------------------------------------------------------------------
add_project_arguments('-DGNUBG_NO_GRESOURCE', language : 'c')
add_project_arguments('-include', 'config.h', language : 'c')

# ---------------------------------------------------------------------------
# Configuration (Autotools Parity)
# ---------------------------------------------------------------------------
conf = configuration_data()
conf.set('HAVE_CONFIG_H', 1)
conf.set_quoted('PACKAGE', 'gnubg')
conf.set_quoted('LOCALEDIR', get_option('prefix') / 'share' / 'locale')
conf.set_quoted('AC_DATADIR', get_option('prefix') / 'share')
conf.set_quoted('AC_PKGDATADIR', get_option('prefix') / 'share' / 'gnubg')
conf.set_quoted('AC_DOCDIR', get_option('prefix') / 'share' / 'doc' / 'gnubg')

configure_file(output : 'config.h', configuration : conf)

# ---------------------------------------------------------------------------
# Resources (Replicating Makefile.am)
# ---------------------------------------------------------------------------
# Note: You must create 'pixmaps/gnubg.gresource.xml' (see below)
resources = gnome.compile_resources(
    'gnubg-stock-resources',
    'pixmaps/stock-icons.xml',
    source_dir : 'pixmaps',
    c_name : 'gnubg_stock',
    export : true,
)

# ---------------------------------------------------------------------------
# Sources & Includes
# ---------------------------------------------------------------------------
inc = include_directories('.', 'lib', 'pixmaps')

generated_sources = files(
    'non-src/copying.c', 'non-src/credits.c',
    'non-src/external_l.c', 'non-src/external_y.c',
    'non-src/sgf_l.c', 'non-src/sgf_y.c',
)

root_sources = files(
    'analysis.c', 'bearoff.c', 'bearoffdump.c', 'bearoffgammon.c',
    'boardpos.c', 'dbprovider.c', 'dice.c', 'drawboard.c',
    'eval.c', 'evallock.c', 'export.c', 'external.c',
    'file.c', 'format.c', 'formatgs.c', 'glib-ext.c',
    'gnubg.c', 'gnubgmodule.c', 'gnubgstock.c', 'html.c',
    'htmlimages.c', 'import.c', 'latex.c', 'matchequity.c',
    'matchid.c', 'mec.c', 'mtsupport.c', 'multithread.c',
    'openurl.c', 'osr.c', 'output.c', 'play.c',
    'positionid.c', 'progress.c', 'randomorg.c', 'relational.c',
    'render.c', 'renderprefs.c', 'rollout.c', 'set.c',
    'sgf.c', 'show.c', 'simpleboard.c', 'sound.c',
    'speed.c', 'text.c', 'timer.c', 'util.c',
)

lib_sources = files(
    'lib/SFMT.c', 'lib/cache.c', 'lib/isaac.c',
    'lib/list.c', 'lib/md5.c', 'lib/neuralnet.c',
)

stock_images = files(
    'pixmaps/16x16/actions/cancel_16.png',
    'pixmaps/16x16/actions/double_16.png',
    'pixmaps/16x16/actions/go_next_16.png',
    'pixmaps/16x16/actions/go_next_cmarked_16.png',
    'pixmaps/16x16/actions/go_next_game_16.png',
    'pixmaps/16x16/actions/go_next_marked_16.png',
    'pixmaps/16x16/actions/go_prev_16.png',
    'pixmaps/16x16/actions/go_prev_cmarked_16.png',
    'pixmaps/16x16/actions/go_prev_game_16.png',
    'pixmaps/16x16/actions/go_prev_marked_16.png',
)

stock_icons_xml = custom_target(
    'stock-icons.xml',
    output : 'stock-icons.xml',
    input  : stock_images,
    command : [
        'sh', '-c',
        '''
    echo '<?xml version="1.0" encoding="UTF-8"?>' > @OUTPUT@
    echo '<gresources>' >> @OUTPUT@
    echo ' <gresource prefix="/org/gnubg">' >> @OUTPUT@
    for img in @INPUT@; do
      echo "  <file>$img</file>" >> @OUTPUT@
    done
    echo ' </gresource>' >> @OUTPUT@
    echo '</gresources>' >> @OUTPUT@
    '''
    ],
    build_by_default : true
)

gnubg_stock_resources = custom_target(
    'gnubg-stock-resources',
    input  : stock_icons_xml,
    output : ['gnubg-stock-resources.c', 'gnubg-stock-resources.h'],
    command : [
        glib_compile_resources,
        '--generate',
        '--manual-register',
        '--c-name=gnubg_stock',
        '--sourcedir=@SOURCE_ROOT@',
        '--target=@OUTPUT0@',
        '@INPUT@'
    ],
    build_by_default : true
)

# ---------------------------------------------------------------------------
# Libraries & Executable
# ---------------------------------------------------------------------------
neuralnetsse_obj = static_library(
    'neuralnetsse',
    files('lib/neuralnetsse.c'),
    c_args : ['-mavx'],
    include_directories : inc,
    dependencies : glib_dep, # Required for glib.h
)

sources = gnubg_stock_resources[0] + root_sources + lib_sources + generated_sources + resources

executable(
    'gnubg',
    sources,
    include_directories : inc,
    dependencies : [glib_dep, gtk_dep, python_dep],
    link_with : neuralnetsse_obj,
)