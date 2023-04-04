#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/mirror_socket"

void
run_client()
{
    int                client_fd;
    struct sockaddr_un server_addr;
    char               buffer[1024];

    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))
        == -1)
    {
        perror("connect");
        exit(1);
    }

    printf("Connected to server.\n");

    while (1)
    {
        printf("Enter a message: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (send(client_fd, buffer, strlen(buffer), 0) == -1)
        {
            perror("send");
            exit(1);
        }

        if (strcmp(buffer, "exit") == 0)
        {
            break;
        }
    }

    close(client_fd);
}