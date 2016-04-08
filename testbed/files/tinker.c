#include <dirent.h>
#include <regex.h>
#include <stdlib.h>
#include <stdio.h> 
#include <signal.h>
#include <string.h>
#include <unistd.h> // for sleep function
#include <time.h> // for filename generation

int fill_disk (int file_count, int size_limit) {
  FILE *fp;
  char *filename;
  long i, j;
  char *dirs[] = {"/etc", "/tmp", "/var/log", "/var/run"};
  char *dir;

  srandom ((unsigned) time(NULL));

  for (i = 1; i != file_count; i++) {
    dir = dirs[4 * random () / RAND_MAX];
    filename = calloc (64 + strlen (dir), sizeof (char));
    sprintf (filename, "%s/%u.%ld", dir, (unsigned) time (NULL), i);
    fp = fopen (filename, "w");
    printf ("will create file %s, len %d\n", filename, (int)strlen (filename));
    for (j = 0; j < size_limit; j++) {
      fprintf (fp, "%c", (int) random ());
    }
    fclose (fp);
  }

  return 0;
}

long claim_ram (int limit) {
  void *cp;
  long cnt = 0;
  int MEM_ALLOC_SIZE = 4 * 1000 * 1024;

  for (cnt = 1; cnt != limit; cnt++) {
    cp = calloc (1, MEM_ALLOC_SIZE);
    if (cp == NULL) {
      printf ("Error occurred at %ld KB\n", cnt);
      return 1;
    }

    if (! ((cnt * MEM_ALLOC_SIZE) % (100 * MEM_ALLOC_SIZE))) {
      printf ("Allocated %ld bytes\n", cnt * MEM_ALLOC_SIZE);
    }
  }

  return cnt * MEM_ALLOC_SIZE;
}

int kill_process (int pid) {
  DIR *dir;
  struct dirent *ent;
  int pids[1000];
  int pcount = 0;
  int retval = 0;
  int result = 0;
  int i;
  
  regex_t regex;
  regcomp (&regex, "^[0-9]+$", REG_EXTENDED);
  srandom ((unsigned) time(NULL));

  if (pid > 0) {
    result = kill (pid, SIGKILL);
    printf ("killing %d\n", pid);
    return result;
  }

  if ((dir = opendir ("/proc")) != NULL) {
    while (((ent = readdir (dir)) != NULL) && pcount < (sizeof (pids)/sizeof(int))) {
      if (! (regexec (&regex, ent->d_name, 0, NULL, 0))) {
        pids[pcount++] = atoi (ent->d_name);
      }
    }
    closedir (dir);
  }

  for (i = 0; i < 8; i++) {
    pid = pids[((random () * pcount) / RAND_MAX)];
    result = kill (pid, SIGKILL);
    printf ("killing %d, result %d\n", pid, result);
    if (result > retval) { retval = result; }
  }

  return retval;
}

void run_test (int pid) {
  fill_disk (3, 1024);

  long mem = claim_ram (10);
  printf ("Claimed %ld KB ram\n", (mem/1024));

  if (pid != 0) {
    int result = kill_process (pid);
    printf ("Kill resulted in %d\n", result);
  }
}

int
main (int argc, char** argv) {
  if (argc > 1) {
    char* cmd = argv[1];
    if (strncmp (cmd, "t", 1) == 0) {
      run_test (atoi (argv[2]));
    }else
    if (strncmp (cmd, "p", 1) == 0) {
      kill_process (0);
    }else
    if (strncmp (cmd, "f", 1) == 0) {
      fill_disk (0, 4 * 1024 * 1024);
    }else
    if (strncmp (cmd, "m", 1) == 0) {
      claim_ram (0);
    }
  }else{
    run_test (0);
  }

  return 0;
}
