#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX 1024

struct fragment {
  int id;
  int size;
  char data[1024];
  int pid;
  int D;
  int M;
  int offset;
};


void displayFragments(struct fragment *fragments, int numFragments) {
  int i;
  printf("\n");
  for (i = 0; i < numFragments; i++) {
    printf("Packet ID: %d, Fragment no. = %d, Size = %d, Offset = %d, D = %d, M = "
           "%d\n",
           fragments[i].pid, fragments[i].id, fragments[i].size,
           fragments[i].offset, fragments[i].D, fragments[i].M);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  int sockfd, n = 0, mss, packetID, ind = 0;
  char buff[MAX] = "hello from client";
  struct fragment fragments[MAX];
  struct sockaddr_in serveraddr;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0)
    perror("Error in socket creation\n");

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(7428);
  serveraddr.sin_addr.s_addr = INADDR_ANY;

  // INITIATE LINK BY SENDING A MESSAGE
  sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
  
  sleep(1);
  n = recvfrom(sockfd, &fragments[ind], sizeof(struct fragment), 0, NULL, NULL);
  ind++;
  
  while (fragments[ind-1].M != 0)
  {
    sleep(1);
    n = recvfrom(sockfd, &fragments[ind], sizeof(struct fragment), 0, NULL, NULL);
    ind++;
  }

  printf("\nPacket(s) received : \n");
  displayFragments(fragments, ind);
  
  for (int i = 0; i < ind; i++)
  {
  	if (fragments[i].M == 0 && fragments[i].offset != 0)
  	{
  		printf("\nLAST FRAGMENT : ");
  		printf("Packet ID: %d, Fragment no. = %d, Size = %d, Offset = %d, D = %d, M = "
           "%d\n",
           fragments[i].pid, fragments[i].id, fragments[i].size,
           fragments[i].offset, fragments[i].D, fragments[i].M);
  	}
  	else if (fragments[i].M == 0 && fragments[i].offset == 0)
  	{
  		printf("\nDATA WITHOUT FRAGMENTATION : ");
  		printf("Packet ID: %d, Fragment no. = %d, Size = %d, Offset = %d, D = %d, M = "
           "%d\n",
           fragments[i].pid, fragments[i].id, fragments[i].size,
           fragments[i].offset, fragments[i].D, fragments[i].M);
  	}  	
  	else if (fragments[i]. offset == 0)
  	{
  		printf("\nFIRST FRAGMENT : ");
  		printf("Packet ID: %d, Fragment no. = %d, Size = %d, Offset = %d, D = %d, M = "
           "%d\n",
           fragments[i].pid, fragments[i].id, fragments[i].size,
           fragments[i].offset, fragments[i].D, fragments[i].M);
  	}
  }
  close(sockfd);
  return 0;
}
