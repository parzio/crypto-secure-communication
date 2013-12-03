#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "Utility/common.h"

#define ClientCommunicationFile 		"client_folder/clientMSG.txt"
#define ClientRsa512PublicKeyFile	"client_folder/client_rsa512_public_key.txt"	
#define ClientRsa64PrivateKeyFile	"client_folder/client_rsa64_private_key.txt"	
#define ServerRsa512PublicKeyFile	"client_folder/server_rsa512_public_key.txt"

typedef enum clientState {
	HANDSHAKE , 		//Handshake completed, authenication
	AUTHENTICATED ,   //Authentication done , key Exchange phase
	COMMUNICATION , 	//Key and protocol established, communication
	CLOSING,				//Send close message, wait for response and close
	SHUTDOWN				//close forced 
} clientState;

char clientName[256];
int inputChannel , outputChannel;
EncType encType;
clientState state;
rsaKey serverRsa, clientRsa;

int  loadRsaKey();
void clearRsaKey();

int encrypt_and_send(byte * msg , int length);
int receive_and_decrypt(byte * msg);

clientState		handshake();
clientState 	authentication();
clientState 	keyExchange();
clientState		communication();
void			 	closeConnection();



#endif
