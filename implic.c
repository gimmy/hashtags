/* 
 * implicit.c
 * Look up for implicit hashtags
 */

#include "def.h"
//Hashtag* iH = malloc( l * sizeof(Hashtag) ); int n = 0;

/* Cerco parola nel testo */
void search_w(char* parola, char* text, int idhash, int idtweet, Hashtag* H, int* himpl) {
  int len_p = strlen(parola);

  /* Spezzo il testo in parole */ 
  char* word;
  word = strtok (text, " ',;:");
  while (word != NULL) {

    int lenw = strlen(word);
#ifdef DEBUG
    printf (" %s",word);
#endif
    if( (lenw >= len_p) && (strncmp(word, parola,len_p) == 0) ) {
      add(idtweet, H[idhash].impl, &H[idhash].impl_f, L); *himpl = *himpl +1;
    }

    word = strtok (NULL, " ',;:");
  }
}

/* Trova Hashtag impliciti */
void lookup_implicit_hash(int hash, Tweet* T, Hashtag* H, User* U) {
  Hashtag h = H[hash]; 
  int himpl = 0;
 
  for (int i = 0; i < h.usedby_f; ++i)
    {
      User u = U[ h.usedby[i] ]; // prendo un autore
      //printf (" usato da %s, ",u.name); 
      for (int j = 0; j < u.at_f; ++j)
	{
	  User a = U[ u.at[j] ]; // prendo @utente adiacente a u
	  //printf ("adiacente a %s.",a.name); 
	  for (int n = 0; n < a.cip_f; ++n) { 
	    int t = a.cip[n];	// vedo i suoi tweet

	    char text[LEN];	/* devo usare una variabile di appoggio per strtok */
	    strncpy(text, T[t].text, LEN);
	    printf (" \n cerco nel Tweet[%d]:",t);
	    search_w(h.tag, text, hash, t, H, &himpl);
	  }
	}
    }
  if( himpl > 0 )
    printf ("\t -> %d (#)%s ",himpl, h.tag);
  else
    printf ("\t (nulla) ");
}
