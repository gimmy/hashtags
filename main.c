/* main.c - Progetto Algoritmi e Strutture di Dati */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NTWEET 1		/* numero di Tweet da leggere */

#include "def.h"
#include "parser.h"
#include "trie.h"
#include "filter.h"

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

  /* Creo il Trie */
  trie_nodo* radice = crea_nodo();

#ifdef DEBUG 
  printf ("\n\t -- Parse and save Tweet -- \n");
#endif
  int skipped = 0;
  //int j = 0;
  while ( ((read = getline(&line, &len, fp)) != -1) && i < NTWEET ) {
    //printf ("\nline: %d\t",j++);
    if ( ParseTweet(line, &T[i]) == 0 )
      i++;
    else
      skipped++;
  }

  printf ("Lette %d righe, salvati %d Tweet, %d saltati\n",i+skipped,i,skipped);

#ifdef DEBUG 
  printf ("\n\t -- Insert in Trie form Tweet[] saved -- \n");
#endif
  /* Insert in Trie from Tweet.text */
  for (i = 0; i < NTWEET; ++i)
    {
#ifdef DEBUG
      printf ("\n Tweet[%d] %s (%s) scrive:\n %s\n",i, \
	      T[i].author.name,T[i].author.screen_name, T[i].text);
      int u = 0;
      while (T[i].udest != 0 && u < T[i].udest) {
	printf (" @user[%d] : %s (%s)\n",u,T[i].dest[u].name, T[i].dest[u].screen_name);
	u++;
      }
      int h = 0;
      while (T[i].nhash > 0 && h < T[i].nhash) {
	printf (" #hash[%d] : %s \n", h, T[i].hash[h].tag);
	h++;
      }
#endif
      InTrie(T[i].text, radice);
    }
  fprintf(stderr, "Costruita trie con %d nodi\n", conta_nodi(radice));

    //stampa_stringhe(radice);

    /* Cerca nel Trie */
    char s[MAX_LENGTH];
    for(int j=0; j < 1; j++) {
      printf ("Inserire stringa da cercare: ");
      fgets(s, MAX_LENGTH, stdin);

      // elimino l'a-capo (newline) finale
      int l = strlen(s);
      if (s[l - 1] == '\n') {
  	s[l - 1] = 0;
      }

      trie_nodo* nodo_prefisso = cerca(radice, s, 1);
      if (nodo_prefisso) {
  	char out[MAX_LENGTH];
  	// copia il prefisso iniziale nella stringa corrente
  	strcpy(out, s);

  	// visita tutti i completamenti
  	stampa_stringhe_visita(nodo_prefisso, out, strlen(s));
      }
      else
  	printf("Prefisso non trovato\n");
      printf("\n");
    }



  if (line)
    free(line);
  exit(EXIT_SUCCESS);
  fclose ( fp );

  /* Cencello il Trie */
  /* ... */

  free(T);
  return 0;
}
