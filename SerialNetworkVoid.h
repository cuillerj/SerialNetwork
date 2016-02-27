
void DataToSendSerial(){

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
      trySendSerial[1]=trySendSerial[1]+1;
    }
	acqSerial=uint8_t(pendingNumberSerial);
    if (trySendSerial[1]<=1)   // 
    {
		msgSerial[7]=acqSerial;
		DataLenSerial=min(PendingDataLenSerial,sizeReqSerial);
		msgSerial[8]=uint8_t (DataLenSerial);

		for (int i=0;i<min(PendingDataLenSerial,sizeReqSerial);i++){
			msgSerial[i+9]=PendingDataReqSerial[i];

		}

#if defined(debugConnection)
		Serial.println();
		for (int i=0;i<9+DataLenSerial;i++)
		{
			Serial.print(msgSerial[i],HEX);
			Serial.print("-");
		}
		Serial.println();
#endif
		Serial2.write((uint8_t *)msgSerial,9+DataLenSerial);
		tSendSerial=millis();
		trySendSerial[1]=0x01; // force ack recu avant eventuel developpement ack par esp
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

uint8_t TrameAnalyzeSlaveSerial(){

}

uint8_t CheckUnitIndSerial(){
	uint8_t retCode=0x00;
	retCode=PendingSecReqSerial;
  if (PendingSecReqSerial != 0){
    PendingReqSerial=PendingSecReqSerial;

	  for (int i=0;i<sizeof(sizeReqSerial);i++)
  {
	PendingDataReqSerial[i]=PendingSecDataReqSerial[i];

  }

    PendingDataLenSerial=PendingSecDataLenSerial;
//	 pendingNumberSerial=pendingNumberSerial+1;

  }

return retCode;
}
/*
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
*/
void SendSecuSerial(){

  PendingSecReqSerial=PendingReqSerial; // copy in case retry is needed
  for (int i=0;i<sizeof(sizeReqSerial-1);i++)
  {
	PendingSecDataReqSerial[i]=PendingDataReqSerial[i]; // copy in case retry is needed
  }

  PendingSecDataLenSerial=PendingDataLenSerial;  // copy in case retry is needed
//  pendingNumberSerial=pendingNumberSerial+1;
//  bcl6=1;
}
void ReSendSecuSerial(){
	pendingNumberSerial=pendingNumberSerial-1;
    PendingReqSerial=PendingSecReqSerial;
	  for (int i=0;i<sizeof(sizeReqSerial-1);i++)
  {
	PendingDataReqSerial[i]=PendingSecDataReqSerial[i];
	  }
    PendingDataLenSerial=PendingSecDataLenSerial;
//	trySendSerial[0]=0x00;
	trySendSerial[1]=0x01;
  }
  int Serial_have_message(){
	  int len=0;
	    if (Serial2.available() )
  {

    while (Serial2.available())
    {
      byte In1 = (Serial2.read());

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
