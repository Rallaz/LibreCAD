TEMPLATE = subdirs
TARGET = librecad
CONFIG += ordered

SUBDIRS     = \
    libraries \
    librecad \
    plugins \
    tools \
    dwgdevel \
    tests

exists( custom.pro ):include( custom.pro )
