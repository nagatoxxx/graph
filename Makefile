CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99 -g

SRC = main.c
NAME = main

all: libgraph $(NAME)

libgraph:
	@if [ ! -f libgraph.a ]; then \
		$(MAKE) -C lib; \
	fi

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -lm -o $(NAME) -Llib -lgraph

clean:
	@$(MAKE) -C include clean
	@rm $(NAME)
