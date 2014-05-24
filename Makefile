NAME = libcontainers.so

CC = gcc
RM = rm -f
LD = gcc

SRCDIR = src/
SRC = $(SRCDIR)list.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

DEBUG = no

CFLAGS = -Wall -Wextra -Werror -ansi -pedantic
CFLAGS += -Wold-style-definition -Wcast-qual -Wlogical-op
CFLAGS += -iquote include/ -fPIC
ifeq ($(DEBUG),yes)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2 -DNDEBUG
endif

LDFLAGS = -shared

$(NAME): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) $(OBJ)

mrproper: clean
	$(RM) $(NAME)

re: mrproper all

.PHONY: all clean mrproper re
.SUFFIXES:
