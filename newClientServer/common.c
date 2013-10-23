#include "common.h"

int openChannel(const char * path){
	int fd;
	/* Open the FIFO in pathname */
	if( (fd = open(path,O_RDWR)) < 0 ) {
		perror("open()");
		exit(1);
	}

	return (fd);
}

void closeChannel(int channel_fd){
	/* Close the file descriptor */
	close(channel_fd);
}

int readFromPipe(const int in , byte *msg){
	
	int read_len;
	u_int16_t msg_size;

	/* The first 16 bits represents the message size */
	if(read(in ,(void *)&msg_size , sizeof(msg_size)) < 0 ) {
		perror("readSize()");
		return -1;
	}

	/* Verify that the size of the message is no more than MSG_SIZE_MAX bytes */
	if( msg_size >= MSG_MAX_SIZE ) {
		perror("Message too large\n");
		return -1;
	}

	char buffer[2048];
	
	/* Read msg_size bytes from the channel */
	read_len = read(in , (void *)buffer , msg_size);
	
	memcpy(msg, buffer , read_len);
	
	if( read_len < 0 ) {
		perror("readMsg()");
		return -1;
	}
	
	return read_len;
		
}

int writeInPipe(const int out , const byte *msg , u_int16_t size){

	/* Send the size */
	if(write(out , (const void *)&size , sizeof(size)) < 0 ) {
		perror("writeSize()");
		return -1;
	}

	u_int16_t written = write(out ,(const void *)msg, size);
	
	/* Send the message */
	if( written  < 0 ) {
		perror("writeMsg()");
		return -1;
	}
	
	return written;
}

int readFromFile(FILE *file, char *string){
	
	while(fgets(string, MSG_MAX_SIZE , file) != NULL){
		
		if(!((string[0] == '/') && (string[1] == '/')))
			return 1;
	}
	
	return 0;
	
}


void printMsg(char *string , u_int16_t size){
	
	char buffer[2049];
	
	memcpy(buffer, string , size);
	
	buffer[size] = '\0';
	
	fprintf(stderr , "message : %s \n" , string);	
}