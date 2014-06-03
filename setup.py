#!/usr/bin/python3

from distutils.core import setup, Extension

spacenavmodule = Extension(
	'spacenav', 
	library_dirs = ['/usr/local/include'],
	libraries = ['spnav'],
	sources = ['src/spacenavmodule.c'])

setup(
	name = 'PySpaceNav',
	version = '0.1',
	description = 'A python extension to communicate to spacenavd from the SpaceNav project.',
	author = 'Tobias Kiertscher',
	author_email = 'dev@mastersign.de',
	url = 'http://www.mastersign.de',
	ext_modules = [ spacenavmodule ] )
