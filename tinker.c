#include <stdlib.h>
#include <stdio.h> 
#include <signal.h>
#include <unistd.h> // for sleep function
#include <time.h> // for filename generation

int fill_disk (int file_count, int size_limit) {
  char *filename;

  for (int i = 0; i < file_count; i++) {
    filename = calloc (128, sizeof (char));
    sprintf (filename, "%d.%d", time (NULL), i);
    printf ("will create file %s\n", filename);
  }

  return 0;
}

int claim_ram (int limit) {
  void *cp;
  int cnt = 0;

  for (cnt = 0; cnt < limit; cnt++) {
    cp = calloc (1, 1024);
    if (cp == NULL) {
      printf ("Error occurred at %d KB\n", cnt);
      return 1;
    }
  }

  return cnt;
}

int kill_process (int pid) {
  return kill (pid, SIGKILL);
}

int
main (int argc, char** argv) {
  fill_disk (3, 1024);

  int mem = claim_ram (12 * 1024);
  printf ("Claimed %d KB ram\n", mem);

  if (argc > 1) {
    int result = kill_process (atoi (argv[1]));
    printf ("Kill resulted in %d\n", result);
  }

  return 0;
}
