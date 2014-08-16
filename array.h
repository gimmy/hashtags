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

int inserisci_hash(char* hashtag, int idtweet, Hashtag* H, int position) {
  int new_position;

  if (position == 0) { // sto inserendo per la prima volta
    strcpy( H[position].tag, hashtag );
    H[position].occur[0] = idtweet;
    H[position].free = 1;
    new_position = position + 1;
  }
  else {
    int found = cerca_hash(hashtag, H);

    if ( (found >= 0) && (position > 0) ) // hashtag just in array
      {
#ifdef HDEBUG
	printf ("%s già inserita, aggiungo occorrenze\n",hashtag);
#endif
	/* Aggiungo id tweet nelle occorrenze */
	int f = H[found].free;
	H[found].occur[f] = idtweet;
	H[found].free = H[found].free + 1;
	new_position = position;
      }
    else if (found < 0) {	// new hashtag
      strcpy( H[position].tag, hashtag );
      H[position].occur[0] = idtweet;
      H[position].free = 1;
      new_position = position + 1;
    }

  }
  return new_position; 	    /* ritorna la prossima posizione libera */
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

int inserisci_user(char* sname, char* name, int idtweet, User* U, int position) {
  int new_position;

  if (position == 0) {		// primo inserimento
    strcpy( U[position].screen_name, sname );
    strcpy( U[position].name, name );

    U[position].cip[0] = idtweet;
    U[position].free = 1;
    new_position = position + 1;
  }
  else {			// altrimenti cerco e se manca inserisco
    int found = cerca_user(sname, U);

    if ( (found >= 0) && (position > 0) ) // user just in array
      {
#ifdef HDEBUG
	printf ("%s già inserito, aggiungo tweet in cip\n",sname);
#endif
	/* Aggiungo id tweet */
	int f = U[found].free;
	U[found].cip[f] = idtweet;
	U[found].free = U[found].free + 1;
	new_position = position;
      }
    else if (found < 0) {	// new user
      strcpy( U[position].screen_name, sname );
      strcpy( U[position].name, name );

      U[position].cip[0] = idtweet;
      U[position].free = 1;
      new_position = position + 1;
    }

  }
  return new_position;
}
