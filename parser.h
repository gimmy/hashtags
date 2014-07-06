/* parser.h
   Parser line and return a Tweet */

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


      for ( int j = 1; (tokens[j].end <= end); j++ ) { 

	if (tokens[j].type == JSMN_STRING || tokens[j].type == JSMN_PRIMITIVE) {		  
	  /* Grep User info */
	  if ( TOKEN_STRING(aux, tokens[j], "screen_name") ) {
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
	  }
	  else if ( TOKEN_STRING(aux, tokens[j], "name") ) {
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
	  }
	}	
      }
}

Tweet ParseTweet(char* js) {
  Tweet Tw;
  Tweet* pTw = &Tw;	// assegno il puntatore

  int result;		/* inizializzo parser */
  jsmn_parser parser;
  jsmntok_t tokens[256];

  jsmn_init(&parser);
  result = jsmn_parse(&parser, js, strlen(js), tokens, 256);

  unsigned int length = 0;
  int stop = 0;

  if (tokens[0].type == JSMN_OBJECT) {
    int EndTweet = tokens[0].end; // lunghezza del Tweet
#ifdef DEBUG
    printf("Tweet: {%d elems}", tokens[0].size);
    TOKEN_PRINT(tokens[0]);
#endif    
    // finché non arrivo in fondo al Tweet
    for ( int j = 1; (tokens[j].end <= EndTweet) && !stop; j++ ) { // remove stop TODO

      if (tokens[j].type == JSMN_STRING || tokens[j].type == JSMN_PRIMITIVE) {		  

	if ( TOKEN_STRING(js, tokens[j], "text") ) {
	  j = j+1; 
	  /* Salvo testo in Tweet[i].text */
	  length = tokens[j].end - tokens[j].start;
	  memcpy(Tw.text, &js[tokens[j].start], length);
	  Tw.text[length] = '\0';
#ifdef DEBUG
	  printf ("Testo salvato: %s\n",Tw.text);
#endif
	}
	/* Grep info from Tweet */
	else if ( TOKEN_STRING(js, tokens[j], "user_mentions") ) {
	  if ( (tokens[j+1].type == JSMN_ARRAY) && (tokens[j+1].size > 0) ) {
	    j++;
	    #ifdef DEBUG
	    printf("@users: [%d elems]\n", tokens[j].size); 
	    #endif
	    int dest_users = tokens[j].size;
	    Tw.udest = dest_users; // aggiorno numero di destinatari
	    length = tokens[j].end - tokens[j].start;
	    char aux[length];
	    memcpy(aux, &js[tokens[j].start], length);
	    //aux[length] = '\0';
	    for (int u = 1; u <= dest_users; u++)	      
	      ScanUser(aux, pTw, u); 
	      
	  }
#ifdef DEBUG
	  else {
	    printf ("No user_mentions\n");
	    Tw.udest = 0; // aggiorno numero di destinatari
	  }
#endif
	}
	else if ( TOKEN_STRING(js, tokens[j], "user") ) {
	  j++;
	  length = tokens[j].end - tokens[j].start;
	  char aux[length];
	  memcpy(aux, &js[tokens[j].start], length);
	  //aux[length] = '\0';
	  ScanUser(aux, pTw, 0); 
	}
      }
    }
  }
  return Tw;
}
