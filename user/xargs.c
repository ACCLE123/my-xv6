#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fd_types.h"
#include "kernel/param.h"

const int MAX_LENGTH = 1024;

int main(int argc, char* argv[]) {
  
  if (argc < 2) {
    fprintf(STDERR, "xargs error\n");
    exit(1);
  }
  
  char* args[MAXARG];
  for (int i = 1; i < argc; i++)
    args[i - 1] = argv[i];
  
  int n;
  char buf;
  char arg[MAX_LENGTH];
  int length = 0;
  int pid;


  while ((n = read(STDIN, &buf, 1)) > 0) {
    if (buf == '\n') {
      arg[length] = '\0';
      
      if ((pid = fork()) == 0) {
        wait(0);
        length = 0;
      } else {
        args[argc - 1] = arg;
        args[argc] = 0;
        exec(args[0], args);
      }

    } else {
      arg[length++] = buf;
    }
  }

  exit(0);
}
