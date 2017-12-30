from distutils.core import setup, Extension

module1 = Extension('digit_sum_module', sources=['digit_sum_module.c'])

setup(name='digit_sum_module_package', ext_modules=[module1])
