#include "kernel/fd_types.h"
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/dbg_macros.h"
const int MAX_P = 35;
// #define FDEBUG
enum PIPE_END { REC = 0, SND = 1 };
void send_to_next(int outpp, int msg[], int msg_len) {
    //发送到下一个子进程
    for (int i = 0; i < msg_len; i++) {
        write(outpp, msg + i, sizeof(int));
    }
}
int* filter(int num, int inpp, int* outlen) {
    //把 inpp 管道中的 num 倍数全部过滤掉，返回过滤好的数组（没有 num 的倍数）
    (*outlen) = 0;
    //len 是过滤之后还有多少个数字
    int* out = (int *)malloc(MAX_P * sizeof(int));
    int ret = 0;
    do {
        ret = read(inpp, out + (*outlen), sizeof(int));
        // ret 返回读到了多少字节
        if (out[(*outlen)] % num != 0 && ret > 0) {
            (*outlen)++;
        }
    } while (ret > 0);
    return out;
}   

void child_proc(int pp[2]) { 
    int prime;
    int len = read(pp[REC], &prime, sizeof(int));   
    if(len == 0){
        exit(0);
        return;
    }

    int child_pp[2];
    pipe(child_pp);

    printf("prime %d\n", prime);
    int outlen;
    int* filtered = filter(prime, pp[REC], &outlen);
    close(pp[REC]);

    int pid = fork();
    if(pid == 0){
        close(child_pp[SND]);
        child_proc(child_pp);
    } else {
        close(child_pp[REC]);
        send_to_next(child_pp[SND], filtered, outlen);
        close(child_pp[SND]);
        wait(0); // wait 可以释放子进程的进程号以及别的资源
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    int pp[2];
    pipe(pp);
    int pid;
    pid = fork();
    if (pid == 0) {
        close(pp[SND]);
        child_proc(pp);
    } else {
        int init_num[MAX_P];
        int idx = 0;
        for(int i = 2; i <= MAX_P; i++){
            init_num[idx++] = i;
        }
        close(pp[REC]);
        send_to_next(pp[SND], init_num, idx);
        close(pp[SND]);
        wait(0);
    }
    exit(0);
}
