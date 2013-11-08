#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "Utils/common.h"

#define ClientCommunicationFile "client_folder/clientMSG.txt"

typedef enum clientState {
	HANDSHAKE , 		//Handshake completed, authenication
	AUTHENTICATED ,   //Authentication done , key Exchange phase
	COMMUNICATION , 	//Key and protocol established, communication
	CLOSING,				//Send close message, wait for response and close
	SHUTDOWN				//close forced 
} clientState;

int inputChannel , outputChannel;

clientState		handshake();
clientState 	authentication();
clientState 	keyExchange();
clientState		communication();
void			 	closeConnection();



#endif
