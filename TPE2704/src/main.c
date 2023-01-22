#include <string.h>
#include <stdio.h>
#include <sys/types.h>

#include "TP3.h"
#include "Modbus.h"

int main (int argc, char** argv)
{
    int isSoketPort = 0;
    BOOL connexionOk = FALSE;

    SOCKET idConnexionSocket = INVALID_SOCKET;
    HANDLE handleSerialPort = NULL;

    printf("****************************************************************************\n");
    printf("*                      PROTOCOLE MODBUS LIAISON SERIE                      *\n");
    printf("****************************************************************************\n");

    //*******************************************************************************
        // Creation et ouverture du support de communication
    if (isSoketPort)
        idConnexionSocket = connectionTCPIpPort();
    else
        handleSerialPort = connectionSerialPort();
    //*******************************************************************************

    if (handleSerialPort || (idConnexionSocket != INVALID_SOCKET) )
    {
        TypeRequest requestType = NO_REQUEST;
        TypeVal typeVal = NO_TYPE;

        while (requestType != REQUEST_QUIT)
        {
            char trameToSend[100];
            int lengthTrameToSend = 0;
            char trameReceived[100];
            int lengthTrameReceived = 99;
            memset(trameReceived,'\0',sizeof(trameReceived));

            int i = 0;
            ErrorComm codret = ERRORCOMM_ERROR;

            printf("\n****************************************************************************\n");
            printf("   1. Demande de lecture.\n");
            printf("   2. Demande d'ecriture.\n");
            printf("   3. Quitter.\n");
            printf("Que souhaitez-vous faire?\n");
            scanf("%d", &requestType);

            //*******************************************************************************
            // Creation de la trame de requete Modbus
            if (requestType == REQUEST_READ || requestType == REQUEST_WRITE)
                lengthTrameToSend = createRequestTrame(requestType, trameToSend, &typeVal);
                
            else
                continue;

            if (isSoketPort)
                lengthTrameToSend = ModbusSerialToTCPIP(trameToSend, lengthTrameToSend, INTEL);

            printf("\n Send trame (length = %i):", lengthTrameToSend);
            for (i = 0; i < lengthTrameToSend ; i++)
            {
                printf("%02X ",((unsigned char)trameToSend[i]));
            }
            printf("\n");

            //*******************************************************************************
            // Envoie de la requete Modbus sur le supporte de communication et reception de la trame reponse
            if (lengthTrameToSend)
            {
                 // A COMPLETER
                //envoie de la trame creer contenant le type de requete READ OR Write
                 codret = sendSerialPort(handleSerialPort, 1000, trameToSend, lengthTrameToSend);

                  
                  //reception de la trame 
                 if(codret == ERRORCOMM_NOERROR){
                    
                    
                    codret = recvSerialPort(handleSerialPort, 1000, trameReceived,&lengthTrameReceived);
   

                    }
                 
                                
            }

            //*******************************************************************************
            //Decodage de la trame reçue
            if (codret!=ERRORCOMM_NOERROR || lengthTrameReceived==0)
            {
                printState(codret);
            }
            else
            {
                printf("\n Receive trame (length = %i): ", lengthTrameReceived);
                for  (i = 0; i < lengthTrameReceived; i++)
                    printf("%02X ",(unsigned char)trameReceived[i]);
                printf("\n");

                if (isSoketPort)
                    lengthTrameReceived = ModbusTCPIPToSerial(trameReceived, lengthTrameReceived, INTEL);

                    codret = parseModbusResponse(trameReceived, lengthTrameReceived, requestType, typeVal);

                if (codret != ERRORCOMM_NOERROR)
                    printState(codret);
            //*******************************************************************************
            }
        }

        //*******************************************************************************
        // Fermeture du support de communication
        
        // A COMPLETER
        // Fermeture du support de communication
        terminateSerialPort(handleSerialPort);

        
        //*******************************************************************************
    }
    else
        printf("Echec de connexion.");

    return 0;
}