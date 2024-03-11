#include "kernel/fd_types.h"
#include "kernel/types.h"
#include "user/user.h"
enum PIPE_END { REC = 0, SND = 1 };

int main(int argc, char* argv[]) {
  if (argc != 1) {
    fprintf(STDERR, "usage pingpong(no parameter)\n");
    exit(1);
  }

  int p[2];
  pipe(p);

  int cur_pid = fork();
  if (cur_pid == 0) {
    char buf[20];
    if (read(p[REC], buf, sizeof buf)) {
      fprintf(STDOUT, "%d: received pong\n", getpid());
    }
    write(p[SND], "child", 5);
    exit(0);
  } else if (cur_pid > 0) {
    char buf[20];
    write(p[SND], "parent", 6);
    wait(0);
    if (read(p[REC], buf, sizeof buf)) {
      printf("%d: received ping\n", getpid(), buf);
    }
    exit(0);
  } else {
    fprintf(STDERR, "fork error\n");
    exit(1);
  }
}
