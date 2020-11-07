objects = ls.c print.c sort.c

ls : $(objects)
	cc -Wall -Werror -Wextra -o ls $(objects)

clean :
	rm $(objects)
