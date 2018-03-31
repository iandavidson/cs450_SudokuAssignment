#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>

int main() {
  char sudokuBoard[9][9];
  int row = 0;
  int column = 0;
  int i = 0;
  const size_t line_size = 300;
  char* line = malloc(line_size);
  while (fgets(line, line_size, stdin) != NULL)  {
    while (line[i] != '\n' && line[i] != '\0') {
      if (line[i] != ' ') {
	printf("Storing value: %c,in row: %d, column: %d\n", line[i], row, column); 
	sudokuBoard[row][column] = line[i];
	column++;
      }
      i++;
    }
    if (line[0] != '\n') {
      row++;
    }
    column = 0;
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
