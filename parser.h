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
void ScanHash(char* aux, Tweet* t, int h) {
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
	    /* Save hash */
	    length = tokens[j].end - tokens[j].start;

	      memcpy(t->hash[h].tag, &aux[tokens[j].start], length);
	      t->hash[h].tag[length] = '\0';

	    done = 1; // salvato
	  }
      }
}

/* Get & save an User (author or mentioned) */
void ScanUser(char* aux, Tweet* t, int u) {
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
	    if (u == 0) {	// se users == 0 sto passando l'autore
	      memcpy(t->author.screen_name, &aux[tokens[j].start], length);
	      t->author.screen_name[length] = '\0';
	    }
	    else {
	      memcpy(t->dest[u-1].screen_name, &aux[tokens[j].start], length);
	      t->dest[u-1].screen_name[length] = '\0';
	    }
	    screen_name_done = 1; // screen_name salvato
	  }
	  else if ( !name_done && TOKEN_STRING(aux, tokens[j], "name") ) {
	    j = j+1;
	    /* Save name */
	    length = tokens[j].end - tokens[j].start;
	    if (u == 0){
	      memcpy(t->author.name, &aux[tokens[j].start], length);
	      t->author.name[length] = '\0';
	    }
	    else {
	      memcpy(t->dest[u-1].name, &aux[tokens[j].start], length);
	      t->dest[u-1].name[length] = '\0';
	    }
	    name_done = 1; // name salvato
	  }
      }
}


int SkipToken(jsmntok_t token) {
  int size = token.size;
  return size;
}

int ParseTweet(char* js, Tweet* pTw) {

  /* Set mentions and hashs for further check */
  pTw->udest = -1;
  pTw->nhash = -1;
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

	if ( (pTw->text[0] == 0) && TOKEN_STRING(js, tokens[j], "text") ) {
	  j = j+1; 
	  /* Salvo testo in T[i].text */
	  length = tokens[j].end - tokens[j].start;
	  memcpy(pTw->text, &js[tokens[j].start], length);
	  pTw->text[length] = '\0';
	}

	/** Grep info from Tweet **/

	/* User mentions */
	else if ( (pTw->udest < 0) && TOKEN_STRING(js, tokens[j], "user_mentions") ) {
	  if ( (tokens[j+1].type == JSMN_ARRAY) && (tokens[j+1].size > 0) ) {
	    j++;
	    #ifdef DEBUG
	    printf("@users: [%d elems]\n", tokens[j].size); 
	    #endif
	    int dest_users = tokens[j].size;
	    pTw->udest = dest_users; // aggiorno numero di destinatari
	    
	    j++; // entro nell'array utenti
	    int end_u =  tokens[j].end;

	    /* Salvo utenti menzionati */
	    for (int u = 1; u <= dest_users; u++) {	

	      end_u =  tokens[j].end;

	      length = tokens[j].end - tokens[j].start;
	      char aux[length];
	      memcpy(aux, &js[tokens[j].start], length);
	      aux[length] = '\0';
	      
	      ScanUser(aux, pTw, u);

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

	    pTw->udest = 0; // aggiorno numero di destinatari
	  }
	}

	/* Hashtags */
	else if ( (pTw->nhash < 0) && TOKEN_STRING(js, tokens[j], "hashtags") ) {
	  if ( (tokens[j+1].type == JSMN_ARRAY) && (tokens[j+1].size > 0) ) {
	    j++;
	    #ifdef DEBUG
	    printf("#hashtags: [%d elems]\n", tokens[j].size); 
	    #endif
	    pTw->nhash = tokens[j].size; // aggiorno numero di hashtags
	    
	    j++; // entro nell'array
	    int end_h =  tokens[j].end;

	    /* Salvo hashtags */
	    for (int h = 0; h < pTw->nhash; h++) {	

	      end_h =  tokens[j].end;

	      length = tokens[j].end - tokens[j].start;
	      char aux[length];
	      memcpy(aux, &js[tokens[j].start], length);
	      aux[length] = '\0';
	      
	      ScanHash(aux, pTw, h);

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

	    pTw->nhash = 0; // aggiorno numero hash
	  }
	}

	else if ( !(pTw->udest < 0) && TOKEN_STRING(js, tokens[j], "user") ) {
	  j++;
	  length = tokens[j].end - tokens[j].start;
	  char aux[length];
	  memcpy(aux, &js[tokens[j].start], length);
	  aux[length] = '\0';
	  ScanUser(aux, pTw, 0);
/* #ifdef DEBUG */
/*       printf ("\n %s (%s) scrive:\n %s\n\n", \ */
/* 	      pTw->author.name,pTw->author.screen_name,pTw->text); */
/* #endif */

	  stop = 1; // mi posso fermare
	}
      }
    }
  }
  return 0;
}
