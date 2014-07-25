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
