/*
 * parser.c
 * Parser line and save Tweet
 */
#include "def.h"
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

void check_result(int r, int s) {

  if(r < 0) {
    switch (s) {
    case 0: printf("parsing Tweet \n"); break;
    case 1: printf("parsing Hashtag \n"); break;
    case 2: printf("parsing User \n"); break;
    default: printf("OK, string was parsed.\n"); break;
    }
  }
  switch (r) {
  case -2: ERR("bad token, JSON string is corrupted."); break;
  case -1: ERR("not enough tokens, JSON string is too large"); break;
  case -3: ERR("JSON string is too short, expecting more JSON data"); break;
  default:
#ifdef DEBUG
    printf("OK, string parsed by JSMN.\n");
#endif
    break;
  }

}

/* void extract(char* save_here, int len_passed, int start, int end, char* js) { */
/*   int length = end - start; */

/*   if(len_passed < length) { // alloco più spazio */
/*     save_here = (char* )realloc( save_here, length * sizeof(char) ); */
/*     //printf ("aumento lo spazio (passato:%d necessario:%d)\n",len_passed,length); */
/*   } */

/*   memcpy(save_here, &js[start], length); */
/*   save_here[length] = '\0'; */
/* } */


/* Get & save an Hashtag */
void ScanHash(char* aux, Tweet* t, int idtweet, int h, Hashtag* H, int dim_H, int* pl) {
  unsigned int end = strlen(aux); 
  unsigned int length = 0;

#ifdef DEBUG
      printf ("Entro nell'Hash parser ");
#endif

      /* Parse */
      int r;
      jsmn_parser p;
      jsmntok_t tokens[128];

      jsmn_init(&p);
      r = jsmn_parse(&p, aux, strlen(aux), tokens, 128);

      int done = 0; // cambiare quando finito

      check_result(r, 1);

      for ( int j = 1; (tokens[j].end <= end) && !done; j++ ) {

	if ( !done && TOKEN_STRING(aux, tokens[j], "text") ) {
	    j = j+1;
	    /* Save hash... */
	    length = tokens[j].end - tokens[j].start;
	    char buf[length]; int sz = 256;  char hash[length];
	    memcpy(buf, &aux[tokens[j].start], length);
	    buf[length] = '\0';

	    /* convert a string "src" containing escape sequences to UTF-8 */
	    u8_unescape(hash, sz, buf);

	    /* char hash[length]; */
	    /* memcpy(hash, &aux[tokens[j].start], length); */
	    /* hash[length] = '\0'; */

	    /* Evito #RT e affini */
	    if (length < 3) {
	      char s[length];
	      for (int i=0 ; i<length; i++) s[i] = tolower(hash[i]);
	      if( strncmp(s,"rt",length) == 0 ) {
		t->hash[h] = -1; /* segno ed evito in futuro */
		done = 1;
	      }
	    }

	    if ( !done ) {
	      /* ... in Hasharray & in Tweet */
	      t->hash[h] = inserisci_hash( hash, idtweet, H, dim_H, pl );
	      /* printf ("Insert hashtag H[%d]: #%s in tweet(%d).hash[%d] \n",t->hash[h],hash,idtweet, h); */

	      done = 1; // salvato
	    }

	}
      }
}

/* Get & save an User (author or mentioned) */
void ScanUser(char* aux, Tweet* t, int idtweet, int u, User* U, int dim_U, int* pm) {
  unsigned int end = strlen(aux);
  unsigned int length = 0;


#ifdef DEBUG
      printf ("Entro nell'User parser ( ");
      if (u>0)
	{
	  printf ("dest ) ");
	}
      else
	printf ("author ) ");
#endif

      /* Parse */
      int r;
      jsmn_parser p;
      jsmntok_t tokens[256];

      jsmn_init(&p);
      r = jsmn_parse(&p, aux, strlen(aux), tokens, 256);

      check_result(r, 2);

      int screen_name_done = 0;
      int name_done = 0;


      for ( int j = 1; (tokens[j].end <= end) && (!screen_name_done || !name_done); ++j ) {

	if ( !screen_name_done && TOKEN_STRING(aux, tokens[j], "screen_name") ) {
	  ++j;
	  /* Save screen_name */
	  length = tokens[j].end - tokens[j].start;
	  char buf[length]; int sz = 256;  char sname[length];
	  memcpy(buf, &aux[tokens[j].start], length);
	  buf[length] = '\0';

	  /* convert a string "src" containing escape sequences to UTF-8 */
	  u8_unescape(sname, sz, buf);

	  /*  */
	  /* char sname[length]; */
	  /* memcpy(sname, &aux[tokens[j].start], length); */
	  /* sname[length] = '\0'; */

	  /* char *sname; sname = (char* )malloc(15); */
	  /* extract(sname, 15, tokens[j].start, tokens[j].end, aux); */


	    /* Salvo nell'User array & nel Tweet */
	    if (u == 0) {	// se users == 0 sto passando l'autore
	      t->author = inserisci_user( sname, idtweet, U, dim_U, pm, u );
	    }
	    else {
	      t->dest[u-1] = inserisci_user( sname, idtweet, U, dim_U, pm, u );
	    }
	    screen_name_done = 1; // screen_name salvato
	    //printf (" screen_name fatto. ");
	    //free(sname);
	  }
	  else if ( !name_done && TOKEN_STRING(aux, tokens[j], "name") ) {
	    j = j+1;
	    /* Save name */
	    length = tokens[j].end - tokens[j].start;
	    char buf[length]; int sz = 256;  char name[length];
	    memcpy(buf, &aux[tokens[j].start], length);
	    buf[length] = '\0';

	    /* convert a string "src" containing escape sequences to UTF-8 */
	    u8_unescape(name, sz, buf);

	    /* char name[length]; */
	    /* memcpy(name, &aux[tokens[j].start], length); */
	    /* name[length] = '\0'; */

	    /* char *name; name = (char* )malloc(15); */
	    /* extract(name, 15, tokens[j].start, tokens[j].end, aux); */

	    /* Aggiungo solo il nome nell'array U */
	    int n;

	    if (u == 0)
	      n = t->author;
	    else
	      n = t->dest[u-1];

	    strcpy( U[n].name, name );

	    name_done = 1; // name salvato
	    //printf (" name fatto. ");
	    //free(name);
	  }
      }
}

int ParseTweet(char* js, Tweet* T, int i, Hashtag* H, int dim_H, int* pl, User* U, int dim_U, int* pm) {

  /* Set mentions and hashs for further check */
  T[i].udest = -1;
  T[i].nhash = -1;

  int text_saved = 0;
  int stop = 0;

  /* inizializzo parser */
  int result;
  jsmn_parser parser;
  jsmntok_t tokens[1024];

  jsmn_init(&parser);
  result = jsmn_parse(&parser, js, strlen(js), tokens, 1024);

  check_result(result, 0);

  unsigned int length = 0;

  if (tokens[0].type == JSMN_OBJECT) {
    int EndTweet = tokens[0].end; // lunghezza del Tweet
    if (EndTweet == -1)
      return 1; // esco con errore

#ifdef DEBUG
    printf("\n[%d]Tweet: {%d elems}", i, tokens[0].size);
    TOKEN_PRINT(tokens[0]);
#endif


    // finché non arrivo in fondo al Tweet
    int j = 1;
    while( (tokens[j].end <= EndTweet) && !stop ) {
      //    for ( int j = 1; (tokens[j].end <= EndTweet) && !stop; j++ ) {

      if (tokens[j].type == JSMN_STRING || tokens[j].type == JSMN_PRIMITIVE) {

	if ( !text_saved && TOKEN_STRING(js, tokens[j], "text") ) {
	  j = j+1;
	  /* Salvo testo in T[i].text */
	  length = tokens[j].end - tokens[j].start;
	  if(length > LEN) {
	    printf ("length tweet text: %d\n",length);
	    assert(length <= LEN);
	  }
	  char buf[length]; int sz = 256;
	  memcpy(buf, &js[tokens[j].start], length);
	  buf[length] = '\0';

	  /* convert a string "src" containing escape sequences to UTF-8 */
	  u8_unescape(T[i].text, sz, buf);

#ifdef DEBUG
	  printf ("%s\n",T[i].text);
#endif
	  text_saved = 1;
	}

	/** Grep info from Tweet **/

	/* User mentions */
	else if ( (T[i].udest < 0) && TOKEN_STRING(js, tokens[j], "user_mentions") ) {
	  if ( (tokens[j+1].type == JSMN_ARRAY) && (tokens[j+1].size > 0) ) {
	    j++;
	    #ifdef DEBUG
	    printf("@users: [%d elems] ", tokens[j].size);
	    #endif
	    int dest_users = tokens[j].size;
	    T[i].udest = dest_users; // aggiorno numero di destinatari
	    assert (T[i].udest <= 20); // evito overflow

	    j++; // entro nell'array utenti
	    int end_u =  tokens[j].end;

	    /* Salvo utenti menzionati */
	    for (int u = 1; u <= dest_users; u++) {

	      end_u =  tokens[j].end;

	      length = tokens[j].end - tokens[j].start;
	      char aux[length];
	      memcpy(aux, &js[tokens[j].start], length);
	      aux[length] = '\0';

	      ScanUser(aux, &T[i], i, u, U, dim_U, pm);

	      // scorro fino al prossimo user
	      // finché ci sono utenti
	      while(tokens[j].start < end_u && u < dest_users)
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
	    printf("#hashtags: [%d elems] ", tokens[j].size);
	    #endif
	    T[i].nhash = tokens[j].size; // aggiorno numero di hashtags
	    assert (T[i].nhash <= 20);	 // evito overflow

	    j++; // entro nell'array
	    int end_h =  tokens[j].end;

	    /* Salvo hashtags */
	    for (int h = 0; h < T[i].nhash; h++) {

	      end_h =  tokens[j].end;


	      length = tokens[j].end - tokens[j].start;
	      char aux[length];
	      memcpy(aux, &js[tokens[j].start], length);
	      aux[length] = '\0';

	      /* char *aux; aux = (char* )malloc(10); */
	      /* extract(aux, 10, tokens[j].start, tokens[j].end, js); */


	      ScanHash(aux, &T[i], i, h, H, dim_H, pl);

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
	  ScanUser(aux, &T[i], i, 0, U, dim_U, pm);

	  stop = 1; // mi posso fermare
	}
      }
      j++;
    }
  }

  /*
   * Prima di uscire riempio .usedby[]
   * degli eventuali #hashtag...
   */
  if(T[i].nhash > 0) {
    int h;
    for (int n = 0; n < T[i].nhash; n++) {
	h = T[i].hash[n];
	if (h >= 0)  { // se hashtag da considerare
	  /* printf ("Aggiungo utente per #%s (hash %d) - erano %d - ",H[h].tag, h, H[h].usedby_f); */
	  /* printf ("in .usedby[] (tweet %d)\n", i); */
	  if ( add(T[i].author, H[h].usedby, &H[h].usedby_f, M) )
	    ERR("usedby[] full!");
	}
    }
  }
  else if (T[i].nhash < 0) {
      ERR("Non ho controllato gli hashtag.");
  }

  /*
   * ...e riempio .at[] con gli
   * eventuali @utenti ( archi in U )
   */
  if(T[i].udest > 0) {
    int u;
    for (int n = 0; n < T[i].udest; n++) {
	u = T[i].dest[n];
	int check = add( u, U[T[i].author].at, &U[T[i].author].at_f, dim_U);
	if ( check ) {		// array pieno
	  //stampa_at(T[i].author, U);
	  ERR("at[] full!");
	}
	/* printf ("%s nomina %d utenti ",U[T[i].author].screen_name, U[T[i].author].at_f); */
    }
  }


  return 0;
}
