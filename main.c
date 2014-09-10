/* main.c - Progetto Algoritmi e Strutture di Dati */
#define _GNU_SOURCE

#include "def.h"

/* #include "array.c" */
/* #include "parser.c" */
/* #include "implicit.c" */

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

#ifdef DEBUG 
  printf (" Allocazione riuscite\n");
  printf ("\n\t -- Parse and save Tweet -- \n");
#endif
  int skipped = 0;

  while ( ((read = getline(&line, &len, fp)) != -1) && i < NTWEET ) {
    if ( ParseTweet(line, T, i, H, &l, U, &m) == 0 )
      i++;
    else
      skipped++;
  }

  /* Resoconto */
  printf ("\n-> Lette %d righe, salvati %d Tweet, %d saltati \n",i+skipped,i,skipped);
  printf ("\n\t Trovati %d #hashtag da %d utenti \n", l, m);

  #ifdef DEBUG
  /* Stampo tweet */
  printf ("\n Tweet letti: \n");
  for (int j = 0; j < i; ++j)	  
    stampa_tweet( j, T, U, H );

  /* Stampo utenti */
  printf ("\n\n User trovati: \n\n");
  for (int j = 0; j < m; ++j)
    stampa_utente(j, U);
  #endif

  if(l > 0) { // se ci sono hashtag
    /* Stampo gli hashtags */
    //#ifdef DEBUG
    printf ("\n some #: ");
    int R = rand(); R = R % 30;
    for (int i = 0; i < R; ++i)
      printf ("%s ", H[i].tag);
    printf ("\n");
    //#endif

    /* Cerco impliciti */
    /* printf ("\n\tCerco impliciti..."); */
    /* for (int v = 0; v < l; v++) */
    /*   { */
    /* 	lookup_implicit_hash(v, T, H, U); */
    /*   } */
    /* printf (" Fatto.\n"); */

    /* Cerca Hash */
    char s[MAX_LENGTH];
    for(int j=0; j < 3; j++) {
      //      while ( l < 2 ) {
      printf ("\nInserire hashtag da cercare: ");
      fgets(s, MAX_LENGTH, stdin);

      // elimino l'a-capo (newline) finale
      int len = strlen(s);
      if (s[len - 1] == '\n') {
	s[len - 1] = 0;
	len = len-1;
      }
      
      if( len < 2 )
	printf ("Hashtag troppo corto\n");
      else {
	printf (" Cerco [#%s] ...",s);
	int h = cerca_hash(s,H,l);
	if (h < 0)	
	  printf ("non trovato\n");
	else {
	  printf ("trovato.\n");
	  /* Stampo utenti che hanno usato l'# */
	  printf (" Usato da %d utenti: ",H[h].usedby_f);
	  for(int n = 0; n < H[h].usedby_f; n++)
	    printf ("%s ", U[ H[h].usedby[n] ].screen_name);

	  /* Stampo tweet relativi */
	  printf ("\n\n Tweet: \n");
	  for (int j = 0; j < H[h].occur_f; ++j)
	    stampa_tweet( H[h].occur[j], T, U, H );	  

	  /* Cerco impliciti */
	  printf ("\n\tCerco usi impliciti...");
	  lookup_implicit_hash(h, T, H, U);
	  printf (" \n");

	  stampa_himpl(h,T,U,H);

	}
      }
    }
  }
  else
    printf ("\n No #hashtag found\n");

  if (line)
    free(line);
  exit(EXIT_SUCCESS);
  fclose ( fp );

  /* Cencello il Trie */
  /* ... */

  free(T); free(H); free(U);
  return 0;
}
