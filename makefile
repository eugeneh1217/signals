CFLAGS=-Wall -lpthread
CC=gcc

.PHONY: clean


test: devices.c test.c
	$(CC) $(CFLAGS) $^


signals: signals.c devices.c
	$(CC) $(CFLAGS) $^


clean: a.out
	rm a.out
