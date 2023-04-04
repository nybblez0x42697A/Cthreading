#ifndef SERVER_H
#define SERVER_H

/**
 * @brief Starts the server and listens for client connections.
 *
 * This function initializes the server using Unix domain sockets,
 * binds it to a specified path, listens for client connections,
 * and mirrors any received messages from connected clients.
 */
void run_server();

#endif /* SERVER_H */
