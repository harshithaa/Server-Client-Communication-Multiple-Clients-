/* This is the server application for cs-528 project 1, submitted by Harshithaa
   Sundareswaran and Daniel Townley (Group 12). The client is broadly modeled on
   the example found on pp 42-44 of Peterson and Davie's Computer Networks: a
   Systems Approach (5th ed.), but introduces multithreading to support up to 16
   clients.
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>

#define PORT_NUM 5436
#define MAX_PENDING 5
#define MAX_LINE    256
#define MAX_CLIENTS 16

struct sargs{
  int sock_id;
};

void *server_output(void *args){
  char buffer[MAX_LINE];
  int len;
  int new_sock_id;

  // Use the socket ID provided by parent to read new messages.
  new_sock_id = ((struct sargs*) args)->sock_id;
  
  //printf("entering thread: s = %d\n", new_s);

  // Print each message the server receives, as long as messages are available.
  while ((len = recv(new_sock_id, buffer, sizeof(buffer), 0))) {
    //fputs(buffer, stdout);
    printf("%s", buffer);
 }

 close(new_sock_id);
 return (void*) 0;
}


int main()
{
  int i = 0; // For itteration
  pthread_t server_threads[MAX_CLIENTS]; // Holds threads to listen to clients
  //int new_sockets[MAX_CLIENTS]; // holds sockets asigned per client
  struct sargs server_args[MAX_CLIENTS]; // holds args to threads
  struct sockaddr_in sin;
  char buffer[MAX_LINE]; // Buffer to hold text from client
  socklen_t len;
  int sock_id;

  /* Populate address fields */
  bzero((char *)&sin, sizeof(sin)); // Clear all fields
  sin.sin_family = AF_INET;         // Use Internet Protocol
  sin.sin_addr.s_addr = INADDR_ANY; 
  sin.sin_port = htons(PORT_NUM);// Set the listening port

  /* Obtain a new socket id and bind it to the address. */
  if ((sock_id = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("clouldn't get a socket");
    exit(1);
  }

  if ((bind(sock_id, (struct sockaddr *) &sin, sizeof(sin))) < 0) {
      perror("clouldn't bind socket");
      exit(1);
  }

  // Wait for a connection over the port.
  listen(sock_id, MAX_PENDING);

  // pass sock_id to any thread reading from client
  //server_args.s = s;
  
  // Wait for the clients to connect, and asign each a new thread to display
  // messages.
  for(i = 0; i < MAX_CLIENTS; i++){
    if ((server_args[i].sock_id = accept(sock_id, (struct sockaddr *)&sin, &len)) < 0) {
      perror("couldn't accept data from client");
      exit(1);
    }
    pthread_create(&(server_threads[i]), NULL, server_output, (void*) &server_args[i]);     
  }

  for(i = 0; i < MAX_CLIENTS; i++){
    pthread_join(server_threads[i], NULL);
  }
}
