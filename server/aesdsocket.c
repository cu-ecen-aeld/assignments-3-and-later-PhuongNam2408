#include <sys/socket.h> // socket()
#include <sys/types.h> // socket()
#include <netdb.h>
#include <string.h> // memset()
#include <stdio.h>
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h> // inet_ntop()
#include <signal.h> // sigaction()
#include <stdlib.h>
#include <fcntl.h> // open()
#include <unistd.h> // unlink()
#include <errno.h>

#define PORT "9000"
#define AESDSOCKETDATA_PATH "/var/tmp/aesdsocketdata"

int sock_server_fd, client_fd, aesdsocketdata_fd;

void signal_handler(int sig_num)
{
  int ret;
  printf("Received signal number = [%d]\n", sig_num);
  
  // Close fd files
  ret = close(sock_server_fd);
  if (ret == -1) {
    printf("Close sock_sever_fd failed\n");
  } else {
    printf("Close sock_server_fd success\n");
  }

  ret = close(client_fd);
  if (ret == -1) {
    printf("Close client_fd failed\n");
  } else {
    printf("Close client_fd success\n");
  }

  ret = close(aesdsocketdata_fd);
  if (ret == -1) {
    printf("Close aesdsocketdata_fd failed\n");
  } else {
    printf("Close aesdsocketdata_fd success\n");
  }
}

int main(int argc, char *argv[]) {
  int ret;
  struct addrinfo hints;
  struct addrinfo *response;
  char response_ip_v4[sizeof("xxx.xxx.xxx.xxx")];
  struct sigaction signal_action;
  struct sockaddr_storage client_addr;
  socklen_t client_addr_len;
  const int setsockopt_enable = 1;

  memset(&signal_action, 0, sizeof(signal_action));
  signal_action.sa_handler = signal_handler;

  // Register to SIGINT and SIGTERM
  ret = sigaction(SIGTERM, &signal_action, NULL);
  if (ret == -1) {
    printf("ERROR: Setup SIGTERM handler failed\n");
    perror("perror: "); 
    return -1;
  }

  ret = sigaction(SIGINT, &signal_action, NULL);
    if (ret == -1) {
    printf("ERROR: Setup SIGINT handler failed\n");
    perror("perror: "); 
    return -1;
  }

  printf("Setup signal success\n");
  
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  ret = getaddrinfo(NULL, PORT, &hints, &response);
  if (ret != 0) {
    printf("ERROR: Get address info failed with ret=%d\n", ret);
    perror("perror: "); 
    return -1;
  } else {
    printf("Get address info success. Information list:\n");
    printf("ai_flags: %d\n", response->ai_flags);
    printf("ai_family: %d\n", response->ai_family);
    printf("ai_socktype: %d\n", response->ai_socktype);
    printf("ai_protocol: %d\n", response->ai_protocol);
    printf("ai_addrlen: %d\n", response->ai_addrlen);
    printf("sin_port: %d\n", ntohs(((struct sockaddr_in *)response->ai_addr)->sin_port));
    inet_ntop(AF_INET, &((struct sockaddr_in *)response->ai_addr)->sin_addr.s_addr, response_ip_v4, sizeof(response_ip_v4));
    printf("sin_addr: %s\n",  response_ip_v4);
    printf("ai_canonname: %s\n", response->ai_canonname);
  }
  
  sock_server_fd = socket(response->ai_family, response->ai_socktype, 0);
  if (sock_server_fd == -1) {
    printf("ERROR: Socket init failed\n");
    perror("perror: "); 
    return -1;
  } else {
    printf("Socket init success with fd = [%d]\n", sock_server_fd);
  }

  // Prevent failure when binding the local address and port
  ret = setsockopt(sock_server_fd, 
                   SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT,
                   &setsockopt_enable,
                  sizeof(setsockopt_enable));
  if (ret == -1) {
    printf("ERROR: setsockopt failed\n");
    perror("perror: "); 
    return -1;
  } else {
    printf("Setsockopt success\n");
  }

  ret = bind(sock_server_fd, response->ai_addr, response->ai_addrlen);
  if (ret == -1) {
    printf("ERROR: Bind to socket failed\n");
    perror("perror: "); 
    return -1;
  } else {
    printf("Bind to socket success\n");

    // Free the socket address
    freeaddrinfo(response);
  }

  // Check whether the program is in deamon mode
  if (argc == 2 && (strcmp(argv[1], "-d") == 0)) {
    pid_t pid = fork();
    if (pid == -1) {
      printf("ERROR: fork failed\n");
      perror("perror: "); 
      return -1;
    } else if (pid == 0) {
      printf("Child process is working, continue proceeding...\n");
    } else {
      printf("Parent process created a child_pid = %d, exiting the parent to working in deamon mode\n", pid);
      return 0;
    }
  }

  ret = listen(sock_server_fd, 5);
  if (ret == -1) {
    printf("ERROR: Listen to socket failed\n");
    perror("perror: ");
    return -1;
  } else {
    printf("Listen to socket success\n");
  }

  // Clean the AESDSOCKETDATA_PATH
  ret = unlink(AESDSOCKETDATA_PATH);
  if (ret == -1) {
    printf("ERROR: unlink (delete) the %s failed\n", AESDSOCKETDATA_PATH);
    perror("perror: "); 
  } else {
    printf("Unlink (delete) the %s success\n", AESDSOCKETDATA_PATH);
  }
  
  while (1) {
    uint8_t client_data_buf[500];
    size_t client_data_current_len = 0, aesdsocketdata_total_len = 0;
    char client_ip_v4[sizeof("xxx.xxx.xxx.xxx")];

    memset(client_ip_v4, 0, sizeof(client_ip_v4));
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr_len = sizeof(client_addr);

    client_fd = accept(sock_server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1) {
      printf("ERROR: accept a new connection failed\n");
      perror("perror: "); 
      return -1;
    } else {
      const char *pret;
      pret = inet_ntop(AF_INET,
                      &((struct sockaddr_in *)&client_addr)->sin_addr.s_addr,
                      client_ip_v4,
                      sizeof(client_ip_v4));
      if (pret == NULL) {
        printf("ERROR: inet_ntop failed\n");
        perror("perror: "); 
        return -1;
      } else {
        printf("Inet_ntop success with pret = %p, dest = %p\n", pret, &client_ip_v4[0]);
      }

      printf("Accept connection from %s, address len = %d\n", client_ip_v4, client_addr_len);
    }

    while (1) {
      size_t ret_len;
      off_t ret_offset;
      ret_len = recv(client_fd, client_data_buf, sizeof(client_data_buf), 0);
      if (ret_len == -1) {
        printf("ERROR: recv from %s failed\n", client_ip_v4);
        perror("perror: "); 
        return -1;
      } else if (ret_len == 0) {
        printf("Connection with %s has been closed, awaiting for a new connection\n", client_ip_v4);
        break;
      } else {
        client_data_current_len = ret_len;
        printf("Receive data from %s with data_len = %lu\n", 
               client_ip_v4, 
               client_data_current_len);

        aesdsocketdata_fd = open(AESDSOCKETDATA_PATH, O_CREAT | O_RDWR , S_IRWXU | S_IRWXG | S_IRWXO);
        if (aesdsocketdata_fd == -1) {
          printf("ERROR: open %s failed\n", AESDSOCKETDATA_PATH);
          perror("perror: "); 
          return -1;
        } else {
          printf("Open %s success\n", AESDSOCKETDATA_PATH);
        }

        ret_offset = lseek(aesdsocketdata_fd, 0, SEEK_END);
        if (ret_offset == -1) {
          printf("ERROR: lseek to end failed\n");
          perror("perror: ");
          return -1;
        } else {
          printf("lseek to the end offset: %ld\n", ret_offset);
        }

        ret_len = write(aesdsocketdata_fd, client_data_buf, client_data_current_len);
        if (ret_len != client_data_current_len) {
          printf("ERROR: write to %s failed with ret_len = %lu\n", AESDSOCKETDATA_PATH, ret_len);
          perror("perror: ");
          return -1;
        } else {
          printf("Write the received data from client to %s success\n", AESDSOCKETDATA_PATH);
        }

        if (client_data_buf[client_data_current_len - 1] == '\n') {
          printf("Reach to the end of the data, closing the connection...\n");

          // Read and send to client all the data that placed in the AESDSOCKETDATA_PATH file
          ret_offset = lseek(aesdsocketdata_fd, 0, SEEK_SET);
          if (ret_offset == -1) {
            printf("ERROR: lseek to end failed\n");
            perror("perror: ");
            return -1;
          } else {
            printf("lseek to the start offset: %ld\n", ret_offset);
          }

          while((ret_len = read(aesdsocketdata_fd, client_data_buf, sizeof(client_data_buf))) > 0) {
            aesdsocketdata_total_len += ret_len;
            ret_len = send(client_fd, client_data_buf, ret_len, 0); 
            printf("Sent %lu bytes to %s\n", ret_len, client_ip_v4);
          }

          ret = close(aesdsocketdata_fd);
          if (ret == -1) {
            printf("ERROR: close aesdsocketdata_fd failed\n");
            perror("perror: "); 
            return -1;
          }

          ret = close(client_fd);
          if (ret == -1) {
            printf("ERROR: close the client_fd connection failed\n");
            perror("perror: "); 
            return -1;
          }
          
          printf("Closed connection from %s\n", client_ip_v4);
          break;
        }
      }
    }
  }

  return 0;
}
