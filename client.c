/* This is the client application for cs-528 project 1, submitted by Harshithaa
   Sundareswaran and Daniel Townley (Group 12) The client is broadly modeled on
   the example on pp 41-42 of Peterson and Davie's Computer Networks: a Systems
   Approach (5th ed.)
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>


#define PORT_NUM 5436
#define MAX_LINE    256

int main(int argc, char * argv[])
{  
  struct sockaddr_in sin;
  struct hostent *host;
  char buff[MAX_LINE];
  int sock_id;
  int msg_len;
  FILE *fp;
  
  if (argc != 2) {
    exit(1);
  }

  /* Populate address fields. The address is actually a complex
     data structure with multiple fields */
  bzero((char *) &sin, sizeof(sin));  // Clear all fields in the address struct
  sin.sin_family = AF_INET;           // Use internet protocol
  sin.sin_port = htons(PORT_NUM);  // Aquire server port and fill out the corresponding field

  /* Retrieve peer ip from name */
  host = gethostbyname(argv[1]);
  if (!host) {
    fprintf(stderr, "Couldn't resolve IP from hostname");
    exit(1);
  }
  bcopy(host->h_addr, (char*) &sin.sin_addr, host->h_length);  // Copy address to the address field

  /* Attempt acquire a socket, exit if one can't be found */
  if ((sock_id = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror( "couldn't open a socket");
    exit(1);
  }

  /* Try to connect to the server using the provided address. */
  if (connect(sock_id, (struct sockaddr *) &sin, sizeof(sin)) < 0 ) {
    perror("couldn't connect to server");
    exit(1);
  }

  /* Keep checking for user input. When the user enters data to stdin,
     send that data to the server. */
  while (fgets(buff, sizeof(buff), stdin)) {
    buff[MAX_LINE-1] = 0;        // Null-terminate the message  
    send(sock_id, buff, strlen(buff) + 1, 0);       // send message to server.
  }
}

  
