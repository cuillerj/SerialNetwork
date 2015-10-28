// Arduino side
/* format du message
 ERCAMessK - E adresse emetteur sur 1 octet (HEX) R adresse recepteur -  C commande sur 1 octet (HEX) - A acquis/ reponse comande 1 octet- Mess Message(max 256 octets)(char) - longueur du message (hex)
 dans la phase d'init le master scrute le reseau a la recherche de stations dans la limite de MaxStations via le commande FF
 attend en retour FFAMessK (K = 1) Mess = type du recepteur 
 // numerique 3 5 6 utilise par virtuallib
 */
//  needed for RF434 network  >>
#ifndef SerialNetworkVariable_h_included
#define SerialNetworkVariable_h_included

uint8_t addrSSerial=0x01; // station address par defaut
uint8_t addrMSerial=0x00;  // adresse Master 
uint8_t addrRSerial=0x00;  // destinataire
uint8_t cmdeSerial=0x00;  // commande (FF init comm, EF polling
uint8_t acqSerial=0x00;   // acq de la commande 
uint8_t dlenSerial=0x00;  // data length (start 6th Byte not longer than 20 Bytes)
uint8_t StationTypeSerial=0x00; // type of station 
uint8_t PendingReqSerial=0x00; // pending request (if 0x00 none)
uint8_t PendingDataReqSerial[20]; // pending data request to send master
uint8_t PendingDataLenSerial;
uint8_t PendingSecReqSerial=0x00; // pending request (if 0x00 none) - copie pour retry
uint8_t PendingSecDataReqSerial[20]; // pending data request to send master- copie pour retry
uint8_t PendingSecDataLenSerial;
uint8_t pendingNumberSerial=0x00;
uint8_t lastSentNumberSerial=0x00;
uint8_t trySendSerial[2]={
  0x00,0x00};
uint8_t lastReceivedNumberSerial=0x00;
uint8_t acqRSerial=0x00;
 // en reception sur serie
uint8_t frameFlag=0x00; 
int frameLen=0;
int frameCount=0;
uint8_t bufParam[3];
//uint8_t PendingDataIn[10]; // pending data request 
int ConnectedSerial=1; // flag connexion force a 1 avant developpeme,t polling
/*
unsigned int TrafficStationsOutSerial; // count output trames - unsigned le 09/03/14
unsigned int AddressedTrameSerial; // index trames ciblees
unsigned int NewInputSerial;  // index have a message 
unsigned int NewBufSerial; // index message a contenu
unsigned int NewOctSerial; // index octet received
unsigned int SavStationsOutSerial;  // pour controle du fonctionnement reseau
*/
//uint8_t reqStatNetwSerial=0x00;   // flag remontee stat network necessaire
uint16_t SpeedNetwSerial=19200; // vitessse reception tunned pour minimiser les erreurs 
int CheckReceiveSerial=100; // definit le cycle de check receive data sur RF434
unsigned long tSendSerial;
unsigned long tOutReceiveMasterSerial;
float Count1Serial; // zone de travail pour stat
float Count2Serial; // zone de travail pour stat
int DataLenSerial=0; // data input detected length
//String val;
//String data=0x00;
//char c=0;
uint8_t msgSerial[31]; // message in and out maxi 27 bytes including 0x7f+0x7e +0x7e+0x7f+header(5)
uint8_t DataInSerial[31]; // data input (max 27)

//


#endif

// ***************************************************************************************************
