/*----------------------------------------------------------------*/
#define PRINT
#ifdef PRINT
#define Print(x) printf x
#else
#define Print(x)
#endif
/*----------------------------------------------------------------*/
/*deklaracje dla przykladu wykorzystujacego kolejki */
#define BUFSIZE (sizeof(char) * 100)
typedef struct {
  long typ;
  char tekst[BUFSIZE];
} msgbufor;
/*---------------------------------------------------------------*/
/*deklaracje dla przykladu wykorzystujacego semafory i pamiec 
wspoldzielona*/
#define A2B 0
#define B2A 1
