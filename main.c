/* main.c - Progetto Algoritmi e Strutture di Dati */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#include "def.h"


#define MAX_STRING 1024

char* get_text_from_file(char* input); /* testo del file in stdin */
int countSubstring(const char *str, const char *sub);
int get_tweet_number(char* text);   /* ritorna il numero di Tweet */

void fill_tweet_text(char* text, Tweet* Tweets, int id);


int main(int argc, char **argv) {
  char* text; 
  int N=0;

  /* Get text from input file */
  text = get_text_from_file(argv[1]);
  /* Get size of array - total tweet */
  N = get_tweet_number(text);
#ifdef DEBUG
  printf ("Numero di tweet: %d\n",N);
#endif
  /* Alloco array dei tweet */
  Tweet* Tweets = malloc (N * sizeof (Tweet));

  /* Fill it */
  for (int id = 0; id < N; ++id)
    {
      fill_tweet_text(text,Tweets,id);
    }

  free(Tweets);
  return 0;
}

void fill_tweet_text(char* text, Tweet* Tweets, int id){
  char* p = strstr(text, "'text'");  // ritorna un puntatore all'inizio della sottostringa
  p = p + strlen("'text': ");	     // posizione inizio testo: 'blabla...
  int lengh = strcspn(++p,"'");	     // lengh of text: ...bla bla'

  strncpy(Tweets[id].text, p, lengh); // copio testo tra apici da p nell'array 

#ifdef DEBUG
  printf("id: [%d]\t", id);
  printf("%s\n", Tweets[id].text);
#endif

}

char* get_text_from_file(char* input) {  
  char* text;
  FILE* fd;
  struct stat stat_buf;
  int fileno( FILE * );
  int tolower(int);		/* ? */

  /* open the text file (read and write) */
  fd = fopen( input, "r+w" );
  if ( fd == NULL ) 
    ERR( "Open failed. Give text file in input." );

  /* get text length in bytes */
  if ( fstat(fileno(fd), &stat_buf) == -1 ) 
    ERR("stat failed");
  int n = stat_buf.st_size;		/* get file size */
  
  /* mmap the open text to a local variable */
  text = (char *)mmap(NULL, n, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(fd), 0);
  if( text == NULL ) 
    ERR("mmap failed");

  fclose( fd );

  return text;
}

int countSubstring(const char *str, const char *sub)
{
    int length = strlen(sub);
    if (length == 0) return 0;
    int count = 0;
    for (str = strstr(str, sub); str; str = strstr(str + length, sub))
        ++count;
    return count;
}

int get_tweet_number(char* text){
  int total_tweet;
  total_tweet = countSubstring(text, "'lang'");
  return total_tweet;  
}
