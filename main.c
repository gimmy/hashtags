/* main.c - Progetto Algoritmi e Strutture di Dati */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NTWEET 10		/* numero di Tweet da leggere */

#include "def.h"
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
  Tweet* T = malloc( NTWEET * sizeof(Tweet) ); int i = 0;

  while ( ((read = getline(&line, &len, fp)) != -1) && i < NTWEET ) {
#ifdef DEBUG
    printf("Retrieved line of length %zu\n", read);
#endif
    T[i] = ParseTweet(line); 
#ifdef DEBUG
    printf ("Tweet[%d] %s (%s) scrive:\n",i, T[i].author.name,T[i].author.screen_name);
    printf ("%s\n\n",T[i].text);
#endif
    i++;
  }

  if (line)
    free(line);
  exit(EXIT_SUCCESS);
  fclose ( fp );


  free(T);
  return 0;
}
