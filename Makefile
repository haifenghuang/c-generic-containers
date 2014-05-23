NAME = liblinked_list.so

CC = gcc
RM = rm -f
LD = gcc

SRCDIR = src/
SRC = $(SRCDIR)linked_list.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

CFLAGS = -Wall -Wextra -Werror -ansi -pedantic -iquote include/ -fPIC -g

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
