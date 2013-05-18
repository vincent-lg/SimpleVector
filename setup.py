from distutils.core import setup, Extension
setup(name="vector", version="0.0",
      ext_modules=[Extension("vector", ["src/vectormodule.c"])])
