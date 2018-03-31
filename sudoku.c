#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>

//think we got all the includes neccessary to use threads
int  *a = NULL;



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
	//printf("Storing value: %c,in row: %d, column: %d\n", line[i], row, column); 
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


  //by this point we have all reading in done, now we use threads to check validity of 2d array: sudokuBoard



  long thread_id;

  pthread_t* thread_handles;
  //allocate ErrorReport array on void *a.
    //worst case: 27 spaces
        //worst case error length in chars: 60
  a = malloc(27 * 60 * sizeof(char));

  thread_handles = malloc(9* sizeof(pthread_t));
  //create 9 threads

  for(thread_id = 0; thread_id < 9; thread_id++){
    pthread_create(&thread_handles[thread_id], NULL, checkRow, checkCol, checkSub,
       (void*) thread_id);
  }

  //for(threads) { checkRows(thisThread->id, grid, &a)}
  //for(threads) { checkCols(thisThread->id, grid, &a)}
  //for(threads) { checkSubgrids(thisThread->id, grid, &a)}






  //join threads
  for(thread_id = 0; thread_id < 9; thread_id++){
    pthread_join(thread_handles[thread_id], NULL);
  }

  
  free(thread_handles);

  //output from array at a


  free(a);

  //prints out the actual sudoku
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      printf("%c ", sudokuBoard[y][x]);
    }
    printf("\n");
  }                  
  return 0;
}
