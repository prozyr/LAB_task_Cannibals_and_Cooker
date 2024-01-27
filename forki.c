// forki.c

#include <stdio.h>
#include <assert.h>

int main()
{
  int pid, i, status;

  for (i = 0; i < 5; i++)
  {
    pid = 0;
    pid = fork();

    printf("powolano potomka %d, PID = %d\n", i, pid);

    assert(pid != -1);
    if (pid == 0)
    {
      printf("potomek %d\n", i);

      exit(0);
    }
  }

  for (i = 0; i < 5; i++)
  {
     printf("czekam na potomka ");
     pid = wait(&status);
     printf("PID = %d, status = %d\n", pid, status);
  }

  return 0;
}