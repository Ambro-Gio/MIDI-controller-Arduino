#include <LiquidCrystal.h>
#include <MIDI.h>

// LCD pins <--> Arduino pins
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int noteON = 144;//144 = 10010000 in binary, note on command
int noteOFF = 128;//128 = 10000000 in binary, note off command

//midi note pitches init
int note1=10;
int note2=20;
int note3=30;
int note4=40;

//support for 2 banks, coded 0 and 1;
int bank = 0;

int channel = 0;
int velocity = 127;


// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  //button 1 to 5 pins
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  //led for button 1
  pinMode(13, OUTPUT);

  //  Set MIDI baud rate:
  Serial.begin(31250);

  //LCD init
  lcd.begin(16, 2); // set up number of columns and rows

  //initial message
  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("MIDI Controller");         // print message at (0, 0)
  updateRow2();
}

void loop() {
  if(digitalRead(12)==0)
    change_bank();
  if(digitalRead(8)==0)
    buttonPress_longIN(note1, velocity, 12);
  if(digitalRead(9)==0)
    buttonPress_longIN(note2, velocity, 11);
  if(digitalRead(10)==0)
    buttonPress_longIN(note3, velocity, 10);
  if(digitalRead(11)==0)
    buttonPress_longIN(note4, velocity, 9);
  
}

//sends a MIDI pitch
//if Bank 2 is selected, note is shifted to the correct value
//capable of long press
void buttonPress_longIN(int note, int vel, int pin)
{
  //note shift
  if(bank == 1){
    note += 40;
  }

  //MIDImessage(noteON, note, vel);
  MIDI.sendNoteOn(note, vel, channel);  //sending midi pitch

  //building screen messages
  //String message = String("NoteOn ") + String(note);

  //screen message
  lcd.clear();
  lcd.setCursor(0, 0);         
  lcd.print(note);
  lcd.setCursor(4, 0);
  lcd.print(pin);
  updateRow2();

  //waiting for button release
  while(digitalRead(pin)==0)
    ;

  //MIDImessage(noteOFF, note, 0);
  MIDI.sendNoteOff(note, 0, channel);

}

void change_bank(){
  if(!bank){
    bank = 1;
    //turning on status led
    digitalWrite(13, HIGH);
  }
  else
  {
    bank = 0;
    //turning off status led
    digitalWrite(13, LOW); 
  }

  //screen message for bank change
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bank changed");
  updateRow2();
}

//row 2 stays relatively fixed
void updateRow2()
{
  String message = String("Bank: ") + String(bank) + String(" Ch: ") + String(channel);
  lcd.setCursor(0, 1);
  lcd.print(message);
}

//send MIDI message
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}