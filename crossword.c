//Peter Bae
//Fundamentals of Computing 
//Lab 8 : crossword 
//Program that creates crossword game 

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "crossfunc.h"

int main(int argc, char *argv[]){
  char sboard[MAX][MAX];
  char pboard[MAX][MAX];
  Word words[MAX_WORDS]; //20 words structs within word
  Word skipped[MAX_WORDS]; //struct array that will be filled with skipped words
  Word placed[MAX_WORDS];
  int wordCount = 0;
  int placedCount = 0;
  int skippedCount = 0;
  char input[MAX];
  char temp[MAX];
  FILE *fp; 
  FILE *in;
  FILE *out;

  initSol(sboard);
  initPuz(pboard);

  if (argc == 1) //enter interactive mode since arg count is just one word
  {
    printf("Please enter the words for the crossword follwed by a '.'\n"); 
    while(1){
      scanf("%s", input);
      if ((strcmp(input, ".") == 0) || wordCount >= MAX_WORDS)
        break;
      else if (strlen(input) > MAX_LEN - 1){
        printf("%s is too long\n", input);
        // clear the rest of the line
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
          continue;
      }

      else if (strlen(input) <= 1){
        printf("%s is too short\n", input);
        continue;
      }
      else if (isLetter(input) == 1){
        printf("%s is invalid. Contains special character\n", input);
        continue;
      }
      else{ 
        strcpy(words[wordCount].text, input);
        words[wordCount].length = strlen(input);
      }
      wordCount++; 
    }
    uppercase(words, wordCount);
    sort(words, wordCount);
    displayWords(words, wordCount, stdout);
    placeFirst(words, sboard, pboard, wordCount);
    placeNext(words, skipped, placed, sboard, pboard, wordCount, &placedCount, &skippedCount, stdout);
    retrySkipped(placed, skipped, words, sboard, pboard, placedCount, skippedCount, wordCount, stdout);
    displayBoard(sboard, stdout);
    displayBoard(pboard, stdout);
    shuffle(words, wordCount);
    clues(words, wordCount, stdout);
  }
  else if(argc == 2){
    fp = fopen(argv[1],"r");
    while (fp == NULL){
      printf("Cannot open file\n");
      scanf("%s", input);
      fp = fopen(input, "r");
    }
    while (fscanf(fp,"%s",input) == 1){
      if (strcmp(input, ".") == 0 || wordCount >= MAX_WORDS)
        break;
      if (strlen(input) > MAX_LEN - 1){
        printf("%s is too long\n", input);      
        // clear the rest of the line
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
          continue;
      }
      else{
        strcpy(words[wordCount].text, input);
        words[wordCount].length = strlen(input); 
      }
      wordCount++;  
    }
    uppercase(words, wordCount);
    sort(words, wordCount);
    displayWords(words, wordCount, stdout);
    placeFirst(words, sboard, pboard, wordCount);
    placeNext(words, skipped, placed, sboard, pboard, wordCount, &placedCount, &skippedCount, stdout);
    retrySkipped(placed, skipped, words, sboard, pboard, placedCount, skippedCount, wordCount, stdout);
    displayBoard(sboard, stdout); 
    displayBoard(pboard, stdout);
    shuffle(words, wordCount);
    clues(words, wordCount, stdout);
  }
  else if (argc == 3){
    in = fopen(argv[1],"r");
    strcpy(temp, argv[2]);

    while (in == NULL){
      printf("Cannot open file. Enter a new one: ");
      scanf("%s", input);
      in = fopen(input, "r");

    }
    while (strstr(temp,".txt") == NULL){
      printf("invalid output filename. must include \".txt\"\n");
      printf("enter a new name: ");
      scanf("%s", temp);
    }
    out = fopen(temp, "w");
    
    while (fscanf(in, "%s", input) == 1){
      if ((strcmp(input, ".") == 0) || wordCount >= MAX_WORDS)
        break;
      if (strlen(input) > MAX_LEN - 1){
        printf("%s is too long\n", input);
        // clear the rest of the line
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
          continue;
      }
      else{ 
        strcpy(words[wordCount].text, input);
        words[wordCount].length = strlen(input);
      }
      wordCount++; 
    }
    uppercase(words, wordCount);
    sort(words, wordCount);
    displayWords(words, wordCount, out);
    placeFirst(words, sboard, pboard, wordCount);
    placeNext(words, skipped, placed, sboard, pboard, wordCount, &placedCount, &skippedCount, out);
    retrySkipped(placed, skipped, words, sboard, pboard, placedCount, skippedCount, wordCount, out);
    displayBoard(sboard, out); 
    displayBoard(pboard, out);
    shuffle(words, wordCount);
    clues(words, wordCount, out);
    fclose(out);
  }
  else{
    printf("Invalid. Entered too many arguments");
  }
 
  return 0;
}

