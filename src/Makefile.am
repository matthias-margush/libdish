AM_CFLAGS=-g -Wall -Werror -O3 --std=c11 -pedantic-errors -Wstrict-prototypes

bin_PROGRAMS = dish
dish_SOURCES = dish.c
dish_CFLAGS = $(AM_CFLAGS)

lib_LTLIBRARIES = libdish.la
libdish_la_SOURCES = dish.c

nobase_include_HEADERS = dish.h
