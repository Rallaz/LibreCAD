TEMPLATE = subdirs
TARGET = librecad
CONFIG += ordered

SUBDIRS     = \
    libraries \
    librecad \
    plugins \
    tools \
    dwgdevel

exists( custom.pro ):include( custom.pro )
