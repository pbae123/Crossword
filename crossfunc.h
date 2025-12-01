//define macro variables 
#define MAX 15
#define MAX_LEN 16
#define MAX_WORDS 20
typedef struct{
  char text[MAX_LEN]; //user input 
  int length; //word length 
  char anagram[MAX_LEN];
  int row; 
  int col; 
  char direction[10];
} Word;

void initSol(char sboard[][MAX]);
void initPuz(char pboard[][MAX]);
void displayBoard(char board[][MAX], FILE *out);
void uppercase(Word Words[MAX_WORDS], int count);
int isLetter(char input[MAX]);
void sort(Word words[MAX_WORDS], int count);
void displayWords(Word words[MAX_WORDS], int count, FILE *out);
void placeFirst(Word words[MAX_WORDS], char sboard[][MAX], char pboard[][MAX], int count);
void shuffle(Word words[MAX_WORDS], int count);
void clues(Word words[MAX_WORDS], int count, FILE *out);
void placeNext(Word words[MAX_WORDS], Word skipped[MAX_WORDS], Word placed[MAX_WORDS], char sboard[][MAX], char pboard[][MAX], int count, int *placedCount, int *skippedCount, FILE *out);
bool findIntersection(Word prev, Word *next, char sboard[][MAX]);
bool bounds(int nrow, int ncol, Word word);
int neighbors(int nrow, int ncol, char sboard[][MAX], Word next);
void retrySkipped(Word placed[MAX_WORDS], Word skipped[MAX_WORDS], Word words[MAX_WORDS], char sboard[][MAX], char pboard[][MAX], int placedCount, int skippedCount, int wordCount, FILE *out);




