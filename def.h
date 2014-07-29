/* def.h */

#define DIM 50
#define LEN 140
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(2); }

#define MAX_LENGTH 512
#define MAX_STRING 1024

/* Tweet structure */

typedef struct {
  char name[DIM];
  char screen_name[DIM];
} User;

typedef struct {
  char tag[DIM];
} Hash;

typedef struct {
  char text[LEN];
  User author;
  int udest;
  User dest[10];
  int nhash;
  Hash hash[10];
} Tweet;

typedef struct {
  char tag[DIM];
  int occur[DIM];
  int free; // prima posizione libera in occur
} Hashtag;


void stampa_tweet(int id, Tweet* T) {
  printf ("\n Tweet[%d] %s (%s) scrive:\n %s\n",id, \
	  T[id].author.name,T[id].author.screen_name, T[id].text);

  /* Print Users */
  int u = 0;
  while (T[id].udest != 0 && u < T[id].udest) {
    printf (" @user[%d] : %s (%s)\n",u,T[id].dest[u].name, T[id].dest[u].screen_name);
    u++;
  }

  /* /\* Print Hashtags *\/ */
  /* int h = 0; */
  /* while (T[id].nhash > 0 && h < T[id].nhash) { */
  /*   printf (" #hash[%d] : %s \n", h, T[id].hash[h].tag); */
  /*   h++; */
  /* } */

}
