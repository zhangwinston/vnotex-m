TEMPLATE = subdirs

CONFIG += c++17

SUBDIRS = \
    libs \
    src \
    tests

src.depends = libs
tests.depends = libs
