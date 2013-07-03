LDFLAGS += -fopenmp -ggdb
#CFLAGS += -Wall -O2
CC = /usr/local/Cellar/gcc48/4.8.1/bin/gcc-4.8 

one_lock:
	${CC} ${LDFLAGS} one_lock.c -o one_lock

two_locks:
	${CC} ${LDFLAGS} two_locks.c -o two_locks

two_critical_sections:
	${CC} ${LDFLAGS} two_critical_sections.c -o two_critical_sections 

lock_free:
	${CC} ${LDFLAGS} lock_free.c -o lock_free

lock_free_and_linearizable:
	${CC} ${LDFLAGS} lock_free_and_linearizable.c -o lock_free_and_linearizable

.PHONY: clean
clean:
	rm -f one_lock two_locks two_critical_sections lock_free lock_free_and_linearizable
	rm -rf *\.dSYM
