#include <wiringPi.h>

#define DATA        12
#define CLOCK       14
#define LOAD        10

array[8][8]={
    0b00011000,
    0b00111100,
    0b01100110,
    0b11000011,
    0b11111111,
    0b11111111,
    0b11000011,
    0b11000011
};

void main(){

    if( wiringPiSetup() < 0 ){
        return 1;
    }

    pinMode(DATA, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(LOAD, OUTPUT);
    delay(100);

    write_Max7219(0x09, 0x00); // Decode Mode - No decode for digits
    write_Max7219(0x0a, 0x00); // Intensity - 7/32
    write_Max7219(0x0b, 0x07); // Scan Limit - All Output Port Enable
    write_Max7219(0x0c, 0x01); // Shutdown - Normal Operation
    write_Max7219(0x0f, 0x00); // Display Test

    int i;
    for( i = 1 ; i < 9 ; i++){
        write_Max7219(i, array[i-1]);
    }
    void write_Max7219(int address, int data){
        digitalWrite(LATCH,LOW);
        write_byte(address);
        write_byte(data);
        digitalWrite(LATCH,HIGH);
    }
    void write_byte(int data){
        int i;
        for (i = 0; i < 8; i++){
        digitalWrite(DIN, !!(data & (1 << 7 - i)));
        digitalWrite(SCK, HIGH);
        digitalWrite(SCK, LOW);
    }
}