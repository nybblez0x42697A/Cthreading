#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/mirror_socket"

void
run_server()
{
    int       server_fd;
    int       client_fd;
    socklen_t len;

    struct sockaddr_un server_addr, client_addr;
    char               buffer[1024];

    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    unlink(SOCKET_PATH);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))
        == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(server_fd, 1) == -1)
    {
        perror("listen");
        exit(1);
    }

    while (1)
    {
        len = sizeof(client_addr);
        if ((client_fd
             = accept(server_fd, (struct sockaddr *)&client_addr, &len))
            == -1)
        {
            perror("accept");
            exit(1);
        }

        printf("Client connected.\n");

        while (1)
        {
            memset(buffer, 0, sizeof(buffer));
            int recv_len = recv(client_fd, buffer, sizeof(buffer), 0);

            if (recv_len > 0)
            {
                printf("Client: %s\n", buffer);
            }
            else
            {
                if (recv_len < 0)
                {
                    perror("recv");
                }
                close(client_fd);
                printf("Client disconnected.\n");
                break;
            }
        }
    }

    close(server_fd);
    unlink(SOCKET_PATH);
}