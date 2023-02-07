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


/*Cette fontion permet de definit l'adresse de lecture en fontion du choix de l utilisatuer */
int StartAdressLectureDef()
{	
	int startAdress;

	int choix;
	printf("\n****************************Menu des module**********************************");
	printf("\n1.	Temperature mesuree par l entree capteur MV ");
	printf("\n2.	Consigne prise en compte par le regulateur SP");
	printf("\n3.	Mode de regulation (0 : Automatique / 1 : Manuel) ");
	printf("\n4.	Consigne de puissance 		");
	printf("\n5.	Parametre de regulation Proportionnelle ");
	printf("\n6.	Parametre de regulation Integrale");
	printf("\n7.	Parametre de regulation Derivee");
	printf("\n votre choix :");
	scanf("%d",&choix);


	switch(choix){
		case 1 :
			startAdress=1;
		break;
		case 2 :
			startAdress=5;
		break;
		case 3 :
			startAdress=273;
		break;
		case 4 :
			startAdress=3;
		break;
		case 5 :
			startAdress=351;
		break;
		case 6 :
			startAdress=352;
		break;
		case 7 :
			startAdress=353;
		break;
		default:
			printf("\nError\n");
		break;
		
	}


	return startAdress;
}

/*Cette fontion permet de definit l'adresse d'ecriture en fontion du choix de l utilisatuer */
int StartAdressEcrituretureDef()
{
	int startAdress;

	int choix;
	printf("\n****************************Menu des voies**********************************");
	printf("\n1.	Consigne de Temperature  ");
	printf("\n2.	Consigne de puissance");
	printf("\n3.	Mode de regulation (0 : Automatique / 1 : Manuel) ");

	scanf("%d",&choix);


	switch(choix){
		case 1 :
			startAdress=2;
		break;
		case 2 :
			startAdress=3;
		break;
		case 3 :
			startAdress=273;
		break;
		default:
			printf("\nError\n");
		break;
		}

	return startAdress;
}

/*en cas de lecture chaque module contient 3 voies */
/*cette fonction permet alors de choisir la voie en faisant le calcul necessaire sur les adresses*/
int choixDevoie(int startAdress)
{	
	int choix;
	int Adressevoie;

		printf("\nchoisir une voie 1/2/3 ");
		scanf("%d",&choix);

		if(choix==1){
			Adressevoie=startAdress;
		}
		else if(choix==2){
			Adressevoie=startAdress+1024;
		}
		else
		{
			Adressevoie=startAdress+2048;
		}

	return Adressevoie;
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
	//**
	// creation du port serie et configuration de celui ci
    BOOL connexionOk = FALSE;
	HANDLE handleSerialPort = NULL;
    int iPort = 2;
		
    handleSerialPort = createSerialPort(iPort);

	int valdef=3;
	
		
	if(handleSerialPort != NULL && handleSerialPort != INVALID_HANDLE_VALUE)
	{
		int iBaudRate = 9600;
        	int iBiteSize = 8;
        	int iParite = 0;
        	int iStopBit = 0;

		printf("(1 oui/0 non)Voulez vous garder les valeur par defaut:\nvitesse de Tr.:9600, nbre de bit de donnees: 8, parite: aucun, stop bit: 1\n");
		scanf_s("%d", &valdef);

		if(valdef==0)
		{
		
			printf("*********** Parametrage du port serie ***********\n");
        	printf("Entrer la vitesse de transmission? (4800,9600,19200) \n");
       		scanf_s("%d", &iBaudRate);

			printf("Entrer le nombre de bits de donnees? (5-8) \n");
        	scanf_s("%d", &iBiteSize);

			printf("Entrer la parite? 0 (pas de parite) / 1 (Parite impair) / 2 (Partie pair)\n");
        	scanf_s("%d", &iParite);

        	printf("Entrer le nombre de bist de stop? (0 (1 bit) / 1 (1.5 bits) / 2 (2 bits)\n");
        	scanf_s("%d", &iStopBit);
		
		}

		

        connexionOk = setParamSerialPort(handleSerialPort, iBaudRate,(BYTE)iBiteSize,(BYTE)iParite,(BYTE)iStopBit);
        if (connexionOk)
            printf("Connexion OK\n");
		else
            printf("Connexion KO - Erreur lor du parametrage\n");
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

			startAdress=StartAdressLectureDef();
			startAdress=choixDevoie(startAdress);			
			
            //**
			//la resolution etant de 16 bits le nombre de mot est donc 2  
			nbParamsToread=1;
		

			//Creation de la trame de lecture modbus=>address 1 |function 03 |adresse de debut|nombre de parametre 2| CRC
			lengthTrameSend= makeTrameLecModBus(1,MODBUS_FUNCTION_READ_NWORDS,startAdress,nbParamsToread,i_trameSend,INTEL); 

			break;}

		// Demande d'ecriture
		case REQUEST_WRITE:{

			int typeValCopy;

			printf("\n DEMANDE D'ECRITURE\n");

			startAdress=StartAdressEcrituretureDef();

			*i_typeVal=TYPE_SHORT;
	
			//Selon le type de la valeur on va recuperer un short un int ou un float 
			if(*i_typeVal == TYPE_SHORT)
			{
				
				printf("Entrer la valeur ");
				scanf("%hi",&valToWriteShort);


				//Creation de la trame d ecriture  modbus pour short (ecriture 1 mot )
				lengthTrameSend = makeTrameEcrModBusFromShort(1,MODBUS_FUNCTION_WRITE_WORD,startAdress,valToWriteShort,i_trameSend,INTEL); 
				
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

	short shortNum;
	float floatNum;
	int intNum;
    // A COMPLETER

	codret=parseTrameModBus(i_trameReceive,i_lengthTrameReceived,value,&nbValue,&address,&codeFunction,INTEL);

	//on prend que la partie avec la donnee pour l afficher 
	 char *asciiDonnee=i_trameReceive+3;
	
	if(i_requestType==REQUEST_READ){
			
		//pour le module EP2407 on sait que le type de donne c est short
		shortNum=ModBusShortAsciiToIeee(asciiDonnee,INTEL) ; 
		printf(" La valeur : %hu",shortNum);


	}





	return codret;
}
