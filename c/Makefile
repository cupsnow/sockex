#------------------------------------
#
PWD=$(abspath .)
PROJDIR?=$(PWD)
DESTDIR?=$(PWD)/destdir
BUILDDIR?=$(PWD)/build

#------------------------------------
#
TARGET=

all: ;
	[ -n "$(TARGET)" ] && $(MAKE) $(TARGET)

#------------------------------------
#

$(DESTDIR)/bin/sockex: src/*.c
	mkdir -p $(dir $@)
	gcc -o $@ $^

sockex: $(DESTDIR)/bin/sockex ;

TARGET+=sockex
