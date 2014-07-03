/* tweet_filter.c - Progetto Algoritmi e Strutture di Dati */

/* Rende i caratteri minuscoli */
int tolower(int);

void delete_char(char *src, char c, int len);

void InTrie(char* text, trie_nodo* radice) {
  int len = 0;

  len = strlen(text);

  char content[len]; // potrei già passare lui?
  memcpy(content, &text[0], len);
  content[len] = '\0';

  /* Spezzo il testo in parole */ 
  char* word;
  char* rmchar;
  word = strtok (content, " ,;:!?");
  while (word != NULL) {
    rmchar = NULL;
    /* Filtro parole "significative" */
    len = strlen(word);
    if( len > 3 ) {		// controllo lunghezza parola
      rmchar = strstr(word, "#");
      if( rmchar != NULL )	// rimuovo # e @
	delete_char(word, '#', len);   

      rmchar = strstr(word, "@");
      if( rmchar != NULL )
	delete_char(word, '@', len); 

      rmchar = strstr(word, "\""); // rimuovo "
      if( rmchar != NULL )
	delete_char(word, '"', len);    

      rmchar = strstr(word, ".");
      if( rmchar != NULL )
	delete_char(word, '.', len); 

      rmchar = strstr(word, "\n");
      if( rmchar != NULL )
	delete_char(word, '\n', len);    

      rmchar = strstr(word, "http");
      if( rmchar != NULL ) {
	printf ("skip link: %s ",word);
	//delete_char(word, '\n', len);    
      }

      /* TODO: check UTF-8 */

      /* rmchar = strstr(word, "\\"); // rimuovo \ */
      /* if( rmchar != NULL ) */
      /* 	delete_char(word, '\\', len);  */

      //rmchar = strstr(word, "/");
      /* if( rmchar != NULL ) */
      /* 	delete_char(word, '\n', len);     */

      rmchar = strstr(word, "\\");
      if( rmchar == NULL ) {
	rmchar = strstr(word, "/");	
	if( rmchar == NULL ) {
	  char s[len];
	  memcpy(s, &word[0], len);
	  s[len] = '\0';

	  for (int i=0 ; i<len; i++) s[i] = tolower(s[i]);

	  printf("insert: %s   \t  (from: %s) \n", s, word);

	  // Aggiungo nel Trie
	  inserisci(radice, s);
	}
      }
    }

    //printf ("[%s]", word);
    word = strtok (NULL, " ,:.;");

  }
}


/**
 * Delete all occurrences of a character from a string
 *
 * The len parameter allows the function to work with non null terminated
 * strings and could also be considered a weak attempt at buffer overflow
 * protection.
 *
 * @note The original string is modified.
 *
 * @param[in] string   Pointer to string from which to remove characters
 * @param[in] c	       Character to remove
 * @param[in] len      Max number of characters to process
 */
void delete_char(char* string, char c, int len) {
  char* pstring;
  int i;

  // Do not remove NULL characters.
  if ( c == 0 )
    return;

  // Small attempt to control a buffer overflow if the
  // the string is not null-terminated and a proper length
  // is not specified.
  if ( len <= 0 )
    len = MAX_STRING;

  pstring = string;

  for ( i=0; i<len && *string != 0; i++, string++ ) {
    if ( *string != c )
      *pstring++ = *string;
  }
  // Ensure the string is null-terminated.
  *pstring = 0;

  return;
}
