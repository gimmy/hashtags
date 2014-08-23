/* def.h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NTWEET 10000		/* numero di Tweet da leggere */
#define NHASH NTWEET/10		/* uso NTWEET come bound */
#define NUSER NTWEET/2

#define L 100
#define M 1000
#define DIM 50
#define LEN 140*2		// TODO: unparsed unicode make text larger!
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(2); }

#define MAX_LENGTH 512
#define MAX_STRING 1024

typedef struct {
  char name[DIM];
  char screen_name[DIM];
  int cip[L];			/* elenco Tweet dell'utente */
  int cip_f;			// prima posizione libera in cip
  int at[NUSER];		/* @utenti adiacenti */
  int at_f;			// prima posizione libera in at
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
  int occur_f;			// prima posizione libera in occur
  int usedby[DIM];
  int usedby_f;
  int impl[L];			/* Occorrenze implicite */
  int impl_f;
} Hashtag;

Tweet* T; Hashtag* H; User* U;

/* Function prototype */

/* Int array */
int cerca(int x, int* array, int N);
int add(int x, int* a, int* free_p, int len_a);

/* Stampe */
void stampa_utente(int id, User* U);
void stampa_at(int iduser, User* U);
void stampa_tweet(int id, Tweet* T, User* U, Hashtag* H);

/* Hashtag array */
int cerca_hash(char* parola, Hashtag* H);
int inserisci_hash(char* hashtag, int idtweet, Hashtag* H, int* position);

/* User array */
int cerca_user(char* utente, User* U);
int inserisci_user(char* sname, int idtweet, User* U, int* position, int u);

/* Parser */
void ScanHash(char* aux, Tweet* t, int idtweet, int h, Hashtag* H, int* pl);
void ScanUser(char* aux, Tweet* t, int idtweet, int u, User* U, int* pm);
int ParseTweet(char* js, Tweet* T, int i, Hashtag* H, int* pl, User* U, int* pm);


/* Implicit hashtag */
void search_w(char* parola, char* text, int idhash, int idtweet, Hashtag* H, int* himpl);
void lookup_implicit_hash(int hash, Tweet* T, Hashtag* H, User* U);
