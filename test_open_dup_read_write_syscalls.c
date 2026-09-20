#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int  main(void) {
	int fd = open("myFile.txt", O_CREAT | O_RDWR | O_APPEND, 0644);
	
	// duplicate fd 
	int fd2 = dup(fd);
	int fd3 = dup2(fd, 1);


	if(fd == -1) {
		perror("open");
		return 1;
	}
	
	if (fd2 == -1){
		perror("dup");
		return 1;
	}
	const char buf[] = "Hello !\n";

	ssize_t w = write(fd, buf, sizeof(buf) -1);
	ssize_t r = read(fd2, buf, sizeof(buf) -1);
		
	if (w == -1) {
		perror("write");
		return 1;
	}
	
	printf("fd = %d\n", fd);
	printf("fd 2 = %d\n", fd2);
	printf("fd 3 = %d\n", fd3);
	printf("%s\n", buf);

	close(fd);
	close(fd2);
	close(fd3);

	return 0;
}
