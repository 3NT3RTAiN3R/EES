#include <sys/socket.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <string.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <time.h>
#include <unistd.h>

#define PORT 4030

int main() {
	char const* const fileName = "uSensor.txt"; /* should check that argc > 1 */
	FILE* file = fopen(fileName, "r"); /* should check the result */
	char msg[1024];
	char MessageArray[4][1024];
	int i=0,k=0;
	while (fgets(msg, sizeof(msg), file)) {
	/* note that fgets don't strip the terminating \n, checking its
	   presence would allow to handle msgs longer that sizeof(msg) */
   	   //printf("%s\n", msg); 
	   for(k=0;k <= sizeof(msg);k++){
		MessageArray[i][k] = msg[k];
           }
	   i++;
	}
        printf("%s", MessageArray[1]); 
	printf("\n\n\n");
	
	/* may check feof here to make a difference between eof and io failure -- network
	timeout for instance */
	fclose(file);
	
	int sfd = -1;
	struct sockaddr_in client_addr,server_addr;
	char buffer[1024];
	int bufferLen = -1;
	fd_set rfds;
    	struct timeval tv;
	int retval;
	int n;
	int flag = 0;
	char nToString;
	struct timespec start, stop;
	double roundTripTime = 0;


	while(sfd == -1) {
		printf("creating socket . . .\n");
		//man: int socket(int domain, int type, int protocol);
		sfd=socket(AF_INET,SOCK_DGRAM,0);
		if(sfd == -1)
			perror("unsuccessfull. Repeating the try . . .\n");
	}
	printf("Socket created. FileDiskriptor: %i.\n",sfd);
	
	//man: void bzero(void *s, size_t n);
	bzero(&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = 0;
	client_addr.sin_port = 0;

	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = 0;
	server_addr.sin_port = htons(PORT);

	//aus man: int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	if(bind (sfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
		printf("binding faulture.\n\n");

	do{
	    i = 0;
	    while(i < 4){
		sendto(sfd, MessageArray[i], sizeof(MessageArray[i]), 0, (struct sockaddr *) &server_addr, sizeof(server_addr) );

		/* Wait up to one second. */
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		FD_ZERO(&rfds);
		FD_SET(sfd, &rfds);

		retval = select(sfd+1, &rfds, NULL, NULL, &tv);

		if (retval == -1)
			perror("select()");
		else if (retval) {

			bufferLen = sizeof(server_addr);
			recvfrom(sfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &server_addr, (socklen_t *) &bufferLen);

			n = sendto(sfd, MessageArray[i], sizeof(MessageArray[i]), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
			if (n == 0) {
				printf("Couldn't send the message.");
			} else {

				if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
					perror("clock gettime");
					exit(EXIT_FAILURE);
				}

				printf("%i bytes of %i sent.", n, (int) sizeof(msg));
				recvfrom(sfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &server_addr, (socklen_t *)&bufferLen);
				if (strstr(buffer, "Empfangen")) {
					printf("\n%s\n\n", buffer);
					flag = 4;
					if (clock_gettime(CLOCK_REALTIME, &stop) == -1) {
						perror("clock gettime");
						exit(EXIT_FAILURE);
					}
					roundTripTime = ((stop.tv_sec - start.tv_sec) * 1000000) + (stop.tv_nsec - start.tv_nsec) / 1000;
					printf("RoundTripTime: 		%5.0lfms\n", roundTripTime);
					printf("Ping: 			%5.0lfms\n", roundTripTime / 2);
				} else {
					flag = 0;
				}
			}
		} else {
			printf("No data within one second.\n");
			flag++;
		}
		i++;					
	     }
	} while (flag != 4);
	//close();
	return 0;

}