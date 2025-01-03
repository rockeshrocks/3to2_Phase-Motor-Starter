 #include <reg51.h>

//Assigning the inputs and outputs
//-------INPUT PORTS-----------------
sbit s1=P2^0; //start					//				 
sbit s2=P2^1; //stop					//  BUTTON INPUTS
sbit am=P2^2; //automatic				//
sbit p2=P2^3; //phase from stop button	//
sbit p3=P1^4; //condenser phase			//	PHASE INPUTS
sbit pc=P1^5; //condenser sense			//
//-------OUTPUT PORTS-----------------
//Set bits to low to activate the corresponding controls
sbit relay2=P0^5;    	//Relay to Start Control
sbit relay1=P0^4;		//Relay to Stop Control
sbit relay3=P0^6;		//Relay to connect running condenser in the phase	 
sbit relay4=P0^7;		//Relay to connect starting condenser in the phase
sbit wait_LED=P0^0;		//WAIT LED
sbit warning_LED=P0^1;
//-------STATUS BITS--------------------
bit motor=0x01;
//sub program function initialization
void start_motor_3phase();
void stop_motor_3phase();
void start_motor_2phase();
void stop_motor_2phase();
void delay(unsigned int);
void sdelay(unsigned int);
void wait();
void Error();
//Variables initialization
extern unsigned n =0;
//main program start
void main(void)
{
 P0 = 0x0f;
 motor =0;
	   for(;;)
	   {
		   if(am==1)
		   {
			   if(motor==0)
			   {
				   if(p3==1) start_motor_3phase();
				   else start_motor_2phase();
			   }
		   }
		   else
		   {
			   if(motor==0)
				   if(s1==1)
					   if(p3==1) start_motor_3phase();
						else start_motor_2phase();
						
				if(motor==1)		
					if(s2==1)
						if(p3==1) stop_motor_3phase();
						else stop_motor_2phase();
			   
		   }
	   if(p2==0 & motor==1)
	   {
		   if(p3==1) stop_motor_3phase();
						else stop_motor_2phase();
	   }
	   }
  }
//--------------------------------------------------------------
void start_motor_3phase(void)
{	
   wait();
   if(p2==0|p3==0)
   {
	   Error();
	   goto end1;
   }
   wait();
   relay1=0;        //close stop
   sdelay(3);
   relay2=0;		//close start
   sdelay(1);
   relay2=1;		//open start
   motor=1;
   end1:;
}
//--------------------------------------------------------------
void start_motor_2phase(void)
{	
   wait();
   if(p2==0)
   {
	   Error();
	   goto end2;
   }
   for(n=0;n<=5;n++)
   {
	  relay2=0;
	  if(pc==0)
	  {
		  relay2=1;
		  sdelay(5);
	  } 
	  else  break;
	}
   
   if(pc==0)
	  {	
	  	  Error();
		  goto end2;
	  } 
   relay1=0;	//close stop relay
   sdelay(1);
   relay2=0;	//close start relay
   relay4=0;	//close starting condenser relay
   sdelay(3);
   relay2=1;	//open start relay
   relay4=1;	//open starting condenser relay
   motor =1;
   end2:	;  	

   }
//--------------------------------------------------------------
void stop_motor_3phase(void)
{
	wait=1;
	relay1=1;
	relay2=1;
	delay(2500);
	motor=0;
	wait=0;
}
//--------------------------------------------------------------
void stop_motor_2phase(void)
{
	wait=1;
	relay1=1;
	relay2=1;
	relay2=1;
	delay(2500);
	motor=0;
	wait=0;
}
 //--------------------------------------------------------------
void delay(unsigned int time)
{
 	unsigned i,j;
	for(i=0;i<time;i++)
	for(j=0;j<=121;j++);
}
//--------------------------------------------------------------
void  sdelay(unsigned int time)
{
	unsigned i,j;
	for(i=0;i<time;i++)
	for(j=0;j<=1000;j++)
	delay(1);
}
//--------------------------------------------------------------
void wait(void)
{
	
	for(n=0;n<=30;n++)
	{
		wait_LED=0;
		delay(500);
		wait_LED=1;
		delay(500);
	}
}
//--------------------------------------------------------------
void Error()
{
	for(;;)
	{
		warning_LED=0;
		delay(500);
		warning_LED=1;
		delay(500);
		if(s2==1) break;
	}
}
