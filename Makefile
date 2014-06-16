NAME = libcontainers.so
TEST = test.out

CC = gcc
RM = rm -f
LD = gcc

SRCDIR = src/
SRC = $(SRCDIR)list.c \
	  $(SRCDIR)stack.c \
	  $(SRCDIR)listeners.c \
	  $(SRCDIR)array.c \
	  $(SRCDIR)map.c \
	  $(SRCDIR)ring_buffer.c

SRCTEST = tests/test.c
OBJTEST = $(SRCTEST:.c=.o)

OBJ = $(SRC:.c=.o)

DEBUG = no

CFLAGS = -Wall -Wextra -Werror -ansi -pedantic
CFLAGS += -Wold-style-definition -Wcast-qual -Wlogical-op
CFLAGS += -Wno-variadic-macros
CFLAGS += -iquote include/ -fPIC
ifeq ($(DEBUG),yes)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2 -DNDEBUG
endif

LDFLAGS = -shared

LDFLAGSTEST = -L. -lcontainers

all: $(NAME)

test: $(TEST)

fulltest: re test

$(TEST): $(OBJTEST)
	$(RM) $(TEST)
	$(CC) -o $@ $^ $(LDFLAGSTEST)
	LD_LIBRARY_PATH=. ./$(TEST)

$(NAME): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) $(OBJ)

mrproper: clean
	$(RM) $(NAME)

re: mrproper all

.PHONY: all clean mrproper re test $(TEST)
.SUFFIXES:
