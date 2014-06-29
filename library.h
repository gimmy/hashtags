#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

char* get_text_from_file(char* input); 
int get_tweet_number(FILE* fp);


/* Return number of tweet (line) from text file */
int get_tweet_number(FILE* fp) {
  int total_tweet;

  /* ---------- */
  char* line;
  size_t  len = 0;
  size_t read;

  int lines = 0;

  if( fp != NULL ) {
    while ((read = getline(&line, &len, fp)) != -1){
      lines ++;
      // printf("%s\n", line);
    }
  }
  printf("number of lines : %d\n", lines);
  /* ---------- */

  /* total_tweet = countSubstring(text, "'lang'"); */
  total_tweet = lines;
  return total_tweet;  
}

/* Return text from file in input */
char* get_text_from_file(char* input) {  
  char* text;
  FILE* fd;
  struct stat stat_buf;
  int fileno( FILE * );

  /* open the text file (read and write) */
  fd = fopen( input, "r+w" );
  if ( fd == NULL ) 
    ERR( "Open failed. Give text file in input." );

  /* get text length in bytes */
  if ( fstat(fileno(fd), &stat_buf) == -1 ) 
    ERR("stat failed");
  int n = stat_buf.st_size;		/* get file size */
  
  /* mmap the open text to a local variable */
  text = (char *)mmap(NULL, n, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(fd), 0);
  if( text == NULL ) 
    ERR("mmap failed");

  fclose( fd );

  return text;
}
