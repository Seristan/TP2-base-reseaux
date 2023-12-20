#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>


#define TFTP_PORT "69"
#define SERVER_MSG_LEN 256
#define FILE_MSG_LEN 256

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s server filename\n", program_name);
}

void print_server_info(const char *servername, const char *filename) {
    char serverMsg[SERVER_MSG_LEN];
    char fileMsg[FILE_MSG_LEN];

    int serverMsgLen = snprintf(serverMsg, sizeof(serverMsg), "Server Name: %s\n", servername);
    int fileMsgLen = snprintf(fileMsg, sizeof(fileMsg), "Filename: %s\n", filename);
    if (serverMsgLen > 0) {
        write(STDOUT_FILENO, serverMsg, serverMsgLen);
    }
    if (fileMsgLen > 0) {
        write(STDOUT_FILENO, fileMsg, fileMsgLen);
    }
}

int resolve_hostname(const char *servername, struct addrinfo **res) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; // Supports both IPv4 and IPv6
    hints.ai_socktype = SOCK_DGRAM; // Specifies UDP for TFTP

    return getaddrinfo(servername, TFTP_PORT, &hints, res);
}


void print_resolved_address(const struct addrinfo *addrinfo_list) {
    char host[NI_MAXHOST];
    for (const struct addrinfo *p = addrinfo_list; p != NULL; p = p->ai_next) {
        if (p->ai_family == AF_INET || p->ai_family == AF_INET6) {
            // Convert network address to a character string
            if (getnameinfo(p->ai_addr, p->ai_addrlen, host, sizeof(host), NULL, 0,
                            NI_NUMERICHOST) == 0) {
                printf("Server Address: %s\n", host);
                break; // Stop after printing the first address for this example
            }
        }
    }
}
int create_socket(const struct addrinfo *server_info) {
    int sockfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (sockfd < 0) {
        perror("Error creating socket");
        return -1;
    }
    return sockfd;
}

int send_tftp_rrq(int sockfd, const struct sockaddr *server_addr, socklen_t server_addrlen, const char *filename) {
    // RRQ TFTP format: | 01 | Filename | 0 | Mode | 0 |
    char rrq_packet[2 + strlen(filename) + 1 + strlen("octet") + 1];
    int packet_len = sprintf(rrq_packet, "%c%c%s%c%s%c", 0, 1, filename, 0, "octet", 0);
    // RRQ REQUEST SENT TO THE SERVER
    if (sendto(sockfd, rrq_packet, packet_len, 0, server_addr, server_addrlen) < 0) {
        perror("sendto");
        return -1;
    }
    // Normally the server should respond with a paquet or a error
    char buffer[512];
    if (recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL) < 0) {
        perror("recvfrom");
        return -1;
    }
    return 0;
}

    int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
}

    const char *servername = argv[1];
    const char *filename = argv[2];

    print_server_info(servername, filename);

    struct addrinfo *res;
    int status = resolve_hostname(servername, &res);

    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return EXIT_FAILURE;
    }
    print_resolved_address(res);



    int socketfd = create_socket(res);
    if (socketfd < 0) {
        freeaddrinfo(res);

        return EXIT_FAILURE;
    }
    // Test socket by sending an RRQ request
    if (send_tftp_rrq(socketfd, res->ai_addr, res->ai_addrlen, "testfile") < 0) {
        close(socketfd);
        freeaddrinfo(res);
        return EXIT_FAILURE;
    }

    close(socketfd);
    freeaddrinfo(res); // Free the memory allocated by getaddrinfo

    return EXIT_SUCCESS;
}


