// modbusTP.c
//
#include <string.h>

#include "Modbus.h"
#include "TP3.h"

int GlobaleAdresseRegulatorModbus = 1;

void printState(ErrorComm codret)
{
	switch(codret)
	{
		case ERRORCOMM_BCC:{
			printf("\nError BCC\n");
		break;}

		case ERRORCOMM_TIMEOUT:{
			printf("\nError Timeout\n");
		break;}

		case ERRORCOMM_NOERROR:{
			printf("\nNo error\n");
		break;}

		case ERRORCOMM_ERROR:{
			printf("\nError\n");
		break;}

		default:{
			printf("\nError\n");
		break;}
	}
	return;
}

SOCKET connectionTCPIpPort()
{
    BOOL connexionOk = FALSE;
    SOCKET idConnexionSocket = INVALID_SOCKET;

    // A COMPLETER

    return idConnexionSocket;
}

HANDLE connectionSerialPort()
{	
	// A COMPLETER
    BOOL connexionOk = FALSE;
	HANDLE handleSerialPort = NULL;
    int iPort = 502;

	printf("Entrer le numero de port?\n");
    scanf_s("%d", &iPort);

	if (iPort <= 502)
	{
		
        handleSerialPort = createSerialPort(iPort);
	
		
		if(handleSerialPort != NULL && handleSerialPort != INVALID_HANDLE_VALUE)
		{
			int iBaudRate = 9600;
            int iBiteSize = 8;
            int iParite = 0;
            int iStopBit = 0;

			printf("*********** Parametrage du port serie ***********\n");
            printf("Entrer la vitesse de transmission? (300,600,1200,2400,4800,9600,19200,38400) \n");
            scanf_s("%d", &iBaudRate);

			printf("Entrer le nombre de bits de donnees? (5-8) \n");
            scanf_s("%d", &iBiteSize);

			printf("Entrer la parite? 0 (pas de parite) / 1 (Parite impair) / 2 (Partie pair)\n");
            scanf_s("%d", &iParite);

            printf("Entrer le nombre de bist de stop? (0 (1 bit) / 1 (1.5 bits) / 2 (2 bits)\n");
            scanf_s("%d", &iStopBit);

            connexionOk = setParamSerialPort(handleSerialPort, iBaudRate,(BYTE)iBiteSize,(BYTE)iParite,(BYTE)iStopBit);
            if (connexionOk)
                printf("Connexion OK\n");
			else
                printf("Connexion KO - Erreur lor du parametrage\n");
		}
	}
	else
	{
		connexionOk = FALSE;
        printf("Connexion KO - Le port n'existe pas ou est deja ouvert\n");
	}

	if (handleSerialPort && !connexionOk)
	{
		terminateSerialPort(handleSerialPort);
		handleSerialPort = NULL;
	}




    return handleSerialPort;
}

int createRequestTrame(TypeRequest i_requestType, char* i_trameSend, TypeVal* i_typeVal)
{
	int lengthTrameSend = 0;
	int startAdress = 0;
	int nbParamsToread = 0;

	short valToWriteShort;
	int valToWriteInt;
	float valToWriteFloat;
	*i_typeVal=NO_TYPE;

	switch(i_requestType)
	{
		// Demande de lecture:
		case REQUEST_READ:{
			printf("\n DEMANDE DE LECTURE\n");

			printf("A partir de quelle adresse souhaitez-vous lire?\n");
            scanf("%d", &startAdress);

			printf("Quel type de parametre voulez-vous lire? 0 (short) / 1 (int) / 2 (float)\n");
            scanf("%d",i_typeVal);

			
            // A COMPLETER
			 
			//On determine le nombre de mot  en fonction du choix de l utilisateur short-> 1 mot | int et float ->2 mots 
			if(i_typeVal == 0){
				nbParamsToread=1;
				
			}else{
				nbParamsToread=2;
			}
			//Creation de la trame de lecture modbus=>address 1 function 03 nombre de parametre 1 ou 2 et CRC
			lengthTrameSend= makeTrameLecModBus(1,MODBUS_FUNCTION_READ_NWORDS,startAdress,nbParamsToread,i_trameSend,INTEL); 

			break;}

		// Deamnde d'ecriture
		case REQUEST_WRITE:{
			int typeValCopy;

			printf("\n DEMANDE D'ECRITURE\n");

			printf("A partir de quelle adresse souhaitez-vous ecrire?\n");
            scanf("%d", &startAdress);
			printf("Quel type de parametre voulez-vous ecrire? 0 (short) / 1 (int) / 2 (float)\n");
            scanf("%d",&typeValCopy);
			printf("Entre la valeur a ecrire?\n");

            // A COMPLETER
			switch(typeValCopy){
				case 0:
					*i_typeVal=TYPE_SHORT;
				 	break;
				case 1:
					*i_typeVal=TYPE_INT;
					break;
				case 2:
					*i_typeVal=TYPE_FLOAT;
					break;

			}
			
			//Selon le type de la valeur on va recuperer un short un int ou un float 
			if(*i_typeVal == TYPE_SHORT)
			{
				
				printf("dans la condition short \n");
				scanf("%hi",&valToWriteShort);

				//nombre d octet =1
				nbParamsToread=1;

				//Creation de la trame d ecriture  modbus pour short (ecriture 1 mot )
				lengthTrameSend = makeTrameEcrModBusFromShort(1,MODBUS_FUNCTION_WRITE_WORD,startAdress,valToWriteShort,i_trameSend,INTEL); 
				printf("a la fin de mon if short \n");
			}else if (*i_typeVal == TYPE_INT)
			{

				scanf("%d",&valToWriteInt);

				nbParamsToread=2;
				//Creation de la trame d ecriture  modbus pour short (ecriture n mots )
				lengthTrameSend = makeTrameEcrModBusFromInt(1,MODBUS_FUNCTION_WRITE_WORDS,startAdress,valToWriteInt,i_trameSend,INTEL);

				
			}else
			{	scanf("%f",&valToWriteFloat);

				nbParamsToread=2;

				lengthTrameSend = makeTrameEcrModBusFromFloat(1,MODBUS_FUNCTION_WRITE_WORDS,startAdress,valToWriteFloat,i_trameSend,INTEL);


			}

			

			break;}
	}

	return lengthTrameSend;
}

ErrorComm parseModbusResponse(char* i_trameReceive, int i_lengthTrameReceived, TypeRequest i_requestType, TypeVal i_typeVal)
{
	ErrorComm codret = ERRORCOMM_ERROR;
	char value[50];
	int nbValue;
	int address;
	int codeFunction;
	i_typeVal=NO_TYPE;

	short shortNum;
	float floatNum;
	int intNum;
    // A COMPLETER

	codret=parseTrameModBus(i_trameReceive,i_lengthTrameReceived,value,&nbValue,&address,&codeFunction,INTEL);
	//affichage de la trame recu
	printf("\n Received trame (length = %i):", i_lengthTrameReceived);
	for (int i = 0; i < i_lengthTrameReceived; i++)
        {
            printf("%02X ",((unsigned char)i_trameReceive[i]));
        }
        printf("\n");

	//on prend que la partie du message pour l afficher 
	 char *asciiDonnee=i_trameReceive+2;
	

	/*if(i_typeVal==TYPE_SHORT){shortNum=ModBusShortAsciiToIeee(asciiDonnee,INTEL) ; printf(" La valeur : %hu",shortNum);}
	if(i_typeVal==TYPE_FLOAT){floatNum=ModBusFloatAsciiToIeee(asciiDonnee,INTEL); printf(" La valeur :  %0.2f",floatNum);}
	if(i_typeVal==TYPE_INT){intNum=ModBusIntAsciiToIeee(asciiDonnee,INTEL);printf(" La valeur : %i",intNum);}*/

	shortNum=ModBusShortAsciiToIeee(asciiDonnee,INTEL) ; printf(" \n La valeur : %hu",shortNum);



	return codret;
}
