project(
    'gnubg',
    'c',
    version: '1.08.003',
    default_options: [
        'c_std=c11',
        'warning_level=2',
        'buildtype=release'
    ]
)

cc = meson.get_compiler('c')

# -------------------------
# Build options
# -------------------------
option_readline = get_option('readline')
option_nls       = get_option('nls')

# -------------------------
# Dependencies
# -------------------------
glib = dependency('glib-2.0', version: '>=2.22')

readline = dependency(
    'readline',
    required: option_readline
)

# -------------------------
# Configuration header
# -------------------------
conf = configuration_data()

conf.set('HAVE_GLIB', 1)
conf.set('HAVE_READLINE', readline.found())
conf.set('ENABLE_NLS', option_nls)
conf.set('PACKAGE', '"gnubg"')
conf.set('VERSION', '"@0@"'.format(meson.project_version()))

configure_file(
    output: 'config.h',
    configuration: conf
)

# -------------------------
# Subdirectories
# -------------------------
subdir('lib')
subdir('src')
