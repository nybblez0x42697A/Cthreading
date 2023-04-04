#ifndef CLIENT_H
#define CLIENT_H

/**
 * @brief Connects to the server and sends messages.
 *
 * This function initializes the client using Unix domain sockets,
 * connects to the server at a specified path, and sends messages
 * to the server for mirroring.
 */
void run_client();

#endif /* CLIENT_H */
