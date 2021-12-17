//헬로 월드

/*

 max7219.c
 
 Raspberry Pi driving the Max7219

 to compile : gcc max7219.c -o max7219 -lwiringPi


*/
 
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// define our pins :

#define DATA        0 // GPIO 17 (WiringPi pin num 0)  header pin 11
#define CLOCK       3 // GPIO 22 (WiringPi pin num 3)   header pin 15
#define LOAD        4 // GPIO 23 (WiringPi pin num 4)   header pin 16


// The Max7219 Registers :

#define DECODE_MODE   0x09                       
#define INTENSITY     0x0a                        
#define SCAN_LIMIT    0x0b                        
#define SHUTDOWN      0x0c                        
#define DISPLAY_TEST  0x0f                         



static void Send16bits (unsigned short output)
{
   
  unsigned char i;

  for (i=16; i>0; i--) 
  {
    unsigned short mask = 1 << (i - 1); // calculate bitmask
  
    digitalWrite(CLOCK, 0);  // set clock to 0
    
    // Send one bit on the data pin
    
    if (output & mask)   
      digitalWrite(DATA, 1);          
      else                              
      digitalWrite(DATA, 0);  
        
    digitalWrite(CLOCK, 1);  // set clock to 1
      
  }

}


// Take a reg numer and data and send to the max7219
// 8x8 4개라 data4까지
static void MAX7219Send (unsigned char reg_number, unsigned short data1,unsigned short data2,unsigned short data3,unsigned short data4)
{
  digitalWrite(LOAD, 1);  // set LOAD 1 to start
  Send16bits((reg_number << 8) + data1);   // send 16 bits ( reg number + dataout )
  Send16bits((reg_number << 8) + data2);   // send 16 bits ( reg number + dataout )
  Send16bits((reg_number << 8) + data3);   // send 16 bits ( reg number + dataout )
  Send16bits((reg_number << 8) + data4);   // send 16 bits ( reg number + dataout )
  digitalWrite(LOAD, 0);  // LOAD 0 to latch
  digitalWrite(LOAD, 1);  // set LOAD 1 to finish
}




int main (void)
{
  if (wiringPiSetup () == -1) exit (1) ;

  //We need 3 output pins to control the Max7219: Data, Clock and Load

  pinMode(DATA, OUTPUT);  
  pinMode(CLOCK, OUTPUT);
  pinMode(LOAD, OUTPUT);  
 
  MAX7219Send(SCAN_LIMIT, 7,7,7,7);     // set up to scan all eight digits


int heart[16][8] = { 
    {0, 101, 21, 117, 87, 33, 1, 0}, 
    {0, 37, 85, 85, 37, 5, 5, 0},
    {0, 138, 85, 85, 128, 0, 0, 0},
    {0, 68, 69, 93, 68, 64, 0, 0},

    {0, 178, 138, 186, 171, 144, 128, 0},
    {0, 18, 170, 170, 18, 2, 2, 0},
    {0, 69, 170, 170, 64, 0, 0, 0},
    {0, 34, 162, 46, 34, 32, 32, 0}, 

    {0, 73, 85, 85, 73, 64, 64, 0},
    {0, 137, 85, 85, 9, 1, 1, 0},
    {0, 34, 85, 85, 32, 0, 0, 0},
    {0, 145, 81, 151, 17, 16, 16, 0},

    {0, 164, 170, 170, 164, 160, 160, 0},
    {0, 137, 85, 85, 9, 1, 1, 0},
    {0, 145, 170, 170, 144, 0, 0, 0},
    {0, 200, 168, 203, 136, 136, 136, 0},
    };
    // 0, 16, 56, 124, 254, 238, 68, 0, 0 //하트
    
    
    for(;;){      
      int i=0;
      for(int k=0; k<4; k++){
          i=4*k;
          //intensity 1,5,10,15로 하면 "오른쪽부터" 밝기 1,5,10,15로 설정됨        
          MAX7219Send(DECODE_MODE, 0,0,0,0);   // Set BCD decode mode on
          MAX7219Send(DISPLAY_TEST, 0,0,0,0);  // Disable test mode
          MAX7219Send(INTENSITY, 1,1,1,1);     // set brightness 0 to 15
          MAX7219Send(SHUTDOWN, 1,1,1,1);      // come out of shutdown mode   / turn on the digits
          
          MAX7219Send(1, heart[i+3][0],heart[i+2][0],heart[i+1][0],heart[i][0]); // (행, 4,3,2,1)
          MAX7219Send(2, heart[i+3][1],heart[i+2][1],heart[i+1][1],heart[i][1]);
          MAX7219Send(3, heart[i+3][2],heart[i+2][2],heart[i+1][2],heart[i][2]);          
          MAX7219Send(4, heart[i+3][3],heart[i+2][3],heart[i+1][3],heart[i][3]);        
          MAX7219Send(5, heart[i+3][4],heart[i+2][4],heart[i+1][4],heart[i][4]);      
          MAX7219Send(6, heart[i+3][5],heart[i+2][5],heart[i+1][5],heart[i][5]);          
          MAX7219Send(7, heart[i+3][6],heart[i+2][6],heart[i+1][6],heart[i][6]);          
          MAX7219Send(8, heart[i+3][7],heart[i+2][7],heart[i+1][7],heart[i][7]);  

          delay(1000);


      }
  }

  return 0;
}