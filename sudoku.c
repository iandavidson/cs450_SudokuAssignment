#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>

#include <stdbool.h>


struct Wrapper {
  bool* errors;
  char** sudokuBoard;

  long _tid;
  //struct Information* _infoObject;
};


void* check(void*);
bool checkRow(struct Wrapper*, int);
bool checkCol(struct Wrapper*, int);
bool checkSub(struct Wrapper*, int);

static char* words[9] = {"left top", "center top", "right top",
                         "left middle", "center middle", "right middle",
                         "left bottom", "center bottom", "right bottom" };

int main() {
  
  struct Wrapper* infoWrapper;
  infoWrapper = malloc(sizeof(struct Wrapper));

  infoWrapper->sudokuBoard = (char**) malloc(9*sizeof(char*));
  for(int i = 0; i < 9; i++) {
    infoWrapper->sudokuBoard[i] = (char*) malloc(9*sizeof(char));
  }

  infoWrapper->errors = malloc(9*sizeof(bool));
  //w->_infoObject = malloc(sizeof(Information));

  //struct Wrapper.Information* info; 

  //struct Information* info;
  //info = malloc(sizeof(struct Information));

  //char sudokuBoard[9][9]; // 2D array, we could make this an int if we wanted to
  int row = 0;	  // row  count
  int column = 0; // column count
  int i = 0;	  // indexer	
  const size_t line_size = 300;
  char* line = malloc(line_size); // basically a buffer

  while (fgets(line, line_size, stdin) != NULL)  { // while the line is not null in the item in stdin
    while (line[i] != '\n' && line[i] != '\0') { // while theres not a new line or terminator
      if (line[i] != ' ') { 
	      //printf("Storing value: %c,in row: %d, column: %d\n", line[i], row, column); 
	      infoWrapper->sudokuBoard[row][column] = line[i]; // stores items cloumn by column
	      column++;
      }
      i++;
    }
    if (line[0] != '\n') { // because row should never be reset we want to keep an accurate row count and disregard
      row++;               // i think that this could prob get placed on line 22 tbh
    }
    column = 0; // reset column
    i = 0;
  }

  free(line);    // dont forget to free heap memory


  //by this point we have all reading in done, now we use threads to check validity of 2d array: sudokuBoard


  //bool* errors;
  //errors = malloc(27*sizeof(bool));

  for(int i = 0; i < 27; i++) {
    infoWrapper->errors[i] = false;
  }
  
  long thread_id;

  pthread_t* thread_handles;

  thread_handles = malloc(27* sizeof(pthread_t));
  //create 27 threads

  for(thread_id = 0; thread_id < 27; thread_id++){
    struct Wrapper* wrap = malloc(sizeof(struct Wrapper));
    wrap->_tid = thread_id;
    wrap->sudokuBoard = infoWrapper->sudokuBoard;
    wrap->errors = infoWrapper->errors;
    //printf("%s\n", "made thread wrapper\n");
    pthread_create(&thread_handles[thread_id], NULL, check,
       (void*) wrap);
  }

  //join threads
  for(thread_id = 0; thread_id < 9; thread_id++){
    pthread_join(thread_handles[thread_id], NULL);
  }

  
  free(thread_handles);

  //output from array at a

  //prints out the actual sudoku
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 9; x++) {
      printf("%c ", infoWrapper->sudokuBoard[y][x]);
    }
    printf("\n");
  }

  // begin error report
  int input = 0;

  for (int i = 0; i < 27; i++) {
    
    if (infoWrapper->errors[i]) {
      input = 1;
      if(i <= 8) { // range 0-8
        printf("Row %d doesn't have the required values.\n", i + 1);  
      }
      else if(i <= 17) { // range 9-17 
        printf("Column %d doesn't have the required values.\n", (i % 9) + 1);  
      } // check column
  
      else { // range 18-26 
        printf("The %s subgrid doesn't have the required values.\n", words[i % 9]);  
      } // check subgrid
    }
   
  }
  if (input == 1) {
    printf("The input is not a valid Sudoku.\n");
  } 
  else {
    printf("The input is a valid Sudoku.\n");
  }    
  
  for (int i = 0; i < 9; i++) {
    free(infoWrapper->sudokuBoard[i]);

  }
  free(infoWrapper->sudokuBoard);
  free(infoWrapper);


  return 0;
}

void* check(void* _wrap) {
  // takes a thread id which points to a thread in
  // the thread_handles array
  // returns a void star?

  /* 
    infoObject: 
      type: Information 
      members: errors, sudokuBoard
      ex: infoObject->errors
  */  

  // check thread id to determine which check we will do. 

  struct Wrapper* wrap = (struct Wrapper*) _wrap;
  long thread_id = wrap->_tid;

  if(thread_id <= 8) { // range 0-8
    if (!checkRow(wrap, thread_id % 9)) {
      wrap->errors[thread_id] = true;
    }// check row
  }
  else if(thread_id <= 17) { // range 9-17 
   if (!checkCol(wrap, thread_id % 9)) {
      wrap->errors[thread_id] = true;
    } // check column
  }
  else { // range 18-26 
    if (!checkSub(wrap, thread_id % 9))  {
      wrap->errors[thread_id] = true;
    }// check subgrid
  }
}

bool checkRow(struct Wrapper* infoObject, int row) {
  int validater[9] = {0,0,0,0,0,0,0,0,0};
  int square = 0;
  
  for (int i =0; i < 9; i++) {
    square = infoObject->sudokuBoard[row][i] - '0';
    if (square > 9 || square < 0) {
      return false;
    }

    if(validater[square] == 0) {
      validater[square] = 1;
    }
    else {
      return false;
    }
  }
  return true;
}

bool checkCol(struct Wrapper* infoObject, int column) {
  int validater[9] = {0,0,0,0,0,0,0,0,0};
  int square = 0;

  for (int i =0; i < 9; i++) {
    square = infoObject->sudokuBoard[i][column] - '0';
    if (square > 9 || square < 0) {
      return false;
    }

    if(validater[square] == 0) {
      validater[square] = 1;
    }
    else {
      return false;
    }
  }
  return true;
}

bool checkSub(struct Wrapper* infoObject, int subgrid) {
  int rowRange = (subgrid / 3) * 3;
  int colRange = (subgrid % 3) * 3;
  int validater[9] = {0,0,0,0,0,0,0,0,0};
  int square = 0;

  for (int i = rowRange; i < (rowRange + 3); i++) {
    for (int j = colRange; j < (colRange + 3); j++) {
      square = infoObject->sudokuBoard[i][j] - '0';
      if (square > 9 || square < 0) {
      return false;
      }

      if(validater[square] == 0) {
        validater[square] = 1;
      }
      else {
        return false;
      }
    }
  }
return true;
}




