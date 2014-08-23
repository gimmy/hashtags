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
    printf ("word: %s\n",word);

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
      for (int j = 0; j < u.at_f; ++j)
	{
	  User a = U[ u.at[j] ]; // prendo @utente adiacente a u
	  for (int t = 0; t < a.cip_f; ++t) { // vedo i suoi tweet
	      search_w(h.tag, T[t].text, hash, t, H, &himpl);
	    }
	}
    }
  if( himpl > 0 )
    printf ("\n\tTrovati %d implicit for %s ",himpl, h.tag);
}
