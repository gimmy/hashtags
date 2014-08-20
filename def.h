/* def.h */

#define L 100
#define DIM 50
#define LEN 140*2		// TODO: unparsed unicode make text larger!
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(2); }

#define MAX_LENGTH 512
#define MAX_STRING 1024

typedef struct {
  char name[DIM];
  char screen_name[DIM];
  int cip[L];			/* elenco Tweet dell'utente */
  int free;			// prima posizione libera in cip
  int at[L];			/* @utenti adiacenti */
  int at_free;			// prima posizione libera in at
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
  int occur[L]; 		/* Tweet in cui compare l'hashtag */
  int free;			// prima posizione libera in occur
} Hashtag;


void stampa_tweet(int id, Tweet* T, User* U) {

  int a = T[id].author;

  printf ("\n Tweet[%d] %s (%s) scrive:\n %s\n",id, \
	  U[a].name, U[a].screen_name, T[id].text);

  /* Print Users */
  if ( T[id].udest != 0 ) {
    if ( T[id].udest > 10 || T[id].udest < 0 )
      {
	printf ("udest anomalo = %d\n", T[id].udest );
	assert (T[id].udest <= 10); // evito overflow    
      }
    int u = 0;
    while (u < T[id].udest) {
      printf (" @user[%d] : %s (%s)\n",u, \
	      U[T[id].dest[u]].name, U[T[id].dest[u]].screen_name);
      u++;
    }    
  }

}
