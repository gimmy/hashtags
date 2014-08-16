/* def.h */

#define DIM 50
#define LEN 140
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(2); }

#define MAX_LENGTH 512
#define MAX_STRING 1024


typedef struct {
  char name[DIM];
  char screen_name[DIM];
  int cip[DIM]; 		/* elenco Tweet dell'utente */
  int free;			// prima posizione libera in cip
} User;

typedef struct {
  char text[LEN];
  int author;
  int udest;
  int dest[10];
  int nhash;
  int hash[10];
} Tweet;

typedef struct {
  char tag[DIM];
  int occur[DIM]; 		/* Tweet in cui compare l'hashtag */
  int free;			// prima posizione libera in occur
} Hashtag;


void stampa_tweet(int id, Tweet* T, User* U) {

  int a = T[id].author;

  printf ("\n Tweet[%d] %s (%s) scrive:\n %s\n",id, \
	  U[a].name, U[a].screen_name, T[id].text);

  /* Print Users */
  int u = 0;
  while (T[id].udest != 0 && u < T[id].udest) {
    printf (" @user[%d] : %s (%s)\n",u, \
	    U[T[id].dest[u]].name, U[T[id].dest[u]].screen_name);
    u++;
  }

}
