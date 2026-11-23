
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc

SYSROOT = $(HOME)/toolchains/armhf-glibc219/rootfs
TARGET = SmartSortMonitor
SRC = main.c

CFLAGS = -Wall -O0 -g --sysroot=$(SYSROOT)
CFLAGS += -isystem $(SYSROOT)/usr/include/arm-linux-gnueabihf
CFLAGS += -isystem $(SYSROOT)/usr/include

LDFLAGS = --sysroot=$(SYSROOT)
LDFLAGS += -B$(SYSROOT)/usr/lib/arm-linux-gnueabihf/
LDFLAGS += -L$(SYSROOT)/usr/lib/arm-linux-gnueabihf
LDFLAGS += -L$(SYSROOT)/lib/arm-linux-gnueabihf
LDFLAGS += -Wl,--dynamic-linker=/lib/ld-linux-armhf.so.3

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
