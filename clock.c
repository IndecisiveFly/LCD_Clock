//Kaitik Kwok
//Michael Wood
//May 2016
//clock.c

#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>

//clock modes
#define MODE 0
#define SELECT 1
#define SET 2
#define CLOCK 7
#define CLOCK_INPUT 3

//I2C LCD clear
#define CLEAR 0x51

//ISR ignore constant
#define WAIT 500

int last_button=0;
int last_time=0;

unsigned int counter=0;
unsigned int time_isr=0;
unsigned int timer=0;
static int fd;

//0 = stopped
//1 = running
int timer_state=0;

//0 = display clock
//1 = display stop watch/timer
//2 = set min
//3 = set hr
int state =0; 

//interrupt functions
void clock_isr(void)
{
  time_isr++;
  //4684= 1 second
  if (time_isr%1171==0)
  {
    counter=counter+250;
    timer=timer+(timer_state*250);
  }

}

void mode_button (void)
{
  //makes sure button only pressed once
  if (last_button ==0 && (counter-last_time<WAIT))
  {
    //do nothing
  }
  else
  {
//    printf("mode button pressed\n");
    last_button=0;
    last_time=counter;

    switch (state) //toggle between states
    {
      case 0: //switch to timer
        state=1;
        break;
      case 1: //switch to clock
        state=0;
        break;
      case 2: //switch to set hour
        state=3;
        break;
      case 3: //switch to set min
        state=2;
        break;
    }
  }
}

void select_button(void)
{
  //makes sure button only pressed once
  if (last_button ==1 && (counter-last_time<WAIT))
  {
    //do nothing
  }
  else
  {
 //   printf("select button pressed\n");
    last_button=1;
    last_time=counter;
    switch (state)
    {
      case 0: //nothing
        break;
      case 1: //start/stop
        if(timer_state==0)
          timer_state=1;
        else 
          timer_state=0;
        break;
      case 2: //increment min
        counter=counter+60000;
        break;
      case 3: //increment hr
        counter=counter+3600000;
        break;
    }
  }
}

void set_button(void)
{
  //makes sure button only pressed once
  if (last_button ==2 && (counter-last_time<WAIT))
  {
    //do nothing
  }
  else
  {
    last_button=2;
    last_time=counter;
  //  printf("set button pressed\n");
    switch (state)
    {
      case 0: //set
        state=2;
        break;
      case 1: //Reset
        timer_state=0;
        timer=0;
        break;
      case 2: //exit
        state=0;
        break;
      case 3: //exit
        state=0;
        break;
    }
  }
}

//print to LCD
int print(char time[])
{
  wiringPiI2CWriteReg16(fd,0xFE,CLEAR);
  delay(3);

  for (int i=0; i<15;i++)
  {
    //wiringPiI2CWriteReg8(fd,0,time[i]);
    wiringPiI2CWriteReg8(fd,time[i],time[i]);
    delay(3);
  }

  return 0;
}

//convert counter to clock format
int display_clock(int counter)
{
  char test[]="00:00:00 CLOCK ";
  char display_mode[]="CLOCK ";

  int sec=(counter/1000)%60;
  int min=(counter/60000)%60;
  int hour=(counter/3600000)%24;

  switch (state)
  {
    case 0: 
      sprintf(display_mode,"Clock ");
      break;
    case 1: 
      sprintf(display_mode,"Timer ");
      break;
    case 2: 
      sprintf(display_mode,"Set Mn");
      break;
    case 3: 
      sprintf(display_mode,"Set Hr");
      break;
  }

  sprintf(test,"%02i:%02i:%02i %s\n",hour,min,sec,display_mode);
  print(test);

  return 0;
}

int main (void)
{
  if (wiringPiSetup()<0)
  {
    fprintf(stderr,"Fail to set up wiringPi");
    return 1;
  }

  //pin Setup
  pinMode(MODE,INPUT);
  pinMode(SET,INPUT);
  pinMode(SELECT,INPUT);
  pinMode(CLOCK,GPIO_CLOCK);
  pinMode(CLOCK_INPUT,INPUT);
  digitalWrite(MODE,LOW);
  digitalWrite(SET,LOW);
  digitalWrite(SELECT,LOW);
  digitalWrite(CLOCK_INPUT,LOW);

  //LCD Setup
  //using default I2C address
  fd=wiringPiI2CSetup(0x28);
  if (fd==-1)
  {
    fprintf(stderr,"fail to set up I2C\n");
    return 1;
  }
  //turn on LCD and clear
  wiringPiI2CWriteReg16(fd,0xFE,0x41);
  wiringPiI2CWriteReg16(fd,0xFE,CLEAR);

  //ISR setup
  if ((wiringPiISR(MODE,INT_EDGE_FALLING, &mode_button))<0)
  {
    fprintf(stderr,"fail to set up mode ISR\n");
    return 1;
  }

  if ((wiringPiISR(SELECT,INT_EDGE_FALLING, &select_button))<0)
  {
    fprintf(stderr,"fail to set up select ISR\n");
    return 1;
  }

  if ((wiringPiISR(SET,INT_EDGE_FALLING, &set_button))<0)
  {
    fprintf(stderr,"fail to set up set ISR\n");
    return 1;
  }
  if ((wiringPiISR(CLOCK_INPUT,INT_EDGE_FALLING, &clock_isr))<0)
  {
    fprintf(stderr,"fail to set upp clock input ISR\n");
    return 1;
  }

  while (1)
  {
    if (state==1)
    {
      display_clock(timer);
      delay(200);
    }
    else
    {
      display_clock(counter);
      delay(200);
    }
  }
  return 0;
}

