/*
void AcqTrameSerial()
{
//  Serial.print("S ack:");
//  Serial.println(acq);
  acqRSerial=acqSerial;
  msgSerial[0]=0x7f;
  msgSerial[1]=0x7e;
  msgSerial[2]=addrSSerial;
  msgSerial[3]=addrMSerial;
  msgSerial[4]=0xEF;
  msgSerial[5]=acqSerial;
  msgSerial[6]=0x00;
  msgSerial[7]=0x7e;
  msgSerial[8]=0x7f;
//  vw_send((uint8_t *)msgSerial, 5);
//  vw_wait_tx(); 
  for (int i=0;i<8;i++)
  {
//	  Serial2.println(msgSerial[i]);
  }
  TrafficStationsOutSerial= TrafficStationsOutSerial+1;
}
*/
void DataToSendSerial(){
 // if (ConnectedSerial!=0x00 && millis()-tSendSerial> 1000 + random(trySendSerial[1]*500,trySendSerial[1]*1000))
	    if (ConnectedSerial!=0x00 )
  {
	msgSerial[0]=0x7f;
    msgSerial[1]=0x7e;
	msgSerial[2]=0x7f;
    msgSerial[3]=0x7e;
    msgSerial[4]=addrSSerial;
    msgSerial[5]=addrMSerial;
    msgSerial[6]=PendingReqSerial;
 
	if (trySendSerial[1]==0x00)
	{
	pendingNumberSerial=pendingNumberSerial+1;
	trySendSerial[0]=pendingNumberSerial;
	trySendSerial[1]=0x00;
	}

    if (trySendSerial[0]!=pendingNumberSerial)
    {
      trySendSerial[0]=pendingNumberSerial;
      trySendSerial[1]=0x01;
    }
    else
    {

      //    delay(200);
      trySendSerial[1]=trySendSerial[1]+1;
    }
	acqSerial=uint8_t(pendingNumberSerial);
    if (trySendSerial[1]<=1)   // 
    {
      //   msgSerial[3]=uint8_t(acqSerial); // 

		msgSerial[7]=acqSerial;
//		Serial.print("sent number:");
//   Serial.print(pendingNumberSerial);
//     Serial.print("retry:");
//     Serial.print(trySendSerial[1]);
//    Serial.print(" sent ack:");
//      Serial.print(lastSentNumber);
//    Serial.print(" deltaT:");
//      Serial.println(millis()-tSend);
 //     msgSerial[5]=StationType;
		DataLenSerial=min(PendingDataLenSerial,20);
		msgSerial[8]=uint8_t (DataLenSerial);
		for (int i=0;i<min(PendingDataLenSerial,20);i++){
			msgSerial[i+9]=PendingDataReqSerial[i];
		}
//	  	msgSerial[9+min(PendingDataLenSerial,20)]=0x7f;
//		msgSerial[9+min(PendingDataLenSerial,20)+1]=0x7e;
	  /*
	  Serial.print("S:");

     for (int i=0;i<PendingDataLenSerial+9;i++){
     Serial.print(msgSerial[i],HEX);
      Serial.print("-");
     }
   Serial.println("");
*/
//      vw_send((uint8_t *)msgSerial, 5+DataLenSerial);
 //     vw_wait_tx(); 
//	    for (int i=0;i<9+DataLenSerial;i++)
//		{
			Serial2.write((uint8_t *)msgSerial,9+DataLenSerial);
//		}
//		delay(50);
		 tSendSerial=millis();
      //   PendingReq=0x00;
      //   PendingDataLenSerial=0;
      //      digitalWrite(13, false);
//		TrafficStationsOutSerial= TrafficStationsOutSerial+1;
		trySendSerial[1]=0x01; // force ack recu avant developpement reception
//	    PendingReqSerial=0x00;
    }
    else
    {
      PendingReqSerial=0x00;
      PendingDataLenSerial=0;
      trySendSerial[0]=0x00;
      trySendSerial[1]=0x00;

    }
  }
}
/*
void StatSlaveNetwSerial(){
  reqStatNetwSerial=0x00;
  Count1=NewBufSerial;
  Count2=NewInputSerial;
  Count1=(Count2-Count1)/Count2*100;
  Serial.println(Count1,3);
  PendingReqSerial=0x82;   // pour affichage par le master + envoi UDP -
  PendingDataReqSerial[0]=0x54;  // reponse a demande de statistique
  PendingDataReqSerial[1]=uint8_t(TrafficStationsOutSerial/256);
  PendingDataReqSerial[2]=uint8_t(TrafficStationsOutSerial);
  PendingDataReqSerial[3]=uint8_t(NewInputSerial/256);
  PendingDataReqSerial[4]=uint8_t(NewInputSerial);
  PendingDataReqSerial[5]=uint8_t(NewBufSerial/256);
  PendingDataReqSerial[6]=uint8_t(NewBufSerial);
  PendingDataReqSerial[7]=uint8_t(NewOctSerial/256);
  PendingDataReqSerial[8]=uint8_t(NewOctSerial);
  PendingDataReqSerial[9]=uint8_t(AddressedTrame/256);
  PendingDataReqSerial[10]=uint8_t(AddressedTrame);
  PendingDataReqSerial[11]=uint8_t(int(Count1));
  PendingDataLenSerial=0x0C;
  TrafficStationsOutSerial=0; // raz des compteurs
  NewInputSerial=0; // modif le 03/10/14 deplace de statslave
  NewBufSerial=0;
  NewOctSerial=0;
  AddressedTrame=0;
  Count1=NewBufSerial;
//  pendingNumberSerial=pendingNumberSerial+1;
}
*/
uint8_t TrameAnalyzeSlaveSerial(){
	/*
  uint8_t retCode=0x00;
  NewInputSerial=NewInputSerial+1;
  // String val = "";
  char c=0;

  if (Serial2.available()){
    NewBufSerial=NewBufSerial+1;
  }
  else
  {
    return(retCode);
  }
  if (NewBufSerial>=64000){
    reqStatNetwSerial=0xff;
  }
 // 	Serial.print("R:");
  for(int i = 0; i < buflen; i++){
    NewOctSerial=NewOctSerial+1;

    if (NewOctSerial>=64000){
      reqStatNetwSerial=0xff;
    }

    c += buf[i];
 //   Serial.print(buf[i], HEX);
 //  Serial.print(":");
    //  val += c;
  }
//  Serial.println("");
  //    addrMSerial=buf[0];
  addrRSerial=buf[1];
  cmdeSerial=buf[2];
  acqSerial=buf[3];
  dlen=buf[4];
  //Serial.println(buflen);
  for(int i = 0; i < min(buflen-5,20); i++){
   DataIn[i]= buf[i+5];
 //       Serial.print(DataIn[i],HEX);
  }
  if (addrRSerial==addrSSerial){
	  AddressedTrame=AddressedTrame+1;
    if (ConnectedSerial==0){
      addrMSerial=buf[0];
      Serial.print("Cnx with: ");
      Serial.println(addrMSerial);
      ConnectedSerial=1;
      //       bitWrite(Diag,7,0);
      //     DemandStatus();
    }

    if (cmdeSerial==0xff)

    { // reponse a init
      tOutReceiveMaster=millis();
//      Serial.println("S alive");
      msgSerial[0]=addrSSerial;
      msgSerial[1]=addrMSerial;
      msgSerial[2]=cmdeSerial;
      msgSerial[3]=0x01; // reponse positive
      msgSerial[4]=0x01;
      msgSerial[5]=StationType;
      DataLenSerial=1;
      //      digitalWrite(13, true);
	  tSendSerial=millis();
//      vw_send((uint8_t *)msgSerial, 5+DataLenSerial);
//      vw_wait_tx(); 
	    for (int i=0;i<5+DataLenSerial;i++)
  {
//	  Serial2.println(msgSerial[i]);
  }
      //      digitalWrite(13, false);
      TrafficStationsOutSerial= TrafficStationsOutSerial+1;

    }
    if (cmdeSerial==0xef)
    {// acquis d une trame
      tOutReceiveMaster=millis();
 //    Serial.print("receive ack:");
 //    Serial.println(acqSerial);
      lastSentNumber=acqSerial;
      if (acqSerial>=pendingNumberSerial%256)
      {
        PendingReqSerial=0x00;
        PendingDataLenSerial=0;
		trySendSerial[1]=0x00;
      }

    }
	    if (cmdeSerial==0x02)
		{
          PendingSecReq=0x00;
		  PendingReqSerial=0x00;
		  PendingDataLenSerial=0;
   
		}
    if (cmdeSerial!=0xef && cmdeSerial!=0xff)
    {
      AcqTrameSerial();

      if (lastReceivedNumber!=acqRSerial) // traiter si trame pas deja reçue
      {
        lastReceivedNumber=acqRSerial;
        retCode=cmdeSerial;
      }

    }
  }
  return(retCode);
  */
}
/*
void StatSlaveSerial(){
  Serial.print("TramT:");
  Serial.print(TrafficStationsOutSerial);
  Serial.print(" -TramR:");
  Serial.print(NewInputSerial);
  Serial.print(" -TramOkR:");
  Serial.print(NewBufSerial);
  Serial.print(" -BytR:");
  Serial.print(NewOctSerial);
 Serial.print(" -Addr:");
  Serial.print(AddressedTrame);
  Serial.print(" -%Bad:");
  if (SavStationsOut==TrafficStationsOutSerial) // pas de traffic reseau depuis derniere sta
  {         
 //   bitWrite(Diag,7,1);   // force bit RF434 ko   
    ConnectedSerial=0;  // indic deconnexion
  }
  SavStationsOut=TrafficStationsOutSerial;
  Count1=NewBufSerial;
  Count2=NewInputSerial;
  Count1=(Count2-Count1)/Count2*100;
  Serial.println(Count1,3);
}
*/
/*
void RequestTimeFromSerial(){
  Serial.println("reqTime");
  //  bitWrite(Diag,4,1);         // position bit diag
  PendingReqSerial=0xb2;     // service n 2
  PendingDataReqSerial[0]=0x01;
  PendingDataLenSerial=0x01;
  pendingNumberSerial=pendingNumberSerial+1;
}
*/
uint8_t CheckUnitIndSerial(){
	uint8_t retCode=0x00;
	retCode=PendingSecReqSerial;
  if (PendingSecReqSerial != 0){
    PendingReqSerial=PendingSecReqSerial;

	  for (int i=0;i<sizeof(PendingDataReqSerial);i++)
  {
	PendingDataReqSerial[i]=PendingSecDataReqSerial[i];

  }

    PendingDataLenSerial=PendingSecDataLenSerial;
//	 pendingNumberSerial=pendingNumberSerial+1;

  }
//		Serial.print("UInd");
//  		Serial.println(retCode);
return retCode;
}
void UnitIndSerial(){
  // a completer pour passer en parametre n° indic + valeur ?
  //      Serial.print("relais: ");
  //      Serial.println(RelaisStatus,HEX);
  //PendingDataLenSerial=0x03;
 // Serial.println("unitind");
  PendingDataReqSerial[0]=0x02;  // remontee data unitaire
  PendingSecReqSerial=PendingReqSerial; // routage UDP
  PendingSecDataReqSerial[0]=PendingDataReqSerial[0];  // 
  PendingSecDataReqSerial[1]=PendingDataReqSerial[1];  // n° indicateur
  PendingSecDataReqSerial[2]=PendingDataReqSerial[2];
  PendingSecDataLenSerial=PendingDataLenSerial;
//  pendingNumberSerial=pendingNumberSerial+1;
//  bcl6=1;
}
void SendSecuSerial(){
  // a completer pour passer en parametre n° indic + valeur ?
  //      Serial.print("relais: ");
  //      Serial.println(RelaisStatus,HEX);
  //PendingDataLenSerial=0x03;
 // Serial.println("unitind");
//  PendingDataReqSerial[0]=0x02;  // remontee data unitaire
  PendingSecReqSerial=PendingReqSerial; // routage UDP
  for (int i=0;i<sizeof(PendingSecDataReqSerial);i++)
  {
	PendingSecDataReqSerial[i]=PendingDataReqSerial[i];
  }
  //      Serial.print("relais: ");
  //      Serial.println(RelaisStatus,HEX);
  PendingSecDataLenSerial=PendingDataLenSerial;
//  pendingNumberSerial=pendingNumberSerial+1;
//  bcl6=1;
}
void ReSendSecuSerial(){
    PendingReqSerial=PendingSecReqSerial;
	  for (int i=0;i<sizeof(PendingDataReqSerial);i++)
  {
	PendingDataReqSerial[i]=PendingSecDataReqSerial[i];
	  }
    PendingDataLenSerial=PendingSecDataLenSerial;
//	 pendingNumberSerial=pendingNumberSerial+1;
	trySendSerial[1]=0x00;
  }
  int Serial_have_message(){
	  int len=0;
	    if (Serial2.available() )
  {
    //  delay(20);
//    Serial.print("serial2:");
    while (Serial2.available())
    {
      byte In1 = (Serial2.read());

   //   Serial.print(In1, HEX);
   //   Serial.print("-");
			switch (In1){
				case 0x7f:
					if (frameFlag==0)
						{
							frameFlag=1;
							break;
						}
                    if (frameFlag==2)
                        {
                            frameFlag=3;
                            break;
						}
                    if (frameFlag==4)
                        {
                            frameFlag=5;
                            break;
						}
                     if (frameFlag==6)
                        {
                        frameFlag=7;
                        break;
                        }


				case 0x7e:
					if (frameFlag==3)
						{
							frameFlag=8;
							frameLen=0;
							frameCount=0;
							break;
						}
					if (frameFlag==1)
						{
                             frameFlag=2;

        						break;
						}
                    if (frameFlag==5)
                       {
                            frameFlag=6;
                            break;
                       }
                    if (frameFlag==7)
                       {
                            frameFlag=8;
                            frameLen=0;
                            frameCount=0;
                            break;
                        }

				default:
				/*
						Serial.print("flag:");
						Serial.println(frameFlag);
												Serial.print("count:");
						Serial.println(frameCount);
												Serial.print("len:");
						Serial.println(frameLen);
				*/		

                    if (frameFlag==5 || frameFlag==6 || frameFlag==7 || frameFlag==8)
                        {
                            frameFlag=4;
                            break;
						}
			}

					switch (frameFlag)
						{

						case 4:
 						frameCount=frameCount+1;


						switch (frameCount)
								{
									case 1:
										frameLen=In1;
										break;
/*									
*/																				
                                    default:
                           			DataInSerial[frameCount-2]=In1;
									if (frameCount==frameLen+1 )
									{
//							Serial.println("frame");
										len=frameLen;
									}
                                    break;

								}

						default:
						break;
						}

//						Out=true;
    }
//    Serial.println("");
  }
  return (len);
  }
  
// ***************************************************************************************************
