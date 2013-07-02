LDFLAGS += -fopenmp -ggdb
#CFLAGS += -Wall -O2
CC = gcc

one_lock:
	${CC} ${LDFLAGS} one_lock.c -o one_lock

two_locks:
	${CC} ${LDFLAGS} two_locks.c -o two_locks

two_critical_sections:
	${CC} ${LDFLAGS} two_critical_sections.c -o two_critical_sections 

atomic:
	${CC} ${LDFLAGS} atomic.c -o atomic 

.PHONY: clean
clean:
	rm -f one_lock two_locks two_critical_sections atomic
	rm -rf *\.dSYM
