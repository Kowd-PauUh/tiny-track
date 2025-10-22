from setuptools import setup, find_packages
from setuptools.dist import Distribution
from pathlib import Path


class BinaryDistribution(Distribution):
    def has_ext_modules(self):
        return True


setup(
    name='tiny-track',
    version='0.0.3',
    description='tiny-track: a minimalist, MLFlow-compatible experiment tracking library',
    long_description=Path('README.md').read_text(encoding='utf-8'),
    long_description_content_type='text/markdown',
    author='Ivan Danylenko',
    author_email='kowd.pauuh@gmail.com',
    license='Apache-2.0',
    python_requires='>=3.10',
    install_requires=['pybind11'],
    packages=find_packages(include=['ttrack']),
    package_data={'ttrack': ['*.so']},
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'Intended Audience :: Education',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: Apache Software License',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3.11',
        'Programming Language :: Python :: 3.12',
        'Topic :: Scientific/Engineering',
        'Topic :: Scientific/Engineering :: Artificial Intelligence',
        'Topic :: Software Development',
        'Topic :: Software Development :: Libraries',
        'Topic :: Software Development :: Libraries :: Python Modules'
    ],
    url='https://github.com/Kowd-PauUh/tiny-track',
    distclass=BinaryDistribution,
)
