#ifndef _SERVER_H
#define _SERVER_H_

#include "Utility/common.h"

#define ServerCommunicationFile 		"server_folder/clientMSG.txt"
#define ClientRsa64PublicKeyFile	"server_folder/client_rsa64_public_key.txt"	
#define ServerRsa512PrivateKeyFile	"server_folder/server_rsa512_private_key.txt"	
#define ServerRsa512PublicKeyFile	"server_folder/server_rsa512_public_key.txt"	


typedef enum serverState {
	WAITING , 			//Waiting for connection + handshake
	HANDSHAKE , 		//Handshake completed, authenication
	AUTHENTICATED ,   //Authentication done , key Exchange phase
	COMMUNICATION , 	//Key and protocol established, communication
	CLOSING,				//Closing message received, closing the connection
	ERROR					//Error, forcing shutdown
} serverState;

int inputChannel , outputChannel;
EncType encType;
serverState state;
rsaKey serverRsa, clientRsa;

int openFifo(const char * pathname);

int  loadServerRsaKey();
int  loadClientRsaKey(const char *clientName);
void clearRsaKey();

int encrypt_and_send(byte * msg , int length);
int receive_and_decrypt(byte * msg);

serverState		handshake();
serverState 	authentication();
serverState 	keyExchange();
serverState		communication();
serverState 	closeConnection();


#endif
