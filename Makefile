INCLUDE = include
MODULES = modules

CC = gcc

DEPS = $(INCLUDE)/mysh.h
OBJS = mysh.o command.o history.o redirection.o wildcharacter.o alias.o pipe.o background.o

EXEC = mysh

ARGS = 

CFLAGS = -Wall -Werror -g

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) -lpthread

mysh.o: $(MODULES)/mysh.c $(DEPS)
	$(CC) -c $(MODULES)/mysh.c -I $(INCLUDE)

pipe.o: $(MODULES)/pipe.c $(DEPS)
	$(CC) $(CFLAGS) -c $(MODULES)/pipe.c -I $(INCLUDE)

alias.o: $(MODULES)/alias.c $(DEPS)
	$(CC) $(CFLAGS) -c $(MODULES)/alias.c -I $(INCLUDE)

command.o: $(MODULES)/command.c $(DEPS)
	$(CC) $(CFLAGS) -c $(MODULES)/command.c -I $(INCLUDE)

history.o: $(MODULES)/history.c $(DEPS)
	$(CC) $(CFLAGS) -c $(MODULES)/history.c -I $(INCLUDE)

background.o: $(MODULES)/background.c $(DEPS)
	$(CC) $(CFLAGS) -c $(MODULES)/background.c -I $(INCLUDE)

redirection.o: $(MODULES)/redirection.c $(DEPS)
	$(CC) $(CFLAGS) -c $(MODULES)/redirection.c -I $(INCLUDE)

wildcharacter.o: $(MODULES)/wildcharacter.c $(DEPS)
	$(CC) $(CFLAGS) -c $(MODULES)/wildcharacter.c -I $(INCLUDE)

clean:
	rm -f $(OBJS) $(EXEC) *.txt

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC) $(ARGS)