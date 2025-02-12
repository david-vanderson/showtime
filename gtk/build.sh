#!/bin/sh
g++ -g -Wall gtk.c `pkg-config --cflags --libs gtk4` -o gtk_c && GDK_SCALE=2 ./gtk_c
