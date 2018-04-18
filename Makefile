CC = clang
FLAGS = -Wall -Wextra -O0

all: my_otp

clean:
	rm -f *.o my_otp

my_otp: my_otp.c cmdline.c msg_queue.c
	${CC} ${FLAGS} my_otp.c cmdline.c -o my_otp

cmdline.c: cmdline.ggo
		gengetopt --input=cmdline.ggo --include-getopt
