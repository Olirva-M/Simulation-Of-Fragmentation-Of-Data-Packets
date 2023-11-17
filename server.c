#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define MAX 1024
#define MAXCLIENT 3

struct fragment {
  int id;
  int size;
  char data[1024];
  int pid;
  int D;
  int M;
  int offset;
};

void fragmentData(int dataSize, int mss, struct fragment *fragments, int *numFragments);
void displayFragments(struct fragment *fragments, int numFragments);

int main(int argc, char *argv[]) {
  int sockfd, ind = 0, mss, temp, data_size;
  struct sockaddr_in serveraddr, clientaddr;
  char buff[MAX], ch;
    struct fragment fragments[MAX];

  sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Use SOCK_DGRAM for UDP
  if (sockfd < 0)
    perror("Error in socket creation\n");

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(7428);
  serveraddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    perror("Error in binding\n");

  int length = sizeof(clientaddr);
  // GETTING clientaddr
  temp = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&clientaddr, &length);
  printf("Enter MSS (maximum segment size): ");
  scanf("%d", &mss);
  //mss = 512;
  printf("Enter data size : ");
  //data_size = 65;
  scanf("%d", &data_size);
 
  int numfragment = 0;

  fragmentData(data_size, mss, fragments, &numfragment);

  /*printf("%d",mss);
  printf("%s",buff);*/

  displayFragments(fragments, numfragment);

  for (int i = 0; i < numfragment; i++)
    sendto(sockfd, &fragments[i], sizeof(fragments[i]), 0, (struct sockaddr *)&clientaddr, sizeof(clientaddr));

  close(sockfd);
  return 0;
}

void fragmentData(int dataSize, int mss, struct fragment *fragments, int *numFragments) {
  int ind = 0, offset = 0;
  srand(time(NULL));
  int packetID = rand() % 9000 + 1000;
  
  printf("\n");
  if (dataSize <= mss)
  {
    double result = dataSize / 8;
    int fragmentSize = (8 * (int)result);
    if (fragmentSize == dataSize)
	  {
	    printf("No fragmentation required!\n");
	    fragments[*numFragments].offset = 0;
	    fragments[*numFragments].id = 0;
	    fragments[*numFragments].size = dataSize;
	    //strncpy(fragments[*numFragments].data, data + ind, fragmentSize);
	    //fragments[*numFragments].data[fragmentSize] = '\0'; // Null-terminate the string
	    fragments[*numFragments].pid = packetID;
	    fragments[*numFragments].M = 0;
	    fragments[*numFragments].D = 0;
	    
	    (*numFragments)++;
	    return;
	  }
  }

  printf("Fragmenting...");
  while (ind < dataSize) {
    int temp_fragmentSize =(dataSize - ind > mss) ? mss : dataSize - ind;

    double result = (temp_fragmentSize) / (8);
    int fragmentSize = (8 * (int)result);

    // Store the fragment in the array of structures
    fragments[*numFragments].offset = (int)offset;
    fragments[*numFragments].id = *numFragments + 1;
    fragments[*numFragments].size = fragmentSize;
    //strncpy(fragments[*numFragments].data, data + ind, fragmentSize);
    //fragments[*numFragments].data[fragmentSize] = '\0'; // Null-terminate the string
    fragments[*numFragments].pid = packetID;
    fragments[*numFragments].M = 1;
    fragments[*numFragments].D = 0;
    
    (*numFragments)++;


    ind += fragmentSize;
    offset = (double)ind/8;
  }
  fragments[(*numFragments) - 1].M = 0;
  fragments[(*numFragments) - 1].D = 1;
}

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
