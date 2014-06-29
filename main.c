/* main.c - Progetto Algoritmi e Strutture di Dati */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

#define NTWEET 10		/* numero di Tweet da leggere */
#include "def.h"
//#include "library.h"
#include "parser.h"



int main(int argc, char **argv) {
  FILE* fp;
  char* line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(argv[1], "r");
  if (fp == NULL)
    ERR("Open failed. Give text file in input.\n");

  /* Alloco array dei Tweet */
  Tweet* T = malloc( NTWEET * sizeof(Tweet) );  
  int i = 0;

  strcpy(T[2].text, "ma funge?");
  strcpy(T[2].author.name, "Pippo");
  strcpy(T[2].author.screen_name, "superfoo");


  while ( ((read = getline(&line, &len, fp)) != -1) && i < NTWEET ) {
    printf("Retrieved line of length %zu :\n", read);
    Tweet ohi = T[i];
    ParseTweet(line, ohi); 

    printf ("Tweet[%d] %s (%s) scrive:\n",i, T[i].author.name,T[i].author.screen_name);
    printf ("%s\n",ohi.text);

    i++;
  }

  printf ("%s (%s) : %s\n\n", T[2].author.name, T[2].author.screen_name, T[2].text);

  if (line)
    free(line);
  exit(EXIT_SUCCESS);
  fclose ( fp );


  free(T);
  return 0;
}
