/* main.c - Progetto Algoritmi e Strutture di Dati */

#include "def.h"
#include "library.h"

//#define MAX_STRING 1024

int countSubstring(const char *str, const char *sub);

void Parser(char* text, Tweet* Tweets, int id);

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

  /* /\* Fill it *\/ */
  /* for (int id = 0; id < N; ++id) */
  /*   { */
  /*     Parser(text,Tweets,id); */
  /*   } */

  free(Tweets);
  return 0;
}

void Parser(char* text, Tweet* Tweets, int id){
  char* p = strstr(text, "'text'");  // ritorna un puntatore all'inizio della sottostringa
  p = p + strlen("'text': ");	     // posizione inizio testo: 'blabla...
  int lengh = strcspn(++p,"'");	     // lengh of text: ...bla bla'

  strncpy(Tweets[id].text, p, lengh); // copio testo tra apici da p nell'array 

#ifdef DEBUG
  printf("id: [%d]\t", id);
  printf("%s\n", Tweets[id].text);
#endif

  char* campo;
  char* label;
  /* while(campo != NULL || int j < 100){ */
  /*   campo = strtok (text, "'"); */
  /*   printf ("%s\n",campo); */
  /*   campo = strtok (NULL, "'"); */
  /*   j++; */
  /* } */
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
