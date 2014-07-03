#ifndef TRIE_H
#define TRIE_H

#define TRIE_MAX_DEPTH 512

static int const N = 123; // invece di 256 o 123

typedef struct trie_nodo {
  void* valore;
  struct trie_nodo* figlio[N];
  /**
   * Numbers 0-9 ASCII 48 to 57
   * Lowercase letters a-z ASCII 97 to 122
   * Uppercase letters A-Z ASCII 65-90  
   **/
} trie_nodo;

trie_nodo* crea_nodo()
{
    return (trie_nodo*)malloc(sizeof(trie_nodo));
}

trie_nodo* inserisci(trie_nodo* n, char* s)
{
    char c = *s;
    if (!n->figlio[c]) {
        n->figlio[c] = crea_nodo();
	//printf ("creato nodo per %c [%d]\n",c, (int)c);
    }

    if (*s != 0) {
        return inserisci(n->figlio[c], s + 1);
    } else {
        return n->figlio[c];
    }
}

trie_nodo* cerca(trie_nodo* tn, char* s, int prefisso) // prefisso b0o1
{
    char c = *s;
    if (c == 0) {
        if (prefisso) {
            return tn;
        } else {
            return tn->figlio[c];
        }
    }

    if (tn->figlio[c])
        return cerca(tn->figlio[c], s + 1, prefisso);
    else
      return NULL;    
}

void stampa_stringhe_visita(trie_nodo* tn, char* stringa, int profondita)
{
    for (int i = 0; i < N; ++i) {
        if (tn->figlio[i]) {
            stringa[profondita] = i;
	    //printf ("%c",(char)i);
            if (i == 0)
	      printf(" trovata: %s\n", stringa);
            else
	      stampa_stringhe_visita(tn->figlio[i], stringa, profondita + 1);
            
	}
    }
}

int conta_nodi(trie_nodo* n) {
    if (n == NULL) return 0;    

    int nodi = 1; // nodo corrente
    for (int i = 97; i < N; ++i) {
      if(n->figlio[i]) {
	//printf ("Nodo %c [%d]\n", (char)i, i);
        nodi += conta_nodi(n->figlio[i]);
      }
    }

    return nodi;
}

void stampa_stringhe(trie_nodo* radice)
{
    char stringa[TRIE_MAX_DEPTH];
    stampa_stringhe_visita(radice, stringa, 0);
}

#endif
