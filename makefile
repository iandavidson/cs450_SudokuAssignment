# Makefile for pipe test
all: sudoku.x

# $@ is make shorthand for the thing on the left side of the colon
#   (pipes.x in this case)
# $^ stands for everything on the right side of the colon (the .o files)
sudoku.x: sudoku.o 
	gcc -g -o $@ $^

clean:
	rm -f *.x *.o *~

run:
	make clean
	make all
	./sudoku.x < sudokuInput.txt
