CC = clang
FLAGS = -Wall -Wextra -O0

all: mq_test

clean:
	rm -f *.o mq_test

mq_test: mq_test.c msg_queue.c
	${CC} ${FLAGS} mq_test.c msg_queue.c -o mq_test
