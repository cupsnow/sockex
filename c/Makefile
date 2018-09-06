#------------------------------------
#
PWD=$(abspath .)
PROJDIR?=$(PWD)
DESTDIR?=$(PWD)/destdir
BUILDDIR?=$(PWD)/build

MKDIR=mkdir -p
CP=cp -dp

CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)gcc
STRIP=$(CROSS_COMPILE)strip -g -p -R ".comment" -R ".note" -R ".note.ABI-tag"
CFLAGS?=-g
LDFLAGS?=
#------------------------------------
#
TARGET=

all: ;
	[ -n "$(strip $(TARGET))" ] && $(MAKE) $(TARGET)

#------------------------------------
#
sockex_BUILDDIR=$(BUILDDIR)/sockex-c
sockex_OBJ=$(patsubst $(PWD)/%.c,$(sockex_BUILDDIR)/%.o,$(wildcard $(PWD)/src/*.c))

sockex: $(sockex_BUILDDIR)/bin/sockex ;

$(sockex_BUILDDIR)/bin/sockex: $(sockex_OBJ)
	[ -e `dirname $@` ] || $(MKDIR) `dirname $@`
	$(LD) -o $@ $^ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -pthread

$(sockex_OBJ): $(sockex_BUILDDIR)/%.o : $(PWD)/%.c
	[ -e `dirname $@` ] || $(MKDIR) `dirname $@`
	$(CC) -c -o $@ $< $(CFLAGS) $(CPPFLAGS)
	$(CC) -E -MM -MF $@.d -MT $@ $< $(CFLAGS) $(CPPFLAGS)

sockex_install: $(sockex_BUILDDIR)/bin/sockex
	[ -e $(DESTDIR)/bin ] || $(MKDIR) $(DESTDIR)/bin
	for i in sockex; do \
	  $(CP) $(sockex_BUILDDIR)/bin/$$i $(DESTDIR)/bin/; \
	  $(STRIP) $(DESTDIR)/bin/$$i; \
	done

TARGET+=sockex

#------------------------------------
#
install clean distclean:
	[ -n "$(strip $(TARGET))" ] && $(MAKE) $(addsuffix _$@,$(TARGET))
