#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>

#define PORT 4030

double diffJitter(double curJitter, double roundTrip, int quantity) {
	return roundTrip / quantity - curJitter;
}

int main() {
	int sfd = -1;
	int msgLen = -1;
	int bufferLen = -1;
	struct sockaddr_in serv_addr,client_addr;
	char buffer[1024];
	char buffer2[1024];
	//char msg[1024];
	char len[1024];
	char msg2[] = "Empfangen";
	char msg3[1024];
        char valueA[1024];
        char valueB[1024];
        char value1[1024];
        char value2[1024];
	int jitterRound = 0;
	char* ptr;
	FILE *fp;
	struct timespec start, stop;
	double roundTripTime = 0, jitter = 0;
	int jitterCounter = 0;

	while(sfd == -1) {
		printf("Versuche Socket zu erstellen . . .\n");
		//aus man: int socket(int domain, int type, int protocol);
		sfd=socket(AF_INET,SOCK_DGRAM,0);
		if(sfd == -1)
			perror("Versuch der Erstellung des Sockets misslungen. Vorgang wird wiederholt.\n\n");
	}

	printf("Socket created. FileDiskriptor: %i.\n\n",sfd);

	//aus man: void bzero(void *s, size_t n);
	bzero(&serv_addr, sizeof(serv_addr));
	bzero(&client_addr, sizeof(client_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = 0;
	serv_addr.sin_port = htons(PORT);

	//aus man: int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	if(bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		printf("binding faulture.\n\n");

	memset (buffer, 0, sizeof(buffer));
	
	while(1) {
		while(msgLen <= 0){
			bufferLen = sizeof(client_addr);
	
			if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
				perror("clock gettime");
				exit(EXIT_FAILURE);
			}
			//aus man: ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
			msgLen = recvfrom(sfd,buffer,sizeof(buffer),0,(struct sockaddr *) &client_addr,(socklen_t *)&bufferLen);
		
			printf("Datei empfangen. Diese lautet: \n%s\n\n",buffer);
			strcpy(buffer2 , buffer);
			//printf("\n\n buffer  = %s\n\n",buffer); 
			//printf("\n\n buffer2 = %s\n\n",buffer2); 
			ptr = strtok(buffer,";");
			if(strcmp(ptr,"GUI") == 0){
			   strcat(valueA,valueB);
		           strcat(valueA,value1);
		           strcat(valueA,value2);
			   strcpy(msg3 , valueA);
          		   sendto(sfd, msg3, sizeof(msg3), 0, (struct sockaddr *) &client_addr, sizeof(client_addr));
			   printf("\n\n\n\nAn die Gui wird übergeben\n\n%s\n\n\n\n\n",msg3);	
			}else if (strcmp(ptr,"A")== 0){
			  strcpy(valueA , buffer2); 
	  		  //printf("\n\n valueA = %s\n\n",valueA); 
			}else if (strcmp(ptr,"B")== 0){
			  strcpy(valueB , buffer2);
   			  //printf("\n\n valueB = %s\n\n",valueB); 
			}else if (strcmp(ptr,"1")== 0){
			  strcpy(value1 , buffer2);
	  		  //printf("\n\n value1 = %s\n\n",value1); 
			}else if (strcmp(ptr,"2")== 0){
			  strcpy(value2 , buffer2);
	  		  //printf("\n\n value2 = %s\n\n",value2); 
			}else{
			 printf("nichts zwischen gespeichert");
			}
      msgLen = 0;
      sendto(sfd, msg2, sizeof(msg2), 0, (struct sockaddr *) &client_addr, sizeof(client_addr));
      printf("Der %s hat ", ptr);
      ptr = strtok(NULL, ";");
      printf("%s gemessen. Als zusätzliche Information wurde folgende Information mitgegeben:", ptr);
      ptr = strtok(NULL, ";");
      printf("\"%s\".\n\n", ptr);
      
			if (clock_gettime(CLOCK_REALTIME, &stop) == -1) {
				perror("clock gettime");
				exit(EXIT_FAILURE);
			}
			roundTripTime = ((stop.tv_sec - start.tv_sec) * 1000000) + (stop.tv_nsec - start.tv_nsec) / 1000;
			printf("RoundTripTime: 		%5.0lfms\n", roundTripTime);
			printf("Ping: 			%5.0lfms\n", roundTripTime / 2);
			jitterRound = diffJitter(jitterRound, roundTripTime, ++jitterCounter);
			printf("Jitter: 		%5.0dms\n", jitterRound);
			printf("Dateizugriff. . .");
			fp = fopen("log.txt", "a");
			ptr = strtok(buffer,";");
			while(ptr != NULL) {
				fprintf(fp, "%s ",ptr);
      				ptr = strtok(NULL, ";");
   			}
			fprintf(fp, "%s", "\n");
			//printf("Dateizugriff Ende \n");
			memset(buffer, 0, sizeof(buffer));			
			fclose(fp);
		}
	}
	//close();
	return 0;
}
