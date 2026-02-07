# dwm version
VERSION = 6.8

# Customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# Use pkg-config for NixOS compatibility
PKGS = x11 xft xinerama fontconfig freetype2

# Xinerama, comment if you don't want it
XINERAMAFLAGS = -DXINERAMA

# includes and libs via pkg-config
INCS = `pkg-config --cflags ${PKGS}`
LIBS = `pkg-config --libs ${PKGS}`

# flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L -DVERSION=\"${VERSION}\" ${XINERAMAFLAGS}
#CFLAGS   = -g -std=c99 -pedantic -Wall -O0 ${INCS} ${CPPFLAGS}
CFLAGS   = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

# compiler and linker
CC = cc
