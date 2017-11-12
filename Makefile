
CC=gcc

CFLAGS=-Wall -Wextra -std=c99
CFLAGS_RELEASE=-O2
CFLAGS_DEBUG=-DDEBUG -g

TARGET=stdin2list

SRCS=\
 main.c\
 list.c\

OBJS=$(SRCS:.c=.o)

################################################################################

.PHONY: release
release: CFLAGS+=$(CFLAGS_RELEASE)
release: all

.PHONY: debug
debug: CFLAGS+=$(CFLAGS_DEBUG)
debug: all

.PHONY: all
all: $(TARGET)

.PHONY: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)

