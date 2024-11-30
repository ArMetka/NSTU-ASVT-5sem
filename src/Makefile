CC=gcc
FLAGS=-std=c11
LFLAGS=-lpng
MODULES_C=solution.c pngio.c
MODULES_O=$(MODULES_C:.c=.o)
SOLUTION_EXE=solution

all: clean
	make solution

$(SOLUTION_EXE): $(MODULES_O)
	$(CC) $(FLAGS) -o $@ $^ $(LFLAGS)
	make clean_o
	./$(SOLUTION_EXE)

%.o:%.c
	$(CC) $(FLAGS) -c $< -o $@ $(LFLAGS)

clean: clean_o clean_exe

clean_o:
	rm -rf *.o

clean_exe:
	rm -rf $(SOLUTION_EXE)