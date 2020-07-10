from setuptools import setup
from distutils.core import setup, Extension
from Cython.Build import cythonize

extensions = [
    Extension(
        name="socketInterface",
        sources=["python/*.pyx"], # cython sources
        include_dirs=["include"],
        libraries=["CPPSockets"], # this is the c++ library name!
        # library_dirs=["build"],
        extra_compile_args=['-std=c++11'],
        language="c++"
    )

]


setup(
    name="pySocketInterface", # name of package in 'pip freeze'
    version='0.0.1',
    # packages=['myclass'], # not exactly sure what this does -> doesnt seem to work
    # package_dir = {"":"build",
    #                "":"python"}, # site_packages, having "" in this list
    ext_modules=cythonize(extensions)
)