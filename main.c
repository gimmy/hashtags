/* main.c - Progetto Algoritmi e Strutture di Dati */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NTWEET 10000		/* numero di Tweet da leggere */
#define NHASH NTWEET		/* uso NTWEET come hashtag bound */

#include "def.h"
#include "parser.h"
#include "array.h"
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
  Hashtag* H = malloc( NHASH * sizeof(Hashtag) );

  /* Creo il Trie */
  // trie_nodo* radice = crea_nodo();

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


#ifdef HDEBUG 
  printf ("\n\t -- Insert Hashtag in array -- \n");
#endif
  int p = 0;
  for (i = 0; i < NTWEET; ++i)
    {
      int h = 0;
      while (T[i].nhash > 0 && h < T[i].nhash) {
#ifdef HDEBUG
	printf ("Tweet[%d], #(%d) -> Hash[%d]: %s \n", i, h, p, T[i].hash[h].tag);
#endif
	/* inserisco hashtag nell'array */
	/* ritorna nuova posizione libera */
	p = inserisci_hash(T[i].hash[h].tag, i, H, p);
	h++;
      }
    }

  printf ("Lette %d righe, salvati %d Tweet, %d saltati - ",i+skipped,i,skipped);
  printf (" %d Hashtag salvati\n", p);

  
/* #ifdef DEBUG */
/*   printf ("\n\t -- Insert in Trie form Tweet[] saved -- \n"); */
/* #endif */
/*   /\* Insert in Trie from Tweet.text *\/ */
/*   for (i = 0; i < NTWEET; ++i) */
/*     { */
/* #ifdef DEBUG */
/*       stampa_tweet(i,T); */
/* #endif */
/*       //InTrie(T[i].text, radice); */
/*     } */
/*   fprintf(stderr, "Costruita trie con %d nodi\n", conta_nodi(radice)); */

    //stampa_stringhe(radice);

    /* Cerca Hash */
    char s[MAX_LENGTH];
    for(int j=0; j < 1; j++) {
      printf ("\nInserire stringa da cercare: ");
      fgets(s, MAX_LENGTH, stdin);

      // elimino l'a-capo (newline) finale
      int l = strlen(s);
      if (s[l - 1] == '\n') {
    	s[l - 1] = 0;
      }
    }

    printf ("Cerco %s ...",s);
    int h = cerca_hash(s,H);
    if (h < 0)	
      printf ("non trovato\n");
    else {
      printf ("trovato! - occorrenze: \n");
      for (int j = 0; j < H[h].free; ++j)	  
	stampa_tweet(H[h].occur[j],T);	  
    }
	

    /*   trie_nodo* nodo_prefisso = cerca(radice, s, 1); */
    /*   if (nodo_prefisso) { */
    /* 	char out[MAX_LENGTH]; */
    /* 	// copia il prefisso iniziale nella stringa corrente */
    /* 	strcpy(out, s); */

    /* 	// visita tutti i completamenti */
    /* 	stampa_stringhe_visita(nodo_prefisso, out, strlen(s)); */
    /*   } */
    /*   else */
    /* 	printf("Prefisso non trovato\n"); */
    /*   printf("\n"); */
    /* } */



  if (line)
    free(line);
  exit(EXIT_SUCCESS);
  fclose ( fp );

  /* Cencello il Trie */
  /* ... */

  free(T); free(H);
  return 0;
}
