#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <time.h>

int main (int argc, char const *argv[]) {
	int lfd, n, port, afd, alen, sopt, pid, tmp;
	struct sockaddr_in baddr, aaddr;
	long unsigned int num_to_send, num_to_receive;

	if (argc != 2) {
		fprintf(stderr, "Too few parametres\n");
		exit(1);
	}

	if (sscanf(argv[1], "%d%n", &port, &n) != 1 || argv[1][n] || port <= 0 || port > 65535) {
		fprintf(stderr, "Bad port number: %s\n", argv[2]);
		exit(1);
	}

	if ((lfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	sopt = 1;
	if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &sopt, sizeof(sopt)) < 0) {
		perror("setsockopt");
		exit(1);
	}

	baddr.sin_family = AF_INET;
	baddr.sin_port = htons(port);
	baddr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(lfd, (struct sockaddr *) &baddr, sizeof(baddr)) < 0) {
		perror("bind");
		exit(1);
	}

	if (listen(lfd, 5) < 0) {
		perror("listen");
		exit(1);
	}

	while (1) {
		while (waitpid(-1, NULL, WNOHANG) > 0);
		alen = sizeof(aaddr);
		if ((afd = accept(lfd, (struct sockaddr*) &aaddr, &alen)) < 0) {
			perror("accept");
			exit(1);
		}

		if ((pid = fork()) < 0) {
			perror("fork");
		} else if (!pid) {
		  close(lfd);
		  printf("%d: Connection from %s: %d accepted.\n", getpid(), inet_ntoa(aaddr.sin_addr), ntohs(aaddr.sin_port));
		  
		  do {
			  if ( ( tmp = read(afd, &num_to_receive, sizeof(num_to_receive)) ) != sizeof(num_to_receive)) {
				  break;
			  } else {
				num_to_receive = ntohl(num_to_receive);		
				printf("Received from client #%d: %ld\n", getpid(), num_to_receive);
				num_to_send = num_to_receive + 1;
				num_to_send = htonl(num_to_send);
				write(afd, &num_to_send, sizeof(num_to_send));
			  }
		  } while (tmp > 0);
		  close(afd);
		  printf("%d: Connection from %s: %d is over.\n", getpid(), inet_ntoa(aaddr.sin_addr), ntohs(aaddr.sin_port));
		  _exit(0);
		}
		close(afd);
	}
		
	return 0;
}
