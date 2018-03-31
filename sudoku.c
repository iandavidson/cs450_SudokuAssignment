#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

int main() {
  char sudokuBoard[9][9]; // 2D array, we could make this an int if we wanted to
  int row = 0;	  // row  count
  int column = 0; // column count
  int i = 0;	  // indexer	
  const size_t line_size = 300;
  char* line = malloc(line_size); // basically a buffer
  while (fgets(line, line_size, stdin) != NULL)  { // while the line is not null in the item in stdin
    while (line[i] != '\n' && line[i] != '\0') { // while theres not a new line or terminator
      if (line[i] != ' ') { 
	printf("Storing value: %c,in row: %d, column: %d\n", line[i], row, column); 
	sudokuBoard[row][column] = line[i]; // stores items cloumn by column
	column++;
      }
      i++;
    }
    if (line[0] != '\n') { // because row should never be reset we want to keep an accurate row count and disregard
      row++;               // i think that this could prob get placed on line 22 tbh
    }
    column = 0; // reset column
    printf("\n");
    i = 0;
  }

  free(line);    // dont forget to free heap memory

  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      printf("%c ", sudokuBoard[y][x]);
    }
    printf("\n");
  }                  
  return 0;
}
