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

int readFromPipe(const int in , u_int8_t **msg){
	
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

	/* Read msg_size bytes from the channel */
	read_len = read(in , (void *)msg , msg_size);
	if( read_len < 0 ) {
		perror("readMsg()");
		return -1;
	}

	return read_len;
		
}

int writeInPipe(const int out , const u_int8_t **msg){
	
	u_int16_t size = sizeof(msg);
	
	printf("size : %d \n" , size);
	
	/* Send the size */
	if(write(out , (const void *)&size , sizeof(size)) < 0 ) {
		perror("writeSize()");
		return -1;
	}

	/* Send the message */
	if( write(out ,(const void *)msg, size) < 0 ) {
		perror("writeMsg()");
		return -1;
	}
	return 1;
}

int read_string(int channel_fd, const char * str){
	/*int read_len;
	u_int8_t * msg;

  read_len = read_msg(channel_fd, &msg);
	if(	(read_len != strlen(str) ) ||
   		 memcmp(msg,str,strlen(str)) )
		return (-1);*/

	return (0);
}

