##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## compile les fichiers et cree un executable
##

NAME := r-type_client

SRC_DIR := src

SRC := $(shell find $(SRC_DIR) -name "*.cpp")

OBJ := $(SRC:.cpp=.o)

CC := gcc
CFLAGS := -Wall -Wextra -O3
CPPFLAGS := -I$(SRC_DIR) -Iinclude

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(CPPFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

all: $(NAME)

debug: CFLAGS += -g3
debug: all

clean:
	rm -f $(OBJ)
	rm -f \#*\#

fclean: clean
	rm -f $(NAME)
	rm -f unit_tests*
	rm -f *.gc*

re: fclean all

TEST_SRC := $(filter-out src/main.cpp,$(SRC))

unit_tests: CFLAGS += -I.
unit_tests:
	$(CC) $(CFLAGS) $(CPPFLAGS) -o unit_tests $(TEST_SRC) tests/unit/*.c
	-lcriterion --coverage

tests_run: unit_tests
	./unit_tests
	gcovr --exclude tests/

.PHONY: all clean fclean re debug unit_tests tests_run
