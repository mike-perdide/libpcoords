#!/usr/bin/env python

from distutils.core import setup, Extension

modulepcv = Extension('picviz',
                    define_macros = [('MAJOR_VERSION', '0'),
                                     ('MINOR_VERSION', '6')],
                    include_dirs = ['../../include/'],
                    libraries = ['picviz','pcre'],
                    library_dirs = ['../../../build/src/'],
                    sources = ['py-picviz.c', 'py-picviz_util.c', 'py-picviz_image.c', 'py-picviz_axes.c', 'py-picviz_lines.c', 'py-picviz_pgdl.c'])

setup (name = 'Picviz',
       version = '0.6',
       description = 'Picviz python bindings',
       author = 'Sebastien Tricaud',
       author_email = 'sebastien@honeynet.org',
       url = 'http://www.wallinfire.net/picviz',
       long_description = '''
High level python binding to acces picviz data, parse, render and transform.
''',
       ext_modules = [modulepcv])

