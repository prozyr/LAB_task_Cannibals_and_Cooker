#include <sys/types.h>
#include <assert.h>
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
  int semafor,  /*identyfikator tablicy semaforow */
      pamiec;   /*identyfikator segmentu pamieci wspoldzielonej*/
  char * pamwsk;  /*wskaznik umozliwiajacy odwolania do pamieci wspoldzielonej*/
  struct sembuf op[1]; /*struktura pomocnicza wykorzystywana przy operacjach */
		       /*semaforowych */
  union semun{
    int val;
    struct semid_ds *bf;
    ushort *array;
  } val;
  
  Print (("Start A\n"));
  /*utworzenie klucza*/
  klucz = ftok("semafor", 'c');
  assert (klucz != -1);
  /*uzyskanie dostepu do segmentu pamieci wspoldzielonej */
  pamiec = shmget (klucz, BUFSIZE, IPC_CREAT| 0600);
  assert (pamiec != -1);   
  /*przylaczenie segmentu pamieci wspoldzielonej do przestrzeni adresowej */
  /*bierzacego procesu*/
  pamwsk = (char *) shmat (pamiec, 0, SHM_RND);
  assert ( (int)pamwsk != -1);
  /*uzyskanie dostepu do tablicy semaforow*/
  assert ( (semafor = semget (klucz, 2, IPC_CREAT | 0600)) != -1);
  /*inicjalizacja semaforow */  
  /*wartosci A2B i B2A zdefiniowane sa w pliku naglowkowym params.h*/
  val.val = 0;
  assert (semctl (semafor, A2B, SETVAL, val) != -1);
  assert (semctl (semafor, B2A, SETVAL, val) != -1);
  Print (("Wysylanie tekstu do B\n"));fflush(stdout);
  /*skopiowanie wiadomosci do segmentu pamieci wspoldzielonej*/
  strcpy (pamwsk, "ALA MA KOTA");
  /*zasygnalizowanie procesowi B obecnosci danych w pamieci wspoldzielonej*/
  op[0].sem_num = B2A; /*operacja bedzie dotyczyc semafora o numerze B2A*/
  op[0].sem_op = 1;    /*jednokrotne wykonanie operacji V*/
  op[0].sem_flg = 0;   /*operacja blokujaca*/
  assert ( semop(semafor, op, 1) == 0);
  Print (("Odbieranie tekstu od B\n"));
  /*oczekiwanie na otrzymanie wiadomosci od procesu B*/
  op[0].sem_num = A2B; /*operacja bedzie dotyczyc semafora o numerze A2B*/
  op[0].sem_op = -1;   /*jednokrotne wykonanie operacji P*/
  op[0].sem_flg = 0;   /*operacja blokujaca*/
  assert (semop(semafor, op, 1) == 0);
  Print (("Tekst odebrany od B: %s\n", pamwsk));
  /*odlaczenie segmentu pamieci wspoldzielonej*/
  assert (shmdt (pamwsk) == 0);
  /*zwolnienie segmentu pamieci wspoldzielonej*/
  assert (shmctl (pamiec, IPC_RMID, NULL) == 0);
  /*zwolnienie semaforow*/
  assert (semctl (semafor, 0, IPC_RMID, 0) != -1);
  return 0;
}
/*----------------------------------------------------------------*/
