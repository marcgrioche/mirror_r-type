##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## compile les fichiers et cree un executable
##


all: r-type_server r-type_client

r-type_server:
	$(MAKE) -C server --no-print-directory
	@cp server/r-type_server .

r-type_client:
	$(MAKE) -C client --no-print-directory
	@cp client/r-type_client .

debug:
	$(MAKE) -C server debug --no-print-directory
	@cp server/r-type_server .
	$(MAKE) -C client debug --no-print-directory
	@cp client/r-type_client .

clean:
	$(MAKE) -C server clean --no-print-directory
	$(MAKE) -C client clean --no-print-directory

fclean:
	$(MAKE) -C server fclean --no-print-directory
	$(MAKE) -C client fclean --no-print-directory
	@rm -f r-type_client r-type_server

re: fclean all

tests_run:
	$(MAKE) -C server tests_run --no-print-directory
	$(MAKE) -C client tests_run --no-print-directory

.PHONY: all zappy_server r-type_client clean fclean re debug tests_run
