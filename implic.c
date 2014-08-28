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

/* 
 * Controlla la presenza implicita
 * dell'hash nei tweet dell'utente a
 */
void view_user_tweet(User a, char* hash, int idhash, Tweet* T, Hashtag* H, int* himpl){

  for (int n = 0; n < a.cip_f; ++n) { 
    int t = a.cip[n];

    char text[LEN];	            // devo usare una variabile 
    strncpy(text, T[t].text, LEN);  // di appoggio per strtok

    //printf (" \n cerco nel Tweet[%d]:",t);
    search_w(hash, text, idhash, t, H, himpl);
  }
  /* ..e segno l'utente come controllato per l'hash */
  add(idhash, a.hash_check, &a.hash_check_f, 100);
}

/* Trova Hashtag impliciti */
void lookup_implicit_hash(int hash, Tweet* T, Hashtag* H, User* U) {
  Hashtag h = H[hash]; 
  int himpl = 0;
 
  for (int i = 0; i < h.usedby_f; ++i)
    {
      User u = U[ h.usedby[i] ]; // prendo un autore
      view_user_tweet(u, h.tag, hash, T, H, &himpl);
      //printf (" usato da %s, ",u.name); 
      for (int j = 0; j < u.at_f; ++j)
	{
	  User a = U[ u.at[j] ]; // prendo @utente adiacente a u

	  /* se non ho già controllato l'utente a per questo hashtag.. */
	  if( cerca(hash, a.hash_check, a.hash_check_f) < 0 ) {			
	    //printf ("adiacente a %s.",a.name);

	    /* vedo i suoi tweet */
	    view_user_tweet(a, h.tag, hash, T, H, &himpl);
	  }
	}
    }

  if( himpl > 0 )
    printf (" %d (#)%s, ",himpl, h.tag);
  /* else */
  /*   printf ("\t (nulla) "); */
}
