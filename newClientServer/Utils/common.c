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

	char buffer[MSG_MAX_SIZE];
	
	/* Read msg_size bytes from the channel */
	read_len = read(in , (void *)buffer , msg_size);

	if( read_len < 0 ) {
		perror("readMsg()");
		return -1;
	}
	
	memcpy(msg, buffer , read_len);
	
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
	
	int read;
	size_t length = MSG_MAX_SIZE;
	
	do{	
	read = getline(&string , &length, file);
	
	if(read == -1)
		return -1;
	else
		return read;
	
	}while((string[0] == '/') && (string[1] == '/'));
	
}


void printMsg(char *header , char *string , u_int16_t size){	
	int i ;
	
	fprintf(stderr , "%s ' " , header);
	
	for(i = 0; i < size ; i++)
		if(string[i] != '\n')
			fprintf(stderr , "%c" , string[i]);
	
	fprintf(stderr , " ' of length %d \n" , size);

}

