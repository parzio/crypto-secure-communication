#ifndef _SERVER_H
#define _SERVER_H_

#include "Utils/common.h"

#define ServerCommunicationFile "server_folder/clientMSG.txt"

typedef enum serverState {
	WAITING , 			//Waiting for connection + handshake
	HANDSHAKE , 		//Handshake completed, authenication
	AUTHENTICATED ,   //Authentication done , key Exchange phase
	COMMUNICATION , 	//Key and protocol established, communication
	CLOSING,				//Closing message received, closing the connection
	ERROR					//Error, forcing shutdown
} serverState;

int inputChannel , outputChannel;

int openFifo(const char * pathname);

serverState		handshake();
serverState 	authentication();
serverState 	keyExchange();
serverState		communication();
serverState 	closeConnection();


#endif
