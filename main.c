/* main.c - Progetto Algoritmi e Strutture di Dati */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NTWEET 10000		/* numero di Tweet da leggere */
#define NHASH NTWEET/10		/* uso NTWEET come bound */
#define NUSER NTWEET/2

#include "def.h"
#include "array.c"
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

  /* Alloco array */
  Tweet* T = malloc( NTWEET * sizeof(Tweet) ); int i = 0;
  Hashtag* H = malloc( NHASH * sizeof(Hashtag) ); int l = 0;
  User* U = malloc ( NUSER * sizeof(User) ); int m = 0;

  /* Creo il Trie */
  // trie_nodo* radice = crea_nodo();

#ifdef DEBUG 
  printf ("\n\t -- Parse and save Tweet -- \n");
#endif
  int skipped = 0;

  while ( ((read = getline(&line, &len, fp)) != -1) && i < NTWEET ) {
    //printf ("\nline: %d\t",j++);
    if ( ParseTweet(line, T, i, H, &l, U, &m) == 0 )
      i++;
    else
      skipped++;
  }


#ifdef DEBUG
  /* Stampo gli hashtags */
  for (int i = 0; i < l; ++i)
    {
      printf ("H[%d] = %s\n",i,H[i].tag);
    }
#endif  

  /* Resoconto */
  printf ("\n-> Lette %d righe, salvati %d Tweet, %d saltati \n",i+skipped,i,skipped);
  printf ("\t Trovati %d #hashtag da %d utenti \n", l, m);


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
      printf ("\nInserire hashtag da cercare: ");
      fgets(s, MAX_LENGTH, stdin);

      // elimino l'a-capo (newline) finale
      int l = strlen(s);
      if (s[l - 1] == '\n') {
    	s[l - 1] = 0;
      }
    }

    printf ("Cerco #%s ...",s);
    int h = cerca_hash(s,H);
    if (h < 0)	
      printf ("non trovato\n");
    else {
      printf ("trovato! - occorrenze: \n");
      /* Stampo tweet relativi */
      for (int j = 0; j < H[h].free; ++j)	  
	stampa_tweet( H[h].occur[j], T, U );	  
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

  free(T); free(H); free(U);
  return 0;
}
