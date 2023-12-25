#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    // 创建 2 个管道
    int pipefd_1[2]; // parent -> child
    int pipefd_2[2]; // child  -> parent

    if (pipe(pipefd_1) == -1) // 创建管道1
    {
        printf("creating pipe1 failed...");
        exit(-1);
    }

    if (pipe(pipefd_2) == -1) // 创建管道2
    {
        printf("creating pipe2 failed...");
        exit(-1);
    }

    int pid = fork(); // 创建 child 进程
    if (pid > 0)
    {
        write(pipefd_1[1], "msg", 4); // 父进程首先发送数据
        char buf;
        read(pipefd_2[0], &buf, 4); // 然后接受子进程的数据
        printf("%d: received pong\n", getpid());
        wait(0);
    }
    else if (pid == 0)
    {
        char buf;
        read(pipefd_1[0], &buf, 4); // 子进程先接受数据
        printf("%d: received ping\n", getpid());
        write(pipefd_2[1], "msg", 4); // 再向父进程发送数据
        exit(0);
    }

    exit(0);
}