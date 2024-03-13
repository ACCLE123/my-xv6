#include "kernel/fd_types.h"
#include "kernel/types.h"
#include "user/user.h"
enum PIPE_END { REC = 0, SND = 1 };

int main(int argc, char* argv[]) {
  if (argc != 1) {
    fprintf(STDERR, "usage pingpong(no parameter)\n");
    exit(1);
  }
  
  int parent_fd[2];
  int child_fd[2];
  char buf[20];

  pipe(child_fd);
  pipe(parent_fd);

  int pid;
  if ((pid = fork()) == 0) {
    close(parent_fd[SND]);
    close(child_fd[REC]);

    read(parent_fd[REC], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    write(child_fd[SND], "pong", 4);

    exit(0);
  
  } else {
    close(parent_fd[REC]);
    close(child_fd[SND]);

    write(parent_fd[SND], "ping", 4);
    read(child_fd[0], buf, 4);
    printf("%d: received %s\n", getpid(), buf);
    
    wait(0);
    exit(0);
  }
}
