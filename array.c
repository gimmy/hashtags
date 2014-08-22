/* array.c - Progetto Algoritmi e Strutture di Dati */

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
 * solo se non presente, nella 
 * prima posizione libera (free_p).
 */
void add(int x, int* a, int* free_p, int len_a) {

  int p = cerca(x, a, len_a);

  if ( p == -1 ) {
    a[*free_p] = x;
    p = *free_p;
    *free_p = *free_p +1;
  }
}


/* Hashtags Array */

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

    if ( found >= 0 ) // hashtag alredy in array
      {
#ifdef HDEBUG
	printf ("%s già inserita, aggiungo occorrenze\n",hashtag);
#endif
	/* Aggiungo id tweet nelle occorrenze */
	int f = H[found].free;
	H[found].occur[f] = idtweet;
	H[found].free = H[found].free + 1;

      }
    else if (found < 0) {	// new hashtag
      int p = *position;	// prendo la prima posizione libera
      strcpy( H[p].tag, hashtag );
      H[p].occur[0] = idtweet;
      H[p].free = 1;

      *(position) = p+1;
      found = p;
    }

  return found;	       /* ritorna posizione dell'hashtag nell'array */

}


/* User Array */

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

int inserisci_user(char* sname, int idtweet, User* U, int* position) {

  int found = cerca_user(sname, U);

    if ( found >= 0 ) // user alredy in array
      {
#ifdef HDEBUG
	printf ("%s già inserito, aggiungo occorrenze\n",sname);
#endif
	/* Aggiungo id tweet nelle occorrenze */
	int f = U[found].free;
	U[found].cip[f] = idtweet;
	U[found].free = U[found].free + 1;

      }
    else if (found < 0) {	// new user
      int p = *position;	// prendo la prima posizione libera
      strcpy( U[p].screen_name, sname );
      U[p].cip[0] = idtweet;
      U[p].free = 1;

      U[p].at_free = 0;		/* in at[] segna 0 */

      *(position) = p+1;
      found = p;
    }

  return found;	       /* ritorna posizione dell'hashtag nell'array */
}