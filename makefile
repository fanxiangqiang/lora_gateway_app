PKG_NAME:=demo_lora_endian
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_MAINTAINER:=<1562573993@qq.com>
PKG_LICENSE:=GPL-2.0
TARGET := main
#LDFLAGS := -L/home/fanxiangqiang/src/mt7628/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/lib/libcurl.so

CC = mipsel-openwrt-linux-gcc 
#CFLAGS=-I/home/fanxiangqiang/src/mt7628/staging_dir/target-mipsel_24kec+dsp_uClibc-0.9.33.2/usr/include

$(TARGET) : main.o
	$(CC)  libcurl.so.4.3.0  -o lorademo  *.c  -lm 

 clean:
	rm lorademo  *.o -rf 

