objects = ls.c print.c sort.c

ls : $(objects)
	cc -Wall -Werror -Wextra -o ls $(objects)

clean :
	rm a.out ls.o print.o sort.o
