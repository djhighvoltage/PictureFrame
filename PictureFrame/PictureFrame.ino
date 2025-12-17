#include "SerialMP3Player.h" //Mp3 board

#include <MPR121.h> //Touch board
#include <MPR121_Datastream.h>
#include <Wire.h>

#define TX 9 //Sound board pins
#define RX 10

const uint8_t MPR121_ADDR = 0x5A; //Touch Address

int Max = 0; //Varibales
int Mode = 0;
int Sound;

SerialMP3Player mp3(RX,TX);


void setup() {

  for(int x = 0; x < 6; x++){ //Sets the pins for the dip switches
    pinMode(x+2, INPUT);
  }
  
  if(digitalRead(2) == HIGH){ //Reads the dip switches
    Max = Max + 1;
  }
  if(digitalRead(3) == HIGH){
    Max = Max + 2;
  }
  if(digitalRead(4) == HIGH){
    Max = Max + 4;
  }
  if(digitalRead(5) == HIGH){
    Max = Max + 8;
  }
  if(digitalRead(6) == HIGH){
    Max = Max + 16;
  }
  if(digitalRead(7) == HIGH){
    Max = Max + 32;
  }
  
  if(Max == 0){ //If no dip switches are flipped go in order
    Mode = 1;
  }
  
  
  Serial.begin(9600);     // start serial interface
  mp3.begin(9600);        // start mp3-communication
  mp3.setVol(30);        // Volume controll
  MPR121.begin(MPR121_ADDR);
  delay(500);             // wait for init

  mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
  delay(500);             // wait for init
  Serial.println(Max);
  randomSeed(analogRead(A0));

  MPR121.setTouchThreshold(1,10);  
  MPR121.setReleaseThreshold(1,2);
  
  MPR121.setFFI(FFI_10);
  MPR121.setSFI(SFI_10);
  MPR121.setGlobalCDT(CDT_4US);  // reasonable for larger capacitances
  delay(1000);
  
  MPR121.autoSetElectrodes();  // autoset all electrode settings
}


// the loop function runs over and over again forever
void loop() {

  MPR121.updateAll(); //Update the touch board to see if there is a touch
  
  if (MPR121.isNewTouch(5)) { //If Frame Touched
    if(Mode == 0){ //Random Mode
      Sound = random(1,Max+1);
    }
    else{ //Incramental Mode
      Sound++;
    }
    mp3.play(Sound);
    delay(2000);
  }

}
