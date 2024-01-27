#include <assert.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
/*----------------------------------------------------------------*/
#include "params.h"
/*----------------------------------------------------------------*/
int main(void){
  int klucz;    /*klucz umozliwiajacy dostep do tablicy semaforow */
                /*i pamieci wspoldzielonej */
  int semafor,  /*identyfikator tablicy semaforow*/
      pamiec;   /*identyfikator segmentu pamieci wspoldzielonej*/
  char * pamwsk;  /*wskaznik umozliwiajacy odwolania do pamieci wspoldzielonej*/
  struct sembuf op[1]; /*struktura pomocnicza wykorzystywana przy operacjach */
		       /*semaforowych*/

  Print (("Start B\n"));
  /*utworzenie klucza*/
  klucz = ftok("semafor", 'c');
  assert (klucz != -1);
  /*uzyskanie dostepu do segmentu pamieci wspoldzielonej*/
  pamiec =  shmget (klucz, BUFSIZE, IPC_CREAT| 0600);
  assert (pamiec != -1);
  /*przylaczenie segmentu pamieci wspoldzielonej do przestrzeni adresowej */
  /*bierzacego procesu*/
  pamwsk = (char *) shmat (pamiec, 0, SHM_RND);
  assert ( (int)pamwsk != -1);
  /*uzyskanie dostepu do tablicy semaforow*/
  assert ( (semafor = semget (klucz, 2, IPC_CREAT | 0600)) != -1);
  /*oczekiwanie na otrzymanie wiadomosci od procesu A*/
  op[0].sem_num = B2A; /*operacja bedzie dotyczyc semafora o numerze B2A*/ 
  op[0].sem_op = -1;   /*jednokrotne wykonanie operacji P*/
  op[0].sem_flg = 0;   /*operacja blokujaca*/
  assert ( semop(semafor, op, 1) == 0);
  Print (("Tekst otrzymany od A: %s\n", pamwsk));
  Print (("Wyslanie tekstu z B \n"));
  /*skopiowanie wiadomosci do segmentu pamieci wspoldzielonej*/
  strcpy (pamwsk, "I co z tego?");
  /*zasygnalizowanie procesowi A obecnosci danych w pamieci wspoldzielonej*/
  op[0].sem_num = A2B; /*operacja bedzie dotyczyc semafora o numerze A2B*/ 
  op[0].sem_op = 1;    /*jednokrotne wykonanie operacji V*/
  op[0].sem_flg = 0;   /*operacja blokujaca*/
  /*odlaczenie obszaru pamieci wspoldzielonej*/
  assert (shmdt (pamwsk) == 0);
  /*zasygnalizowanie procesowi A obecnosci daych w pamieci wspoldzielonej*/
  /*proces A zwolni obszar pamieci wspoldzielonej wykorzystywanej */
  /*do komunikacji miedzy procesami A i B */
  assert (semop(semafor, op, 1) == 0);
  return 0;
}
/*----------------------------------------------------------------*/
