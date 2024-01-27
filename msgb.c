#include <sys/types.h>
#include <assert.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
/*-------------------------------------------------------------------*/
#include "params.h"
/*-------------------------------------------------------------------*/
int main(void){
  int klucz;    /*klucz umozliwiajacy dostep do kolejki komunikatow*/
  int kolejka;  /*identyfikator kolejki komunikatow*/
  msgbufor bufor; /*struktura pomocnicza wykorzystywana */
	                /*przy przesylaniu wiadomosci*/
  int kod;        /*zmienna pomocnicza przechowujaca kody wykonania operacji*/

  Print (("Start B\n"));
  /*utworzenie klucza kolejki*/
  klucz = ftok("kolejka", 'c');
  assert (klucz != -1);
  /*uzyskanie dostepu do kolejki */
  kolejka = msgget (klucz, IPC_CREAT | 0600);
  assert (kolejka != -1);
  /*pobieranie wiadomosci z kolejki*/
  kod = msgrcv (kolejka, &bufor, BUFSIZE, 1L, 0);
  assert (kod != -1);
  Print (("Tekst odebrany od A: %s\n", bufor.tekst));
  /*wyslanie odpowiedzi do A*/
  Print (("Wysylanie odpowiedzi do A\n"));
  bufor.typ = 2L; /*okreslenie typu wiadomosci*/
  strcpy (bufor.tekst, "I co z tego?"); /*okreslenie tresci wiadomosci*/
	/*wstawienie wiadomosci do kolejki*/
  kod = msgsnd (kolejka, &bufor, BUFSIZE, 0);
  assert (kod == 0);
  return 0;
}
/*-------------------------------------------------------------------*/
