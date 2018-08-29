#------------------------------------
# 程式功能
#   server 與 client
#   
# 編譯需求
#   需要安裝 java, ant
#   
# 檔案結構介紹
# /android - android studio 專案
# /java - java 專案
# /java/libsockex - 可共用的程式
# /java/sockex - linux 主程式
# /linux - c 專案
# /destdir - 輸出目錄
# /destdir/bin/sockex.jar - java 專案的 linux 主程式
#   Server
#     java -jar /destdir/bin/sockex.jar
#   輸出大致如下   
#     Sat Aug 25 19:59:06 CST 2018 INFO sockex Start server 0.0.0.0/0.0.0.0:42131
#   client, 42131 為 server 輸出中的 42131
#     java -jar /destdir/bin/sockex.jar 127.0.0.1:42131
#   

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
sockex-java: sockex-java_ ;
sockex-java%:
	cd $(PROJDIR)/java && \
	  DESTDIR=$(DESTDIR) BUILDDIR=$(BUILDDIR)/sockex-java ant $(patsubst _%,%,$(@:sockex-java%=%))

# TARGET+=sockex-java

#------------------------------------
#
sockex-c_MAKE=$(MAKE) PROJDIR=$(PROJDIR) DESTDIR=$(DESTDIR) \
  BUILDDIR=$(BUILDDIR) -C $(PROJDIR)/c
sockex-c: sockex-c_ ;
sockex-c%:
	$(sockex-c_MAKE) $(patsubst _%,%,$(@:sockex-c%=%))

TARGET+=sockex-c
