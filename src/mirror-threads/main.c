#include "./include/server.h"
#include "./include/client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int
main(int argc, char *argv[])
{
    int opt;
    int server_flag = 0;
    int client_flag = 0;

    while ((opt = getopt(argc, argv, "sc")) != -1)
    {
        switch (opt)
        {
            case 's':
                server_flag = 1;
                break;
            case 'c':
                client_flag = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s -s|-c\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!server_flag && !client_flag)
    {
        fprintf(stderr,
                "Please specify either server (-s) or client (-c) mode.\n");
        exit(EXIT_FAILURE);
    }

    if (server_flag && client_flag)
    {
        fprintf(stderr,
                "Opening two terminals is beyond the capabilities of a simple "
                "C program. "
                "Please run the program separately in two different terminals: "
                "one as a server and one as a client.\n");
        exit(EXIT_FAILURE);
    }

    if (server_flag)
    {
        run_server();
    }
    else
    {
        run_client();
    }

    return 0;
}
