/* Tweet structure */

#define DIM 50
#define LEN 140
#define ERR(msg) { fprintf(stderr, "%s\n", msg); exit(2); }

typedef struct {
  char name[DIM];
  char screen_name[DIM];
} User;

typedef struct {
  char text[LEN];
  User author;
} Tweet;
