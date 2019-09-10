#include <reg51.h>

#define starting_delay 5     //Time of starting coil energized in seconds
#define debounce 3		     //sensor debounce time in seconds
#define ON 1
#define OFF 0

//Assigning the inputs and outputs
//-------INPUT PORTS-----------------
sbit oht_high=P2^0;	 	//Over Head Tank High Level	//				 
sbit oht_low=P2^1; 		//Over Head Tank Low Level	//  SENSOR INPUTS
sbit sump_high=P2^2; 	//Sump Tank High Level		//
sbit Sump_low=P2^3; 	//Sump Tank Low Level		//

//-------OUTPUT PORTS-----------------

sbit start=P0^5;    	//Motor Starting Coil		//
sbit run=P0^4;			//Motor Running coil		//

//-------STATUS BITS--------------------
enum STATE
{
	high 	= 0;
	low 	= 1;
	mid 	= 2;
}
bit motor = OFF;

//Function declarations
void start_motor();
void stop_motor();
void delay(unsigned int);
void sdelay(unsigned int);

//Variables declaration

//Main program start
void main(void)
{
	P2 = 0xff;
	motor =0;
	enum STATE sumpstate , ohtstate;
	while(1)
	{
		//Update the states based on sensor inputs
		if(sump_high ==1) 							// Sump High STATE
		{
			sdelay(debounce);
			if(sump_high ==1)	sumpstate = high;
		}
		
		if(sump_low == 0)							// Sump Low STATE
		{
			sdelay(debounce);		
			if(sump_low == 0)	sumpstate = low;
		}
		
		if((sump_low==1)&(sump_high == 0))			//Sump at mid level STATE
		{
			sdelay(debounce);
			if((sump_low==1)&(sump_high == 0)) sumpstate = mid;
		
		}
		if(oht_high == 1)							//Over Head Tank High STATE
		{
			sdelay(debounce);	
			if(oht_high == 1)	ohtstate = high;
		}
		
		if(oht_low == 0) 							//Over Head Tank Low STATE
		{
			sdelay(debounce);
			if(oht_low == 0)	ohtstate = low;
		}
		
		if((oht_low==1)&(oht_high == 0))			//Over Head Tank at mid level STATE
		{
			sdelay(debounce);
			if((oht_low==1)&(oht_high == 0)) ohtstate = mid;
		}
		
		//Action to be done based on states
		switch(sumpstate)
		{
		  case high:
			{
				break;
			}
			case mid:
			{
				break;
			} 									 
			case low:
			{
				if(motor == ON)
				{
					stop_motor();
					break;
				}
				break;
			}
			case default:
			{
				stop_motor();
				break;
			}
		}//End sump state
		
		switch(ohtstate)
		{
			case high:
			{
				if(motor == ON) 
				{
					stop_motor();
					break;
				}
				break;
			}
			case mid:
			{
				break;
			}
			case low:
			{
				if(motor == OFF)
				{
					if((sumpstate == high)|(sumpstate == mid)) 
					start_motor();
					break;
				}
				break;
			}
			case default:
			{
				stop_motor();
				break;
			}
		}//end oht state
	}//end while
}//end main

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
void start_motor();
{
	start = 1;
	run = 1;
	sdelay(starting_delay);
	start =0;
	motor =1;
}
void stop_motor()
{
	run = 0;
	motor = 0;
}