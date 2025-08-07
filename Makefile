CC = gcc
#CFLAGS = -Wall -Wextra -std=c11 -O2 -g -mavx2
CFLAGS = -Ofast -mavx2 -funroll-loops -fpeel-loops -funswitch-loops -fpredictive-commoning -ftree-vectorize	-fweb -frename-registers -flto -fomit-frame-pointer
INCLUDES = -Isrc
LDFLAGS = -lm -lX11 -lpng

SRC_DIR = src
BUILD_DIR = build
CODE_DIR = code

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

TARGET = $(BUILD_DIR)/Main

all: 
	$(CC) $(CFLAGS) $(INCLUDES) ./$(SRC_DIR)/Main.c -o ./$(TARGET) $(LDFLAGS) 

exe:
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)/*

do: clean all exe