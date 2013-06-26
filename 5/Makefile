LDFLAGS += -fopenmp
CFLAGS += -Wall -O2
CC = gcc

coarse:
	${CC} ${LDFLAGS} fifo_coarse.c -o fifo_coarse

fine:
	${CC} ${LDFLAGS} fifo_fine.c -o fifo_fine	 

.PHONY: clean
clean:
	rm -f fifo_coarse fifo_fine
