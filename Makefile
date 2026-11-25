
# ==========================================
# 1. ARM cross compiler
# ==========================================

CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc


# ==========================================
# 2. Target sysroot
# ==========================================

SYSROOT = $(HOME)/toolchains/armhf-glibc219/rootfs


# ==========================================
# 3. Project directories
# ==========================================

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

APP_NAME = SmartSortMonitor
TARGET = $(BIN_DIR)/$(APP_NAME)


# ==========================================
# 4. Source files
# ==========================================

SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/system_info.c \
       $(SRC_DIR)/logger.c \
       $(SRC_DIR)/config.c

OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)


# ==========================================
# 5. Compiler flags
# ==========================================

CFLAGS = -Wall -Wextra -O0 -g -std=gnu11
CFLAGS += -I$(INC_DIR)
CFLAGS += --sysroot=$(SYSROOT)
CFLAGS += -isystem $(SYSROOT)/usr/include/arm-linux-gnueabihf
CFLAGS += -isystem $(SYSROOT)/usr/include
CFLAGS += -MMD -MP


# ==========================================
# 6. Linker flags
# ==========================================

LDFLAGS = --sysroot=$(SYSROOT)
LDFLAGS += -B$(SYSROOT)/usr/lib/arm-linux-gnueabihf/
LDFLAGS += -L$(SYSROOT)/usr/lib/arm-linux-gnueabihf
LDFLAGS += -L$(SYSROOT)/lib/arm-linux-gnueabihf
LDFLAGS += -Wl,--dynamic-linker=/lib/ld-linux-armhf.so.3


# ==========================================
# 7. AX7020 connection configuration
# ==========================================

BOARD_IP = 192.168.10.2
BOARD_USER = root
BOARD_DIR = /root/SmartSortMonitor

SSH = ssh
SCP = scp

CONFIG_FILE = config/smartsort.conf


# ==========================================
# 8. Default target
# ==========================================

.PHONY: all

all: $(TARGET)


# ==========================================
# 9. Link object files
# ==========================================

$(TARGET): $(OBJS)
	mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@


# ==========================================
# 10. Compile source files
# ==========================================

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


# ==========================================
# 11. First-time initialization
# ==========================================

.PHONY: setup

setup: $(CONFIG_FILE)
	$(SSH) $(BOARD_USER)@$(BOARD_IP) "mkdir -p $(BOARD_DIR)/config $(BOARD_DIR)/logs"
	$(SCP) $(CONFIG_FILE) $(BOARD_USER)@$(BOARD_IP):$(BOARD_DIR)/config/smartsort.conf


# ==========================================
# 12. Deploy executable only
# ==========================================

.PHONY: deploy

deploy: all
	$(SCP) $(TARGET) $(BOARD_USER)@$(BOARD_IP):$(BOARD_DIR)/$(APP_NAME)


# ==========================================
# 13. Deploy configuration only
# ==========================================

.PHONY: deploy-config

deploy-config: $(CONFIG_FILE)
	$(SCP) $(CONFIG_FILE) $(BOARD_USER)@$(BOARD_IP):$(BOARD_DIR)/config/smartsort.conf


# ==========================================
# 14. Deploy and run on AX7020
# ==========================================

.PHONY: run

run: deploy
	$(SSH) -t $(BOARD_USER)@$(BOARD_IP) "cd $(BOARD_DIR) && chmod +x $(APP_NAME) && ./$(APP_NAME)"


# ==========================================
# 15. Connect to AX7020
# ==========================================

.PHONY: connect

connect:
	$(SSH) $(BOARD_USER)@$(BOARD_IP)


# ==========================================
# 16. Clean local generated files
# ==========================================

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)


# ==========================================
# 17. Include automatically generated dependencies
# ==========================================

-include $(DEPS)
