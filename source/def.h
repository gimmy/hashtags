/* def.h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NTWEET 100000		/* numero di Tweet da leggere */
/* using dim_H and dim_U for dynamic array */
#define NHASH 5000		/* uso NTWEET come bound */
#define NUSER 14000

#define CIP 600
#define L 100
#define M NUSER
#define DIM 50
#define LEN 280
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(2); }

#define MAX_LENGTH 512
#define MAX_STRING 1024

typedef struct {
  char name[DIM];
  char screen_name[DIM];
  int cip[CIP];			/* elenco Tweet dell'utente */
  int cip_f;			// prima posizione libera in cip
  int at[NUSER];		/* @utenti adiacenti */
  int at_f;			// prima posizione libera in at
  int hash_check[L];		// hashtag impliciti ricercati nei tweet dell'utente
  int hash_check_f;
} User;

typedef struct {
  char text[LEN];
  int author;
  int udest;
  int dest[20];
  int nhash;
  int hash[20];
} Tweet;

typedef struct {
  char tag[DIM];
  int occur[L]; 		/* Tweet in cui compare l'hashtag */
  int occur_f;			// prima posizione libera in occur
  int usedby[M];
  int usedby_f;
  int impl[L];			/* Occorrenze implicite */
  int impl_f;
} Hashtag;

Tweet* T; Hashtag* H; User* U;

/* Function prototype */

/* convert a string "src" containing escape sequences to UTF-8 */
int u8_unescape(char *buf, int sz, char *src);

/* Int array */
int cerca(int x, int* array, int N);
int add(int x, int* a, int* free_p, int len_a);

/* Dynamic array */
void verifica_raddoppio_H( Hashtag* H, int* dim_H, int p );

/* Stampe */
void stampa_utente(int id, User* U);
void stampa_at(int iduser, User* U);
void stampa_tweet(int id, Tweet* T, User* U, Hashtag* H);
void stampa_himpl(int id, Tweet* T, User* U, Hashtag* H);

/* Hashtag array */
int cerca_hash(char* parola, Hashtag* H, int dim_H, int until);
int inserisci_hash(char* hashtag, int idtweet, Hashtag* H, int dim_H, int* position);

/* User array */
int cerca_user(char* utente, User* U, int dim_U, int until);
int inserisci_user(char* sname, int idtweet, User* U, int dim_U, int* position, int u);

/* Parser */
void check_result(int r, int s);
void ScanHash(char* aux, Tweet* t, int idtweet, int h, Hashtag* H, int dim_H, int* pl);
void ScanUser(char* aux, Tweet* t, int idtweet, int u, User* U, int dim_U, int* pm);
int ParseTweet(char* js, Tweet* T, int i, Hashtag* H, int dim_H, int* pl, User* U, int dim_U, int* pm);
int tolower(int);

/* Implicit hashtag */
void search_w(char* parola, char* text, int idhash, int idtweet, Hashtag* H, int* himpl);
void lookup_implicit_hash(int hash, Tweet* T, Hashtag* H, User* U);
