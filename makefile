CC = gcc
CFLAGS = -Wall -Wextra

SRCS = main.c pixels.c pictures.c filename.c lut.c
OBJS = $(SRCS:.c=.o)

TARGET = prog

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
