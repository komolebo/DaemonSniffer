build_cli:
	gcc cli.c functions.h functions.c -o cli

build_daemon:
	gcc daemon.c functions.h functions.c sniffer.c -o daemon

start_cli:
	./cli

start_daemon:
	./daemon

all: build_cli build_daemon

show_proc:
	ps -A