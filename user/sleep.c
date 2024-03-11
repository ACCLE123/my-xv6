#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(2, "argument error!\n");
    exit(1);
  }

  int wait_time = atoi(argv[1]);
  sleep(wait_time);
  exit(0);
}
