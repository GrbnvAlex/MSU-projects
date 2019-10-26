#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

int main (int argc, char const *argv[]) {
	int 			port, n, sfd, tmp;
	struct			hostent *phe;
	struct sockaddr_in 	sin;
	long unsigned int num_to_send, num_to_receive;

	if (argc != 3) {
		fprintf(stderr, "Too few parameters\n");
		exit(1);
	}

	if (!(phe = gethostbyname(argv[1]))) {
		fprintf(stderr, "Bad host name: %s\n", argv[1]);
		exit(1);
	}

	if (sscanf(argv[2], "%d%n", &port, &n) != 1 || argv[2][n] || port <= 0 || port > 65535) {
		fprintf(stderr, "Bad port number: %s\n", argv[2]);
		exit(1);
	}

	if ((sfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	memcpy(&sin.sin_addr, phe->h_addr_list[0], sizeof(sin.sin_addr));
	if (connect(sfd, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
		perror("connect");
		exit(1);
	}

	printf("Input a number(s) to be sent:\n");

	do {	
		if (scanf("%ld", &num_to_send) == 0) break;  // Break if no numbers to read 
		num_to_send = htonl(num_to_send);
		write(sfd, &num_to_send, sizeof(num_to_send));
		
		if ( ( tmp = read(sfd, &num_to_receive, sizeof(num_to_receive)) ) != sizeof(num_to_receive) ) {
			fprintf(stderr, "read failed\n");
		} else {
			num_to_receive = ntohl(num_to_receive);
			printf("Received from server: %ld\n", num_to_receive);
		}
		
	} while (1); 

	close(sfd);
	return 0;

}
