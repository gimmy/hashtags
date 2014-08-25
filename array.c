/* array.c - Progetto Algoritmi e Strutture di Dati */

#include "def.h"

/* 
 * Cerca intero in un array:
 * ritorna posizione dell'elemento
 *  se trovato, -1 altrimenti
 */
int cerca(int x, int* array, int N) {
  int found = -1; int i = 0;
  while(i < N && found < 0){
    if ( array[i] == x )
      found = i;
    else
      i++;
  }
  return found;
}

/* 
 * Aggiunge intero x nell'array a,
 * nella prima posizione libera (free_p)
 * solo se non presente.
 */
int add(int x, int* a, int* free_p, int len_a) {

  int p = cerca(x, a, *free_p);
  int check = 0;

  if ( p == -1 ) {
      p = *free_p;		// prendo prima posizione libera
      //printf ("(inizio) free_p: %d, len_a: %d\n",p, len_a);
      if ( p+1 < len_a ) {
	a[p] = x;
	*free_p = p+1;		// aggiorno 
      }
      else {
	check = 1;
	/* printf (" [ "); */
	/* for (int j = 0; j < len_a; ++j) */
	/*   printf ("%d ",a[j]); */
	/* printf (" ] \n"); */
	//	ERR("Full array!");
      }
  }
  //printf ("(uscita) free_p: %d, len_a: %d\n",p, len_a);
  return check;
}

void stampa_tweet(int id, Tweet* T, User* U, Hashtag* H) {

  int a = T[id].author;

  printf ("\n Tweet[%d] %s (%s) scrive:\n %s\n",id, \
	  U[a].name, U[a].screen_name, T[id].text);

  if(T[id].nhash > 0){
    printf ("  #[ ");
    for (int j = 0; j < T[id].nhash; ++j)
      {
	int hg = T[id].hash[j];
	printf (" %s", H[hg].tag);
      }
    printf ("]\n");
  }

  /* Print dest Users */
  if ( T[id].udest != 0 ) {
    if ( T[id].udest > 10 || T[id].udest < 0 ) {
	printf ("udest anomalo = %d\n", T[id].udest );
	assert (T[id].udest <= 10); // evito overflow    
      }
    else {
      int u = 0;
      while (u < T[id].udest) {
	printf ("  @user[%d] : %s (%s)\n",u, \
		U[T[id].dest[u]].name, U[T[id].dest[u]].screen_name);
	u++;
      }    
    }
  }
}

void stampa_himpl(int id, Tweet* T, User* U, Hashtag* H) {

  if(H[id].impl_f > 0){
    Hashtag h = H[id];
    printf ("\t Occorenze implicite: \n");
    for (int j = 0; j < h.impl_f; ++j)
      {
	int tw = h.impl[j];
	stampa_tweet(tw, T, U, H);
      }
  }
}

void stampa_utente(int id, User* U) {
  printf (" Utente[%d]: %s (%s) ",id, U[id].name, U[id].screen_name); 
  stampa_at(id, U);
  /* printf ("\n"); */
}

/* Stampa utenti menzionati da (id) in @[] */
void stampa_at(int id, User* U) {
  if( U[id].at_f > 0 ) { 
    int a; printf ("  @[ ");
    for (int u = 0; u < U[id].at_f; ++u)
      {
	a = U[id].at[u];
	printf ("%s, ", U[a].screen_name);
      }
    printf (" ]\n");
  }
}

/*** Hashtags Array ***/

int cerca_hash(char* parola, Hashtag* H) {
  int found = -1; int i = 0;
  while(i < NHASH && found < 0){
    if ( strcmp(parola, H[i].tag) == 0 )
	found = i;
    else
      i++;
  }
  return found;
}

int inserisci_hash(char* hashtag, int idtweet, Hashtag* H, int* position) {

  int found = cerca_hash(hashtag, H);

  if (found < 0) {		// new hashtag
      int p = *position;	// prendo la prima posizione libera
      strcpy( H[p].tag, hashtag );

      H[p].occur_f = 0;

      /* // inizializzo usedby[] */
      /* for (int i = 0; i < DIM; ++i) */
      /* 	H[p].usedby[i] = -1; */

      H[p].usedby_f = 0;
      H[p].impl_f = 0;

      *(position) = p+1;
      found = p;
    }

    /* Aggiungo sempre id tweet nelle occorrenze */
    int f = H[found].occur_f;
    H[found].occur[f] = idtweet;
    H[found].occur_f = H[found].occur_f + 1;


  return found;	       /* ritorna posizione dell'hashtag nell'array */

}


/*** User Array ***/

int cerca_user(char* utente, User* U) {
  int found = -1; int i = 0;
  while(i < NUSER && found < 0){
    if ( strcmp(utente, U[i].screen_name) == 0 )
	found = i;
    else
      i++;
  }
  return found;
}

int inserisci_user(char* sname, int idtweet, User* U, int* position, int u) {

  int found = cerca_user(sname, U);

  if (found < 0) {		// new user
      int p = *position;	// prendo la prima posizione libera
      strcpy( U[p].screen_name, sname );

      U[p].at_f = 0;		/* in at[] segna 0 */
      U[p].cip_f = 0;		/* e in cip[] */

      found = p;
      *(position) = p+1;
    }

    /* Aggiungo id tweet nei cip SOLO se è l'autore */
    if( u == 0 ) { 
      int f = U[found].cip_f;
      U[found].cip[f] = idtweet;
      U[found].cip_f = U[found].cip_f + 1;
    }

  return found;	       /* ritorna posizione dell'utente nell'array */
}
