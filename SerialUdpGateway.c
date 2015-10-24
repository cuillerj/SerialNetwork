// raspberry side
// impossible de  gerer les  interruption a la fois cote serie et udp 
// soit l'un soit l autre ok
// bidouille mise en place > forcer  l arduino a evnoyer du traffic sur la liaison serie pour lire UDP
//  code plein de chose inutiles !!
void io_handler(int);
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#define BUFSIZE 1024
//#define serveraddr  "192.168.1.5"
//#define portno  1830
#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttyUSB0"
#define _POSIX_SOURCE 1         //POSIX compliant source
#define FALSE 0
#define TRUE 1
#define SERVER "192.168.1.5"
#define PORT 1830   //The port on which to send data
struct sockaddr_in si_other;
int s, slen=sizeof(si_other);
int sockfd;
struct sockaddr_in myaddr;
struct sockaddr_in client_addr; /* the address of client  */
socklen_t len;
//int io_handler();
//void io_handler(int signal);
//signal(SIGIO, io_handler);
char mesg[1000];
char buf[BUFSIZE];
volatile int STOP=FALSE;
void signal_handler_IO (int status);    //definition of signal handler
void sigintHandler(int sig_num);   // ajout JC
void SendToUdp(int strlen); 
//void io_handler(int);
//void die(char *s);
int wait_flag=TRUE;                     //TRUE while no signal received
char devicename[80];
long Baud_Rate = 19200;         // default Baud Rate (110 through 38400)
long BAUD;                      // derived baud rate from command line
long DATABITS;
long STOPBITS;
long PARITYON;
long PARITY;
int Data_Bits = 8;              // Number of data bits
int Stop_Bits = 1;              // Number of stop bits
int Parity = 0;                 // Parity as follows:
                  // 00 = NONE, 01 = Odd, 02 = Even, 03 = Mark, 04 = Space
int Format = 4;
FILE *input;
FILE *output;
int status;
int errorJC=99;
int frameFlag=0;
int frameLen=0;
int frameCount=0;
int udpFlag=0;
char bufUdp[255];
char bufParam[3];
char bufSerialOut[255];
main(int Parm_Count, char *Parms[])
{
   char version[80] = "       POSIX compliant Communications test program version 1.00 4-25-1999\r\n";
   char version1[80] = "          Copyright(C) Mark Zehner/Peter Baumann 1999\r\n";
   char version2[80] = " This code is based on a DOS based test program by Mark Zehner and a Serial\r\n";
   char version3[80] = " Programming POSIX howto by Peter Baumann, integrated by Mark Zehner\r\n";  
   char version4[80] = " This program allows you to send characters out the specified port by typing\r\n";
   char version5[80] = " on the keyboard.  Characters typed will be echoed to the console, and \r\n";
   char version6[80] = " characters received will be echoed to the console.\r\n";
   char version7[80] = " The setup parameters for the device name, receive data format, baud rate\r\n";
   char version8[80] = " and other serial port parameters must be entered on the command line \r\n";
   char version9[80] = " To see how to do this, just type the name of this program. \r\n";
   char version10[80] = " This program is free software; you can redistribute it and/or modify it\r\n";
   char version11[80] = " under the terms of the GNU General Public License as published by the \r\n";
   char version12[80] = " Free Software Foundation, version 2.\r\n";
   char version13[80] = " This program comes with ABSOLUTELY NO WARRANTY.\r\n";
   char instr[100] ="\r\nOn the command you must include six items in the following order, they are:\r\n";
   char instr1[80] ="   1.  The device name      Ex: ttyS0 for com1, ttyS1 for com2, etc\r\n";
   char instr2[80] ="   2.  Baud Rate            Ex: 38400 \r\n";
   char instr3[80] ="   3.  Number of Data Bits  Ex: 8 \r\n";
   char instr4[80] ="   4.  Number of Stop Bits  Ex: 0 or 1\r\n";
   char instr5[80] ="   5.  Parity               Ex: 0=none, 1=odd, 2=even\r\n";
   char instr6[80] ="   6.  Format of data received:  1=hex, 2=dec, 3=hex/asc, 4=dec/asc, 5=asc\r\n";
   char instr7[80] =" Example command line:  com ttyS0 38400 8 0 0 4 \r\n";
   char Param_strings[7][80];
   char message[90];
   signal(SIGINT, sigintHandler);
   signal(SIGSTOP, sigintHandler);   
   int fd, tty, c, res, i, error;
   char In1, Key;
   struct termios oldtio, newtio;       //place for old and new port settings for serial port
   struct termios oldkey, newkey;       //place tor old and new port settings for keyboard teletype
   struct sigaction saio;               //definition of signal action
                       //buffer for where data is put
/*   
   input = fopen("/dev/tty", "r");      //open the terminal keyboard
   output = fopen("/dev/tty", "w");     //open the terminal screen

   if (!input || !output)
   {
      fprintf(stderr, "Unable to open /dev/tty\n");
      exit(1);
   }
   error=0;
   fputs(version,output);               //display the program introduction
   fputs(version1,output);
   fputs(version2,output);
   fputs(version3,output);
   fputs(version4,output);
   fputs(version5,output);
   fputs(version6,output);
   fputs(version7,output);
   fputs(version8,output);
   fputs(version9,output);
   fputs(version10,output);
   fputs(version11,output); 
   fputs(version12,output);
   fputs(version13,output);
   errorJC=Parm_Count;
   //read the parameters from the command line
*/  
 if (Parm_Count==7)  //if there are the right number of parameters on the command line
   {
   errorJC=98;
 for (i=1; i<Parm_Count; i++)  // for all wild search parameters
      {
         strcpy(Param_strings[i-1],Parms[i]);
      }
      i=sscanf(Param_strings[0],"%s",devicename);
      if (i != 1) error=1;
      i=sscanf(Param_strings[1],"%li",&Baud_Rate);
      if (i != 1) error=2;
      i=sscanf(Param_strings[2],"%i",&Data_Bits);
      if (i != 1) error=3;
      i=sscanf(Param_strings[3],"%i",&Stop_Bits);
      if (i != 1) error=4;
      i=sscanf(Param_strings[4],"%i",&Parity);
      if (i != 1) error=5;
      i=sscanf(Param_strings[5],"%i",&Format);
      if (i != 1) error=6;
      errorJC=error;
/* 
     sprintf(message,"Device=%s, Baud=%li\r\n",devicename, Baud_Rate); //output the received setup parameters
      fputs(message,output);
      sprintf(message,"Data Bits=%i  Stop Bits=%i  Parity=%i  Format=%i\r\n",Data_Bits, Stop_Bits, Parity, Format);
      fputs(message,output);
*/
   }  //end of if param_count==7
   if ((Parm_Count==7) && (error==0))  //if the command line entrys were correct
   {
/*                                    //run the program
      tty = open("/dev/tty", O_RDWR | O_NOCTTY | O_NONBLOCK); //set the user console port up
      tcgetattr(tty,&oldkey); // save current port settings   //so commands are interpreted right for this program
      // set new port settings for non-canonical input processing  //must be NOCTTY
      newkey.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
      newkey.c_iflag = IGNPAR;
      newkey.c_oflag = 0;
      newkey.c_lflag = 0;       //ICANON;
      newkey.c_cc[VMIN]=1;
      newkey.c_cc[VTIME]=0;
      tcflush(tty, TCIFLUSH);
      tcsetattr(tty,TCSANOW,&newkey);
*/
      switch (Baud_Rate)
      {
         case 38400:
         default:
            BAUD = B38400;
            break;
         case 19200:
            BAUD  = B19200;
            break;
         case 9600:
            BAUD  = B9600;
            break;
         case 4800:
            BAUD  = B4800;
            break;
         case 2400:
            BAUD  = B2400;
            break;
         case 1800:
            BAUD  = B1800;
            break;
         case 1200:
            BAUD  = B1200;
            break;
         case 600:
            BAUD  = B600;
            break;
         case 300:
            BAUD  = B300;
            break;
         case 200:
            BAUD  = B200;
            break;
         case 150:
            BAUD  = B150;
            break;
         case 134:
            BAUD  = B134;
            break;
         case 110:
            BAUD  = B110;
            break;
         case 75:
            BAUD  = B75;
            break;
         case 50:
            BAUD  = B50;
            break;
     }  //end of switch baud_rate
      switch (Data_Bits)
      {
         case 8:
         default:
            DATABITS = CS8;
            break;
         case 7:
            DATABITS = CS7;
            break;
         case 6:
            DATABITS = CS6;
            break;
         case 5:
            DATABITS = CS5;
            break;
      }  //end of switch data_bits
      switch (Stop_Bits)
      {
         case 1:
         default:
            STOPBITS = 0;
            break;
         case 2:
            STOPBITS = CSTOPB;
            break;
      }  //end of switch stop bits
      switch (Parity)
      {
         case 0:
         default:                       //none
            PARITYON = 0;
            PARITY = 0;
            break;
         case 1:                        //odd
            PARITYON = PARENB;
            PARITY = PARODD;
            break;
         case 2:                        //even
            PARITYON = PARENB;
            PARITY = 0;
            break;
      }  //end of switch parity
       
      //open the device(com port) to be non-blocking (read will return immediately)
      fd = open(devicename, O_RDWR | O_NOCTTY | O_NONBLOCK);
      if (fd < 0)
      {
         perror(devicename);
         exit(-1);
      }
	int rc=0;
	bufSerialOut[1]=0x7f;
	bufSerialOut[2]=0x7e;
	bufSerialOut[3]=0x7f;
	bufSerialOut[4]=0x7e;
//	rc=write(fd,"start",5);
//	scanf("%d",&rc);
//	printf("start %d\n",rc);
      //install the serial handler before making the device asynchronous
// signal(SIGIO,io_handler);

      saio.sa_handler = signal_handler_IO;
      sigemptyset(&saio.sa_mask);   //saio.sa_mask = 0;
      saio.sa_flags = 0;
      saio.sa_restorer = NULL;
      sigaction(SIGIO,&saio,NULL);

      // allow the process to receive SIGIO
      fcntl(fd, F_SETOWN, getpid());
      // Make the file descriptor asynchronous (the manual page says only
      // O_APPEND and O_NONBLOCK, will work with F_SETFL...)
      fcntl(fd, F_SETFL, FASYNC);

      tcgetattr(fd,&oldtio); // save current port settings 
      // set new port settings for canonical input processing 
      newtio.c_cflag = BAUD | DATABITS | STOPBITS | PARITYON | PARITY | CLOCAL | CREAD;
      newtio.c_iflag = IGNPAR;
      newtio.c_oflag = 0;
      newtio.c_lflag = 0;       //ICANON;
      newtio.c_cc[VMIN]=1;
      newtio.c_cc[VTIME]=0;
      tcflush(fd, TCIFLUSH);
      tcsetattr(fd,TCSANOW,&newtio);
//
   // convert (host, port) to a struct sockaddr
     sockfd=socket(AF_INET,SOCK_DGRAM,0);
     myaddr.sin_port=htons(8888);
                    bzero((char *)&myaddr, sizeof(myaddr));
               myaddr.sin_family = AF_INET;
               myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
     //          myaddr.sin_port = htons(SERV_PORT);
     myaddr.sin_port=htons(8888);    
	 bind(sockfd,(struct sockaddr *)&myaddr,sizeof(myaddr));
//	signal(SIGIO,io_handler);  
   char buf[BUFSIZE];
// second: set the process id or process group id that is to receive
	// notification of pending input to its own process id or process 
	// group id
//	signal(SIGIO, io_handler);
    pid_t pid = getpid();

    int a_flags = fcntl(sockfd, F_GETFL);
    fcntl(sockfd, F_SETFL, a_flags | O_ASYNC);
    struct sigaction a_sa;
    a_sa.sa_flags = 0;
    a_sa.sa_handler = signal_handler_IO;
    sigemptyset(&a_sa.sa_mask);

    sigaction(SIGIO, &a_sa, NULL);
	if (fcntl(sockfd,F_SETOWN, getpid()) < 0){
		perror("fcntl F_SETOWN");
		exit(1);
	}

	// third: allow receipt of asynchronous I/O signals
	if (fcntl(sockfd,F_SETFL,FASYNC) <0 ){
		perror("fcntl F_SETFL, FASYNC");
		exit(1);
	}

int bcl=0;
      // loop while waiting for input. normally we would do something useful here
      while (STOP==FALSE)
      {
	while(1)
{
	if (bcl%10==0)
	{
		printf(".");
	}
	bcl=bcl+1;
   //      status = fread(&Key,1,1,input);
         if (status==1)  //if a key was hit
         {
            switch (Key)
            { /* branch to appropiate key handler */
               case 0x1b: /* Esc */
                  STOP=TRUE;
                  break;
               default:
  //                fputc((int) Key,output);
//                  sprintf(message,"%x ",Key);  //debug
//                  fputs(message,output);
                  write(fd,&Key,1);          //write 1 byte to the port
                  break;
            }  //end of switch key
         }  //end if a key was hit
         // after receiving SIGIO, wait_flag = FALSE, input is available and can be read
         if (wait_flag==FALSE)  //if input is available
         {
	wait_flag==TRUE; 
	 int i=0;
	for  (i=0;i<255;i++)
		{
			buf[i]=0x00;
		}

            res = read(fd,buf,255);
//	    printf("size:%d",res);
            if (res>0)
            {
               for (i=0; i<res; i++)  //for all chars in string
               {
                  In1 = buf[i];
		  switch(In1)
			{
//                                        printf("flag:%d",frameFlag);

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

					//	}
				case 0x7e:
					if (frameFlag==3)
						{
							frameFlag=8;
							printf(" +");
							frameLen=0;
							frameCount=0;
							udpFlag=0;
							break;
						}
					if (frameFlag==1)
						{
                                                        frameFlag=2;
                                                 //       printf(" fin");
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
                                                        printf(" +");
                                                        frameLen=0;
                                                        frameCount=0;
							udpFlag=0;
                                                        break;
                                                }

				default:
//                                printf(" flag:%d",frameFlag);

                                      if (frameFlag==5 || frameFlag==6 || frameFlag==7 || frameFlag==8)
                                              {
                                                        frameFlag=4;
                                                        break;
						}
				}

					switch (frameFlag)
						{
//						case 7:
//                                                case 6:
 //                                               case 5:
						case 4:
//                                                printf(" num:%d",frameCount);

							frameCount=frameCount+1;

							  printf(" num:%d",frameCount);
							switch (frameCount)
								{
									case 1:
										 printf(" addrS:%d",In1);
										bufParam[0]=In1;
										break;
									case 2:
                                                                                printf(" addrM:%d",In1);
										bufParam[1]=In1;
                                                                                break;
                                                                        case 3:
						
                                                                                 printf(" req:%d",In1);
                                                                                bufParam[2]=In1;
										break;
                                                                        case 4:
										bufParam[3]=In1;

                                                                                printf(" acq:%d",In1);
                                                                                break;
                                                                        case 5:
										frameLen=In1;
										bufParam[4]=In1;
                                                                                printf(" len:%d",In1);
                                                                                break;
                                                                        default:
                                                        			bufUdp[frameCount-6]=In1;
					                                        if (frameCount==(frameLen+5) && udpFlag==0 && bufParam[2]!=0xff)
                                        				        {
                                                        				SendToUdp(frameLen);
                                                        				udpFlag=1;
                                                        					printf("sendUdp");
                                                				}
                                                				if (bufParam[2]==0xff)
                                                        			{
                                                        				udpFlag=1;
                                                                        	}

                                                                                break;


								}

						default:
//						break;
//						}
                                          //    if (frameCount==(frameLen+6) && udpFlag==0 && bufParam[2]!=0xff)
					//	{
					//		SendToUdp(frameLen);
					//		udpFlag=1;
					//		printf("sendUdp");
					//	}
					//	if (bufParam[2]==0xff)
					//		{
					//		udpFlag=1;
					//				}
						break;
}
			}
                   //end of for all chars in string
            }  //end if res>0
//            buf[res]=0;
//            printf(":%s:%d\n", buf, res);
//            if (res==1) STOP=TRUE; /* stop loop if only a CR was input */
            wait_flag = TRUE;      /* wait for new input */
         }  //end if wait flag == FALSE

      int n;
      len = sizeof(client_addr);
      n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&client_addr,&len);
	if (n>0)
         	{
			scanf("%d",&n);
			printf("Udp %d\n", n);
		int rc=0;
//		set_blocking (fd, 0);
		int i=0;
		bufSerialOut[5]=n;
		for(i=0;i<n;i++)
			{
				bufSerialOut[i+6]=mesg[i];
			} 
            	rc= write(fd,bufSerialOut,n+6);
//		usleep ((7 + 25) * 100); 		
		scanf("%d",&rc);
                        printf("WSerial %d\n", rc);	
			}
//      printf("-------------------------------------------------------\n");
//      mesg[n] = 0//      printf("Received the following:\n");
//      printf("%s",mesg);
//      printf("-------------------------------------------------------\n");
}
    }  //while stop==FALSE
      // restore old port settings
      tcsetattr(fd,TCSANOW,&oldtio);
      tcsetattr(tty,TCSANOW,&oldkey);
      close(tty);
      close(fd);        //close the com port
      exit(-1);
   }  //end if command line entrys were correct
   else  //give instructions on how to use the command line
   {
/*
      fputs(instr,output);
      fputs(instr1,output);
      fputs(instr2,output);
      fputs(instr3,output);
      fputs(instr4,output);
      fputs(instr5,output);
      fputs(instr6,output);
      fputs(instr7,output);
  
*/
    printf("erro:%d\n",errorJC);
   }
//   fclose(input);
//   fclose(output);
}  //end of main
/***************************************************************************
* signal handler. sets wait_flag to FALSE, to indicate above loop that     *
* characters have been received.                                           *
***************************************************************************/

void signal_handler_IO (int status)
{
//   printf("received SIGIO signal.\n");
//	printf(".");
   wait_flag = FALSE;
}
void sigintHandler(int sig_num)
{
   printf("received signal.\n");
   STOP=TRUE;
}

void SendToUdp(int strlen)
{
//    	SOCKET hSocket; 

//  #define SERVER "192.168.1.5"
#define BUFLEN 512  //Max length of buffer
//#define PORT 1830   //The port on which to send data
//   struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
 //   char buf[BUFLEN];
    char message[BUFLEN];
     buf[0]=bufParam[0];
     buf[1]=0x3B;
     buf[2]=0x64;
     buf[3]=0x3B;
//    buf[4]=bufParam[4];
    buf[4]=strlen;  
    buf[5]=0x3B;
    int i=0;
    for (i=0; i<strlen; i++)
		{
			buf[i+6]=bufUdp[i];
		}
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        printf("pb socket");
    }   
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
         if (sendto(s, buf, strlen+6 , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            printf("pb UDP");
        }
}
 void io_handler(int signal)
{
wait_flag = FALSE;
	#define MAXBUFLEN  1024
	int 			numbytes;	/* Number of bytes recieved from client */
        int 			addr_len;	/* Address size of the sender		*/
        struct sockaddr_in 	their_addr;	/* connector's address information	*/
	
	printf("The recvfrom proc. !\n");

        if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, \
                    (struct sockaddr *)&their_addr, &addr_len)) == -1) {
                perror("recvfrom");
                exit(1);
        }
	
	buf[numbytes]='\0';
        printf("got from %s --->%s \n  ",inet_ntoa(their_addr.sin_addr),buf);


	return;
}

/*
	struct sockaddr_in szHost

    	struct sockaddr udpaddr;
	int nPort = 1830;
        int hSocket;
	char szHost[256] = "192.168.1.5";
	    bzero((char *) &szHost, sizeof(szHost));
    	szHost.sin_family = AF_INET;
   	 bcopy((char *)server->h_addr, 
	  (char *)&szHost.sin_addr.s_addr, server->h_length);
    	szHost.sin_port = htons(nPort);
	udpaddr = UdpFormatAdress( szHost, (u_short)nPort );
	hSocket = socket( AF_INET, SOCK_DGRAM, 0 );
	printf("send udp");
    	int n = sendto(hSocket, bufUdp, strlen, 0, &udpaddr, sizeof(udpaddr));
 	   if (n < 0)
	{
		printf("ERROR in sendto");

	} 
*/

