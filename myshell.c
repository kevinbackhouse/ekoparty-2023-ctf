#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  char **newargv = (char **)malloc((argc + 6) * sizeof(char *));
  size_t i = 0;
  char arg0[] = "/usr/bin/docker";
  char arg1[] = "run";
  char arg2[] = "--rm";
  char arg3a[] = "-it";
  char arg3b[] = "-i";
  char arg4[] = "ekoparty-2023-ctf";
  char arg5[] = "/usr/local/bin/git-shell";
  if (!newargv) {
    exit(1);
  }
  newargv[i++] = arg0;
  newargv[i++] = arg1;
  newargv[i++] = arg2;
  if (isatty(STDIN_FILENO)) {
    newargv[i++] = arg3a;
  } else {
    newargv[i++] = arg3b;
  }
  newargv[i++] = arg4;
  newargv[i++] = arg5;
  memcpy(&newargv[i], &argv[1], argc * sizeof(char *));
  if (execv(newargv[0], newargv) < 0) {
    const int err = errno;
    fprintf(stderr, "%s\n", strerror(err));
  }
  free(newargv);
  return 0;
}
