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
	[ -n "$(strip $(TARGET))" ] && $(MAKE) $(TARGET)

#------------------------------------
#
sockex-java_ANT=cd $(PROJDIR)/java && \
  DESTDIR=$(DESTDIR) BUILDDIR=$(BUILDDIR)/sockex-java ant
sockex-java: sockex-java_ ;
sockex-java%:
	$(sockex-java_ANT) $(patsubst _%,%,$(@:sockex-java%=%))

#TARGET+=sockex-java

#------------------------------------
#
sockex-c_MAKE=$(MAKE) PROJDIR=$(PROJDIR) DESTDIR=$(DESTDIR) \
  BUILDDIR=$(BUILDDIR) -C $(PROJDIR)/c
sockex-c: sockex-c_ ;
sockex-c%:
	$(sockex-c_MAKE) $(patsubst _%,%,$(@:sockex-c%=%))

TARGET+=sockex-c

#------------------------------------
#
