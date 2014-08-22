/* parser.h
 * Parser line and save Tweet 
 */

#include "jsmn/jsmn.h"

#define TOKEN_EQ(t, tok_start, tok_end, tok_type) \
	((t).start == tok_start \
	 && (t).end == tok_end  \
	 && (t).type == (tok_type))

#define TOKEN_STRING(js, t, s) \
	(strncmp(js+(t).start, s, (t).end - (t).start) == 0 \
	 && strlen(s) == (t).end - (t).start)

#define TOKEN_PRINT(t) \
	printf("start: %d, end: %d, type: %d, size: %d\n", \
			(t).start, (t).end, (t).type, (t).size)

//#define JSMN_STRICT
#include "jsmn/jsmn.c"

/* Get & save an Hashtag */
void ScanHash(char* aux, Tweet* t, int idtweet, int h, Hashtag* H, int* pl) {
  unsigned int end = strlen(aux); 
  unsigned int length = 0;

      /* Parse */
      int r;			
      jsmn_parser p;
      jsmntok_t tokens[128];

      jsmn_init(&p);
      r = jsmn_parse(&p, aux, strlen(aux), tokens, 128);

      int done = 0; // cambiare quando finito

#ifdef DEBUG
      printf ("Entro nell'Hash parser \n");
#endif
	  
      for ( int j = 1; (tokens[j].end <= end) && !done; j++ ) { 

	if ( !done && TOKEN_STRING(aux, tokens[j], "text") ) {
	    j = j+1;
	    /* Save hash... */
	    length = tokens[j].end - tokens[j].start;

	    char hash[length];
	    memcpy(hash, &aux[tokens[j].start], length);
	    hash[length] = '\0';

	    /* ... in Hasharray & in Tweet */
	    t->hash[h] = inserisci_hash( hash, idtweet, H, pl );

	    done = 1; // salvato
	  }
      }
}

/* Get & save an User (author or mentioned) */
void ScanUser(char* aux, Tweet* t, int idtweet, int u, User* U, int* pm) {
  unsigned int end = strlen(aux); 
  unsigned int length = 0;

      /* Parse */
      int r;			
      jsmn_parser p;
      jsmntok_t tokens[128];

      jsmn_init(&p);
      r = jsmn_parse(&p, aux, strlen(aux), tokens, 128);

      int screen_name_done = 0;
      int name_done = 0;

#ifdef DEBUG
      printf ("Entro nell'User parser ( ");
      if (u>0)
	{
	  printf ("dest )\t");
	}
      else
	printf ("author )\n");
      //printf ("token user: %s\n",aux);    
#endif
	  
      for ( int j = 1; (tokens[j].end <= end) && (!screen_name_done || !name_done); j++ ) { 

	if ( !screen_name_done && TOKEN_STRING(aux, tokens[j], "screen_name") ) {
	    j = j+1;
	    /* Save screen_name */
	    length = tokens[j].end - tokens[j].start;
	    char sname[length];
	    memcpy(sname, &aux[tokens[j].start], length);
	    sname[length] = '\0';

	    /* Salvo nell'User array & nel Tweet */

	    if (u == 0) {	// se users == 0 sto passando l'autore
	      t->author = inserisci_user( sname, idtweet, U, pm );
	    }
	    else {
	      t->dest[u-1] = inserisci_user( sname, idtweet, U, pm );
	      /* aggiungo archi in U */
	      add(t->dest[u-1], U[t->author].at, &U[t->author].at_free, DIM);
	    }
	    screen_name_done = 1; // screen_name salvato
	  }
	  else if ( !name_done && TOKEN_STRING(aux, tokens[j], "name") ) {
	    j = j+1;
	    /* Save name */
	    length = tokens[j].end - tokens[j].start;
	    char name[length];
	    memcpy(name, &aux[tokens[j].start], length);
	    name[length] = '\0';

	    /* Aggiungo solo il nome nell'array */
	    int n;

	    if (u == 0)
	      n = t->author;	    
	    else
	      n = t->dest[u-1];

	    strcpy( U[n].name, name );
	    
	    name_done = 1; // name salvato
	  }
      }
}


int SkipToken(jsmntok_t token) {
  int size = token.size;
  return size;
}

int ParseTweet(char* js, Tweet* T, int i, Hashtag* H, int* pl, User* U, int* pm) {

  /* Set mentions and hashs for further check */
  T[i].udest = -1;
  T[i].nhash = -1;
  //pTw->author.name[0] = 0;

  int result;		/* inizializzo parser */
  jsmn_parser parser;
  jsmntok_t tokens[256];

  jsmn_init(&parser);
  result = jsmn_parse(&parser, js, strlen(js), tokens, 256);

  unsigned int length = 0;
  int stop = 0;

  if (tokens[0].type == JSMN_OBJECT) {
    int EndTweet = tokens[0].end; // lunghezza del Tweet
    if (EndTweet == -1)
      return 1; // esco con errore

#ifdef DEBUG
    printf("\nTweet: {%d elems}", tokens[0].size);
    TOKEN_PRINT(tokens[0]);
#endif


    // finché non arrivo in fondo al Tweet
    for ( int j = 1; (tokens[j].end <= EndTweet) && !stop; j++ ) {

      if (tokens[j].type == JSMN_STRING || tokens[j].type == JSMN_PRIMITIVE) {		  

	if ( (T[i].text[0] == 0) && TOKEN_STRING(js, tokens[j], "text") ) {
	  j = j+1; 
	  /* Salvo testo in T[i].text */
	  length = tokens[j].end - tokens[j].start;
	  memcpy(T[i].text, &js[tokens[j].start], length);
	  T[i].text[length] = '\0';
	}

	/** Grep info from Tweet **/

	/* User mentions */
	else if ( (T[i].udest < 0) && TOKEN_STRING(js, tokens[j], "user_mentions") ) {
	  if ( (tokens[j+1].type == JSMN_ARRAY) && (tokens[j+1].size > 0) ) {
	    j++;
	    #ifdef DEBUG
	    printf("@users: [%d elems]\n", tokens[j].size); 
	    #endif
	    int dest_users = tokens[j].size;
	    T[i].udest = dest_users; // aggiorno numero di destinatari
	    
	    j++; // entro nell'array utenti
	    int end_u =  tokens[j].end;

	    /* Salvo utenti menzionati */
	    for (int u = 1; u <= dest_users; u++) {	

	      end_u =  tokens[j].end;

	      length = tokens[j].end - tokens[j].start;
	      char aux[length];
	      memcpy(aux, &js[tokens[j].start], length);
	      aux[length] = '\0';
	      
	      ScanUser(aux, &T[i], i, u, U, pm);

	      while(tokens[j].start < end_u) // scorro fino al prossimo user
		j++;
	    }
	      
	  }
	  else {
#ifdef DEBUG
	    printf ("No user_mentions. ");
#endif
	    /* 
	     *  l'update di udest mi permette di non rientrare nell'if
	     *  e di non valutare il TOKEN_STRING()
	     */

	    T[i].udest = 0; // aggiorno numero di destinatari
	  }
	}

	/* Hashtags */
	else if ( (T[i].nhash < 0) && TOKEN_STRING(js, tokens[j], "hashtags") ) {
	  if ( (tokens[j+1].type == JSMN_ARRAY) && (tokens[j+1].size > 0) ) {
	    j++;
	    #ifdef DEBUG
	    printf("#hashtags: [%d elems]\n", tokens[j].size); 
	    #endif
	    T[i].nhash = tokens[j].size; // aggiorno numero di hashtags
	    
	    j++; // entro nell'array
	    int end_h =  tokens[j].end;

	    /* Salvo hashtags */
	    for (int h = 0; h < T[i].nhash; h++) {	

	      end_h =  tokens[j].end;

	      length = tokens[j].end - tokens[j].start;
	      char aux[length];
	      memcpy(aux, &js[tokens[j].start], length);
	      aux[length] = '\0';
	      
	      ScanHash(aux, &T[i], i, h, H, pl);

	      while(tokens[j].start < end_h) // scorro fino al prossimo hashtag
		j++;
	    }
	      
	  }
	  else {
#ifdef DEBUG
	    printf ("No hashtags. ");
#endif
	    /* 
	     *  l'update di hash mi permette di _non_ rientrare nell'if
	     *  e di non valutare il TOKEN_STRING()
	     */

	    T[i].nhash = 0; // aggiorno numero hash
	  }
	}

	else if ( !(T[i].udest < 0) && TOKEN_STRING(js, tokens[j], "user") ) {
	  j++;
	  length = tokens[j].end - tokens[j].start;
	  char aux[length];
	  memcpy(aux, &js[tokens[j].start], length);
	  aux[length] = '\0';
	  ScanUser(aux, &T[i], i, 0, U, pm);

	  stop = 1; // mi posso fermare
	}
      }
    }
  }

  /* 
   * Prima di uscire riempio usedby  
   * degli eventuali #hashtag
   */
  if(T[i].nhash > 0) {
    int a = T[i].author;
    int h;
    for (int n = 0; n < T[i].nhash; n++) {
	h = T[i].hash[n];
	add(a, H[h].usedby, &H[h].us_free, DIM);
    }
  }

  return 0;
}
