#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "crossfunc.h"

//function that initializes the solution board 
void initSol(char sboard[][MAX]){
  for (int i = 0; i < MAX; i++){
    for (int j = 0; j < MAX; j++){
      sboard[i][j] = '.';
    }
  }
}

//function that initializes puzzle board 
void initPuz(char pboard[][MAX]){
  for (int i = 0; i < MAX; i++){
    for (int j = 0; j < MAX; j++){
      pboard[i][j] = '#';
    }
  }
}

//function that displays the board 
void displayBoard(char board[][MAX], FILE *out){
//print top row
  fprintf(out, "+"); 
  for (int j = 0; j < MAX; j++){
    fprintf(out, "-");
  }
  fprintf(out, "+\n");

//printf middle rows 
  for (int i = 0; i < MAX; i++){
    fprintf(out, "|");
    for (int j = 0; j < MAX; j++){
      fprintf(out, "%c",board[i][j]);
    }
   fprintf(out, "|\n");  
  } 

//print bottom row
  fprintf(out, "+"); 
  for (int j = 0; j < MAX; j++){
   fprintf(out, "-");
  }
  fprintf(out, "+\n");
}

//function that turns words into uppercase
void uppercase(Word words[MAX_WORDS], int count){
  for (int i = 0; i < count; i++){ //loop through each word 
    for (int j = 0; words[i].text[j] != '\0'; j++){ //loop through letter of each word
      words[i].text[j] = toupper(words[i].text[j]);
    }
  }   

}

//function that checks to see if input it alphanumeric
int isLetter(char input[MAX]){
  for (int i = 0; i < strlen(input); i++){
    if (isalpha(input[i]))
      continue;//continue if character is a letter
    else
      return 1; //means that there is a special character in the word 
  }
  return 0;
}

//function that sorts words in descending order 
void sort(Word words[MAX_WORDS], int count){
  for (int i = 0; i < count - 1; i++){
    for (int j = 0; j < count -1; j++){
      if (words[j].length < words[j+1].length)
      {
        Word temp = words[j]; //temporary struct variable 
        words[j] = words[j+1];
        words[j+1] = temp; 
      }
    }
  }
} 

//function that displays words 
void displayWords(Word words[MAX_WORDS], int count, FILE *out){
  for (int i = 0; i < count; i++){
    fprintf(out, "%s\n", words[i].text);
  }
}

//function that places first word onto the solution and puzzle board 
void placeFirst(Word words[MAX_WORDS], char sboard[][MAX], char pboard[][MAX], int count){
  int mid = MAX / 2; //middle of board 
  int start = 0;
  start = (MAX - words[0].length) / 2;
  for (int j = 0; j < words[0].length; j++){
    sboard[mid][start + j] = words[0].text[j];
    pboard[mid][start + j] = ' ';
  }
  //add values to struct 
  words[0].row = mid;
  words[0].col = start;
  strcpy(words[0].direction, "Across");
}

//function that places subsequent words onto solution and puzzle board
void placeNext(Word words[MAX_WORDS], Word skipped[MAX_WORDS], Word placed[MAX_WORDS], char sboard[][MAX], char pboard[][MAX],int count, int *placedCount, int *skippedCount, FILE *out){
  int r; 
  int c;
  for (int nextWord = 1; nextWord < count; nextWord++){ //looping through all subsequent words 
    bool successfullyPlaced = false; 

    for (int prevWord = 0; prevWord < nextWord; prevWord++){ //try intersecting with each previous word 
      bool valid = findIntersection(words[prevWord], &words[nextWord], sboard);
      if (valid){ //then word can be placed 
        r = words[nextWord].row;
        c = words[nextWord].col;
        for (int p = 0; p < words[nextWord].length; p++){//loops through each letter of the word 

          sboard[r][c] = words[nextWord].text[p];
          pboard[r][c] = ' ';

          if (strcmp(words[nextWord].direction, "Across") == 0)
            c++;
          else 
            r++;
        }
        successfullyPlaced = true; 
        placed[*placedCount] = words[nextWord];
        (*placedCount)++;
        break; //once word has been placed, head to subsequent word
     }
       
    }
    if (!successfullyPlaced){ //if word not placed after checking each previous word, let user know 
      fprintf(out, "Skipped: %s\n", words[nextWord].text);
      skipped[*skippedCount] = words[nextWord];
      (*skippedCount)++;
    } 
  }
}
  
//function that finds the intersection between previous words and next word on deck 
bool findIntersection(Word prev, Word *next, char sboard[][MAX]){
  //get previous words row and column 
  int prow = prev.row;
  int pcol = prev.col;
  //get next words row and column 
  int nrow;
  int ncol;
  int bound; 
  int neighbor;

  for (int i = 0; i < prev.length; i++){ //comparing next word to all letters of prev word
    for (int j = 0; j < (*next).length; j++){
      if (prev.text[i] == (*next).text[j]){ //potential match
        if (strcmp(prev.direction, "Across") == 0){ //if prev word is across, new word goes down
          nrow = prow - j;
	      ncol = pcol + i;
          strcpy((*next).direction, "Down");         
        }
        else{//prev word is down, so new word must be across
 	      nrow = prow + i;
	      ncol = pcol - j; 

          strcpy((*next).direction, "Across");
        }
        bound = bounds(nrow, ncol, *next);
        if (bound == 0)
	      continue;

	    //check to see if any neighbors 
	    neighbor = neighbors(nrow, ncol, sboard, *next);
        if (neighbor == 0)
          continue;
        
        (*next).row = nrow;
	    (*next).col = ncol;
        return true; 
      }
    }
  }
  return false; 
}

//function that checks to make sure word will be in bounds 
bool bounds(int nrow, int ncol, Word next){
//returning 0 if any condition fails (i.e. not valid) 
  if (nrow < 0 || ncol < 0)
    return 0;
  if (strcmp(next.direction, "Across") == 0){
    if (ncol + next.length > MAX)
      return false;
  }  
  else if (strcmp(next.direction, "Down") == 0){
    if (nrow + next.length > MAX)
      return false;
  }
  return true;
}


//function that checks to see if there are any neighbors 
int neighbors(int nrow, int ncol, char sboard[][MAX], Word next){
  int r;
  int c;

  if (strcmp(next.direction, "Across") == 0){
    for (int i = 0; i < next.length; i++){
      r = nrow;
      c = ncol + i; //incrementing through each letter horizontally 
       
      if (sboard[r][c] != '.' && sboard[r][c] != next.text[i]) //means there is already a letter there so cant place word 
        return 0;  
      if (i == 0 && c-1 >= 0 && sboard[r][c-1] != '.')
        return 0; //for first letter, need to make sure no letter behind it since i only check up and down 
      if (i == next.length-1 && c+1 < MAX && sboard[r][c+1] != '.')
        return 0;
      //check up and down 
      if (sboard[r][c] == next.text[i])
        continue; //skip intersection
      if (r - 1 >= 0 && sboard[r-1][c] != '.')
        return 0;
      if (r + 1 < MAX && sboard[r+1][c] != '.')
        return 0;
    }
  }
  if (strcmp(next.direction, "Down") == 0){
    for (int i = 0; i < next.length; i++){
      r = nrow + i; 
      c = ncol;
      
      if (sboard[r][c] != '.' && sboard[r][c] != next.text[i])
        return 0;  
      if (i == 0 && r-1 >= 0 && sboard[r-1][c] != '.')
        return 0; 
      if (i == next.length-1 && r+1 != MAX && sboard[r+1][c] != '.')
        return 0;
      if (sboard[r][c] == next.text[i])
        continue; 
      //check left and right 
      if (c - 1 >= 0 && sboard[r][c-1] != '.')
        return 0;
      if (c + 1 <= MAX && sboard[r][c+1] != '.')
        return 0;
    }
  }   
   
   return 1;
}

//function that shuffles words 
//using fisher-yates method 
void shuffle(Word words[MAX_WORDS], int count){
  char temp;
  int random;  
  int len; 
  for (int i = 0; i < count; i++){
    strcpy(words[i].anagram, words[i].text);
    len = words[i].length;
    for (int j = len - 1; j > 0; j--){
      random = rand() % (j+1); //will always give number 0 through count
      temp = words[i].anagram[j]; //one character at a time
      words[i].anagram[j] = words[i].anagram[random];
      words[i].anagram[random] = temp;
    }
  }
}

//function that displays clues for words 
void clues(Word words[MAX_WORDS], int count, FILE *out){
  fprintf(out, "Clues: \n");
  fprintf(out, "Location | Direction | Anagram\n");
  for (int i = 0; i < count; i++){
    fprintf(out, "%4d, %2d | %9s | %-17s\n", 
          words[i].row, words[i].col,
	  words[i].direction, 
	  words[i].anagram); 
  }
}

//function that checks words that were previously skipped when placing 
void retrySkipped(Word placed[MAX_WORDS], Word skipped[MAX_WORDS], Word words[MAX_WORDS], char sboard[][MAX], char pboard[][MAX], int placedCount, int skippedCount, int wordCount, FILE *out){
  int r; 
  int c; 

  for (int i = 0; i < skippedCount; i++){
    bool successfullyPlaced = false;
    for (int j = 0; j < placedCount; j++){
      bool valid = findIntersection(placed[j], &skipped[i], sboard);
      if (valid) {
        r = skipped[i].row;
        c = skipped[i].col;
        for (int p = 0; p < skipped[skippedCount].length; p++){//loops through each letter of the word 

          sboard[r][c] = skipped[i].text[p];
          pboard[r][c] = ' ';

          if (strcmp(skipped[skippedCount].direction, "Across") == 0)
            c++;
          else 
            r++;
        }
        successfullyPlaced = true; 
        fprintf(out, "%s\n was able to be placed after another iteration.", skipped[i].text);

        break; //once word has been placed, head to subsequent word
     }
   }  
   if (!successfullyPlaced){ //if word not placed after checking each previous word, let user know 
     fprintf(out, "%s was not able to be placed after another iteration.\n", skipped[i].text);
     for (int k = 0; k < wordCount; k++){
       if(strcmp(skipped[i].text, words[k].text) == 0){
         words[k].row = -1;
         words[k].col = -1;
         strcpy(words[k].direction, "NA");
       }
     }
   }
 }
}













