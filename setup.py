"""When run from the command line, installs librfsim to the environment.

Run ```python setup.py build``` to build the C++ module.

Run ```python setup.py install``` to build the C++ module and install it to the environment.

Run ```python setup.py develop``` to install the package in development mode.
"""
import logging
import os

from setuptools import setup, Extension, find_packages

logging.basicConfig()
logging.getLogger().setLevel(logging.CRITICAL)
librfsim_path = os.path.join("rfsim", "lib", "librfsim.so")
if os.path.exists(librfsim_path):
	os.remove(librfsim_path)
try:
	from rfsim import __version__ as p_version
	from rfsim.installer import Installer
except ImportError:
	# Means that one of the dependencies isn't installed properly - this is fine
	pass

class CustomExtension(Extension):
	def __init__(self, name, sourcedir=''):
		Extension.__init__(self, name, sources=[])
		self.sourcedir = os.path.abspath(sourcedir)


long_description = "a very long description of your package (or read from the readme)."


setup(name='rfsim',
	  version=p_version,
	  description='A toy rabbit and fox simulation demonstrating C++ extensions with python',
	  author='Sam Thompson',
	  author_email='samuel.thompson14@imperial.ac.uk',
	  url='http://thompsonsed.co.uk',
	  long_description=long_description,
	  long_description_content_type="text/x-rst",
	  ext_modules=[CustomExtension("librfsim", os.path.join("rfsim","lib"))],
	  cmdclass=dict(build_ext=Installer),
	  license='MIT',
	  packages=find_packages(exclude=["*tests*", 'docs']),
	  classifiers=['Development Status :: 4 - Beta',
				   'License :: OSI Approved :: MIT License',
				   'Operating System :: MacOS',
				   'Operating System :: MacOS :: MacOS X',
				   'Operating System :: Microsoft :: Windows :: Windows 10',
				   'Operating System :: POSIX',
				   'Programming Language :: C++',
				   'Programming Language :: Python :: 2.7',
				   'Programming Language :: Python :: 3.6',
				   'Programming Language :: Python :: 3.7',
				   'Programming Language :: Python :: 3',
				   'Programming Language :: Python :: Implementation :: CPython',
				   'Topic :: Scientific/Engineering',
				   'Intended Audience :: Science/Research',
				   'Natural Language :: English',],
	  zip_safe=False,
	  keywords='modelling',
	  install_requires=['numpy'],
	  extras_require={
		  'scipy': ['scipy>=0.12.0'],
		  'plotting': ['matplotlib']}
      )

