#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#define MAX_BUFFER_LEN 1024

#define handle_error_en(en, msg) \
           do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)


void * client_routine(void * argv)
{
    int fd = *(int*)argv;
    char buf[MAX_BUFFER_LEN] = {0};
    while (1)
    {
        memset(buf, 0, MAX_BUFFER_LEN);
        //同理 read函数也与accept函数类似
        int res = read(fd, buf, sizeof(buf));
        if (res < 0)
        {
            printf("remote client socket is closed.\n");
            break;
        }
        printf("recved user data : %s\n", buf);

        memset(buf, 0, MAX_BUFFER_LEN);

        strcpy(buf, "Hello Client");
        //同理 write函数也与accept函数类似
        res = write(fd, buf, sizeof(buf));
        printf("send data : %s\n", buf);
    }
    close(fd);
    return NULL;
}

int main(int argc, char *argv[])
{
    struct addrinfo hint, *result;
    struct sockaddr remote;
    int res, sfd, fd;

    if (argc < 2) 
    {
		printf("Paramter Error\n");
		return -1;
    }
    int port = atoi(argv[1]);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) 
    {
 		perror("socket");
		return -1;
    }

    int opt = 1;  
    //需要地址复用的套接字 TIME_WAIT ，socket关闭后，会进入time_wait状态
	//重新绑定时会出现失败 错误信息为bind: Address already in use
	//使用netstat -ano | grep 6666查找端口状态
	//tcp  0  0 127.0.0.1:6666  127.0.0.1:35268  TIME_WAIT timewait(32.62 / 0 / 0)
    //setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) 
    {
		perror("bind");
		exit(1);
    }

    res = listen(sfd, SOMAXCONN);
    if (res == -1) 
    {
        perror("error : cannot listen at the given socket!\n");
        exit(1);
    }

   
    while (1) 
    {
        int len = sizeof(struct sockaddr);
		//文件描述符如果阻塞, 那么accept会阻塞, 如果返回值 < 0, 代表错误
        //如果为非阻塞描述符，会立即返回，且错误码为EAGAIN
        //验证：使用gdb调试时, 在accept后面语句下断点，如果没有连接进来或者发生错误
        //那么会一直阻塞在accept函数，当使用客户端连接时，会成功返回，然后命中断点
        fd = accept(sfd, &remote, &len);
        if (fd < 0)
        {
            perror("accept failed");
        }
        else
        {
            printf("client socket=%d connect success\n", fd);
            pthread_t tid;
            int ret = pthread_create(&tid, NULL, client_routine, &fd);
            if (ret != 0)
            {
                handle_error_en(ret, "create thread failed.");
            }
        }
    }
    
    return 0;
}
