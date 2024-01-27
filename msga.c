#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
/*----------------------------------------------------------------*/
#include "params.h"
/*----------------------------------------------------------------*/
int main(void){
  int klucz;    /*klucz umozliwiajacy dostep do kolejki komunikatow*/
  int kolejka;  /*identyfikator kolejki komunikatow*/
  msgbufor bufor; /*struktura pomocnicza wykorzystywana */
	                /*przy przesylaniu wiadomosci*/

  Print (("Start A\n"));
  /*utworzenie klucza kolejki*/
  assert ( (klucz = ftok("kolejka", 'c')) != -1);
  /*uzyskanie dostepu do kolejki*/
  assert ( (kolejka = msgget (klucz, IPC_CREAT| 0600)) != -1);
  /*przeslanie wiadomosci do procesu B*/
  Print (("Wyslanie tekstu do B\n"));
  bufor.typ = 1L;  /*okreslenie typu wiadomosci*/
  strcpy (bufor.tekst, "ALA MA KOTA"); /*okreslenie tresci wiadomosci*/
	/*wstawienie wiadomosci do kolejki*/
  assert ( msgsnd (kolejka, &bufor, BUFSIZE, 0) == 0);
  Print (("Odbieranie tekstu od B\n"));
  /*pobieranie wiadomosci z kolejki*/
  assert ( msgrcv (kolejka, &bufor, BUFSIZE, 2L, 0) != -1);
  Print (("Tekst odebrany od B: %s\n", bufor.tekst));
  /*usuniecie kolejki*/
  assert ( msgctl (kolejka, IPC_RMID, NULL) == 0);
  return 0;
}
/*----------------------------------------------------------------*/
