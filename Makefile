# 程式功能
#   server 與 client
#   
# 編譯需求
#   需要安裝 java, ant
#   
# 檔案結構介紹
# /android - android studio 專案
# /java - java 專案
# /java/libsockex1 - 可共用的程式
# /java/sockex1 - linux 主程式
# /linux - c 專案
# /destdir - 輸出目錄
# /destdir/bin/sockex1.jar - java 專案的 linux 主程式
#   Server
#     java -jar /destdir/bin/sockex1.jar
#   輸出大致如下   
#     Sat Aug 25 19:59:06 CST 2018 INFO Sockex1 Start server 0.0.0.0/0.0.0.0:42131
#   client, 42131 為 server 輸出中的 42131
#     java -jar /destdir/bin/sockex1.jar 127.0.0.1:42131
#   

PWD=$(abspath .)
PROJDIR?=$(PWD)
DESTDIR?=$(PWD)/destdir
BUILDDIR?=$(PWD)/build

TARGET=

all: ;
	[ -n "$(TARGET)" ] && $(MAKE) $(TARGET)

sockex1-java: sockex1-java_ ;
sockex1-java%:
	cd $(PROJDIR)/java && \
	  DESTDIR=$(DESTDIR) BUILDDIR=$(BUILDDIR)/sockex1-java ant $(patsubst _%,%,$(@:sockex1-java%=%))

TARGET+=sockex1-java
