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

Tweet ParseTweet(char* js) {
  Tweet Tw;

  int result;			/* inizializzo parser */
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
    for ( int j = 1; (tokens[j].end <= EndTweet) && !stop; j++ ) { 

      if (tokens[j].type == JSMN_STRING || tokens[j].type == JSMN_PRIMITIVE) {		  

	if ( TOKEN_STRING(js, tokens[j], "text") ) {
	  j = j+1; 
	  /* Salvo testo in Tweet[i].text */
	  length = tokens[j].end - tokens[j].start;
	  //strncpy(Tw.text, &js[tokens[j].start], length);
	  memcpy(Tw.text, &js[tokens[j].start], length);
	  Tw.text[length] = '\0';
	}
	else if ( TOKEN_STRING(js, tokens[j], "screen_name") ) {
	  j = j+1;
	  /* Salvo screen_name in Tweet[i].author.screen_name */
	  length = tokens[j].end - tokens[j].start;
	  memcpy(Tw.author.screen_name, &js[tokens[j].start], length);
	  Tw.author.screen_name[length] = '\0';
	}
	else if ( TOKEN_STRING(js, tokens[j], "name") ) {
	  j = j+1;
	  /* Salvo name in Tweet[i].author.name */
	  length = tokens[j].end - tokens[j].start;
	  memcpy(Tw.author.name, &js[tokens[j].start], length);
	  Tw.author.name[length] = '\0';
	  stop = 1; // ho salvato tutto. Esco e ritorno il Tweet
	}
      }
    }
  }
  return Tw;
}
