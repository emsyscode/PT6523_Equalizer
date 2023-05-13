/*Begining of Auto generated code by Atmel studio */
/*
This code is not clean and not perfect, this is only 
a reference to extract ideas and adapte to your solution.
*/
void send_char(unsigned char a);
void send_data(unsigned char a);
void send_update_clock(void);
void DigitTo7SegEncoder(unsigned char digit);
void update_clock(void );

#define LCD_in 8  // This is the pin number 8 on Arduino UNO
#define LCD_clk 9 // This is the pin number 9 on Arduino UNO
#define LCD_stb 10 // This is the pin number 10 on Arduino UNO
#define LCD_INH 11 // This is the pin number 11 on Arduino UNO

/*
#define BIN(x) \
( ((0x##x##L & 0x00000001L) ? 0x01 : 0) \
| ((0x##x##L & 0x00000010L) ? 0x02 : 0) \
| ((0x##x##L & 0x00000100L) ? 0x04 : 0) \
| ((0x##x##L & 0x00001000L) ? 0x08 : 0) \
| ((0x##x##L & 0x00010000L) ? 0x10 : 0) \
| ((0x##x##L & 0x00100000L) ? 0x20 : 0) \
| ((0x##x##L & 0x01000000L) ? 0x40 : 0) \
| ((0x##x##L & 0x10000000L) ? 0x80 : 0))
*/

//ATT: On the Uno and other ATMEGA based boards, unsigned ints (unsigned integers) are the same as ints in that they store a 2 byte value.
//Long variables are extended size variables for number storage, and store 32 bits (4 bytes), from -2,147,483,648 to 2,147,483,647.

//*************************************************//
void setup() {
  pinMode(LCD_clk, OUTPUT);
  pinMode(LCD_in, OUTPUT);
  pinMode(LCD_stb, OUTPUT);
  pinMode(LCD_INH, OUTPUT);//When this pin is Low, the display is forcibly turned off. (SG1~SG52, COM1~COM3 are set to Low)
                           //When /INH = Low: Serial data transfers can be performed when the display is forcibly off. 
  pinMode(13, OUTPUT);
  
  Serial.begin(115200);
  /*CS12  CS11 CS10 DESCRIPTION
  0        0     0  Timer/Counter1 Disabled 
  0        0     1  No Prescaling
  0        1     0  Clock / 8
  0        1     1  Clock / 64
  1        0     0  Clock / 256
  1        0     1  Clock / 1024
  1        1     0  External clock source on T1 pin, Clock on Falling edge
  1        1     1  External clock source on T1 pin, Clock on rising edge
 */
  
// Note: this counts is done to a Arduino 1 with Atmega 328... Is possible you need adjust
// a little the value 62499 upper or lower if the clock have a delay or advnce on hours.

  digitalWrite(LCD_stb, LOW);
  digitalWrite(LCD_INH, HIGH);
  delayMicroseconds(5);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
 
}
void send_char(unsigned char a)
{
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  // the validation of data happen when clk go from LOW to HIGH.
  // This lines is because the clk have one advance in data, see datasheet of sn74HC595
  // case don't have this signal instead of "." will se "g"
  digitalWrite(LCD_stb, LOW); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(5);
  digitalWrite(LCD_clk,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
  digitalWrite(LCD_clk,LOW);// need invert the signal to allow 8 bits is is low only send 7 bits
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(LCD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
    //
    digitalWrite(LCD_stb, HIGH); // When strobe is low, all output is enable. If high, all output will be set to low.
  delayMicroseconds(5);
  }
}
// I h've created 3 functions to send bit's, one with strobe, other without strobe and one with first byte with strobe followed by remaing bits.
void send_char_without(unsigned char a)
{
 //
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
  data=a;
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
  digitalWrite(LCD_clk, LOW);
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(LCD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(5);
  }
}
void send_char_8bit_stb(unsigned char a)
{
 //
 unsigned char transmit = 15; //define our transmit pin
 unsigned char data = 170; //value to transmit, binary 10101010
 unsigned char mask = 1; //our bitmask
 int i = -1;
  data=a;
  // This lines is because the clk have one advance in data, see datasheet of sn74HC595
  // case don't have this signal instead of "." will se "g"
  for (mask = 00000001; mask>0; mask <<= 1) { //iterate through bit mask
   i++;
   digitalWrite(LCD_clk, LOW);
  delayMicroseconds(5);
    if (data & mask){ // if bitwise AND resolves to true
      digitalWrite(LCD_in, HIGH);
      //Serial.print(1);
    }
    else{ //if bitwise and resolves to false
      digitalWrite(LCD_in, LOW);
      //Serial.print(0);
    }
    digitalWrite(LCD_clk,HIGH);// need invert the signal to allow 8 bits is is low only send 7 bits
    delayMicroseconds(1);
    if (i==7){
    Serial.println(i);
    digitalWrite(LCD_stb, HIGH);
    delayMicroseconds(2);
    }
     
  }
}
void patternLCD(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B11111111);  send_char_without(0B11111111); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B11111111);  send_char_without(0B11111111); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B11111111);  send_char_without(0B11111111); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B11111111);  send_char_without(0B11111111); // 49:56  -57:64  
send_char_without(0B11111111);  send_char_without(0B11111111); // 65:72  -73:80// skip 78    
send_char_without(0B11111111);  send_char_without(0B11111111); // 81:88  -89:96// skip 85
send_char_without(0B11011111);  send_char_without(0B11111111); // 97:104-105:112
send_char_without(0B11111111);  send_char_without(0B11111111); //113:120-121:128
send_char_without(0B11111111);  send_char_without(0B11111111); //129:136-137:144
send_char_without(0B11111110);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void equalizer1(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B10000000); //129:136-137:144
send_char_without(0B00111111);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void equalizer2(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00111111);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void equalizer3(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00111110);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void equalizer4(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00111100);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void equalizer5(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00111000);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void equalizer6(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00110000);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void equalizer7(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00100000);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void wheel_a(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000011);  send_char_without(0B10000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000001);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00000000);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void wheel_b(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000011);  send_char_without(0B10000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00000000);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void wheel_c(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000011);  send_char_without(0B00000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00000000);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void wheel_d(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000001);  send_char_without(0B00000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00000000);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void wheel_e(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B0000000); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B00000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B00000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B00000000);  send_char_without(0B00000000); // 49:56  -57:64  
send_char_without(0B00000000);  send_char_without(0B00000000); // 65:72  -73:80// skip 78    
send_char_without(0B00000000);  send_char_without(0B00000000); // 81:88  -89:96// skip 85
send_char_without(0B00000000);  send_char_without(0B00000000); // 97:104-105:112
send_char_without(0B00000000);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00000000);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void test(){
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
byte data = 0xFF;
byte mask = 0x80;
byte info = 0x00;
int n, l, x;
l=0x00;
n=0x00;
x=0x00;
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//

   for ( n = 0; n<19; n++){
      for ( x =0; x<8; x++){
        info = (data ^ (mask >> x));
        send_char_without(info);
        Serial.println(info, BIN);
        l++;
      }

  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
Serial.print(l, DEC);
Serial.print("  ");
Serial.print(n, DEC);
Serial.print("; ");
delay(500);
}

}
void msgLCD(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
/*
 * Notes presents at datasheet of PT6523 (23 pages): 
 * 1. Address: 41H 
 * 2. D1 to D156 = Display Data     When D1 to D156 are set to “1”, Display is turned ON. When D1 to D156 are set to “0”, Display is turned OFF. 
 * 3. DR = 1/2 Bias Drive or 1/3 Bias Drive Switching Control Data 
 * 4. SC = Segment ON/OFF Control Data 
 * 5. BU = Normal Mode/Power-Saving Control Data 
 * 6. x = Not Relevant For example, there are 63 segments that are being used, the 63 bits of display data (D94 to D156) must be sent. Please refer to the diagram below. 
 */
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00011100);  send_char_without(0B01100100); //  1:8    -9:16// First 14 segments display(Note: skip the 6 & 9)Belongs Second 
send_char_without(0B01000000);  send_char_without(0B00000000); // 17:24  -25:32// Second 14 segments display(Note: skip 25 & 26) Belongs Third 
send_char_without(0B00000000);  send_char_without(0B10000000); // 33:40  -41:48// Third 14 segments display(Note:Skip 40 & 44 the 41,42 & 43) Belongs first 7seg display 
send_char_without(0B10100011);  send_char_without(0B11000000); // 49:56  -57:64  
send_char_without(0B00110000);  send_char_without(0B01100110); // 65:72  -73:80// skip 78    
send_char_without(0B00010000);  send_char_without(0B01110000); // 81:88  -89:96// skip 85
send_char_without(0B01100000);  send_char_without(0B00101000); // 97:104-105:112
send_char_without(0B01001110);  send_char_without(0B00000000); //113:120-121:128
send_char_without(0B00000000);  send_char_without(0B00000000); //129:136-137:144
send_char_without(0B00000000);  send_char_without(0B00000000); //145:152-153:160 // 0B0000xxxx see the bits DR, SC, BU, X(This bits switch on/off and sleep...
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void allON(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111); 
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  
send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B11111111);  send_char_without(0B00001111);
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void allOFF(){
//send total of 156 bits, the 4 last bits belongs to DR, SC, BU, X;
digitalWrite(LCD_stb, LOW); //
delayMicroseconds(2);
send_char_8bit_stb(0B01000001); // firts 8 bits is address, every fixed as (0B010000001), see if clk finish LOW or HIGH Very important!
//
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000); 
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000); 
send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B00000000);  send_char_without(0B01100000);
delayMicroseconds(2);
digitalWrite(LCD_stb, LOW); // 
delayMicroseconds(2);
}
void loop() {
  long randNumber;
   // patternLCD();
 for(int c=0; c<4; c++){
  allON(); // All on
  delay(400);
  allOFF(); // All off
  delay(400);
 }
  allOFF(); // All off
  msgLCD();
 for (int i=0; i< 500; i++){
  delay(10);
 }
 for (int t = 0; t<10; t++){
   wheel_e();
   delay(50);
   wheel_d();
   delay(50);
   wheel_c();
   delay(50);
   wheel_b();
   delay(50);
   wheel_a();
   delay(50);
 }

 equalizer1();
 delay(1000);
 equalizer2();
 delay(1000);
 equalizer3();
 delay(1000);
 equalizer4();
 delay(1000);
 equalizer5();
 delay(1000);
 equalizer6();
 delay(1000);
 equalizer7();
 delay(1000);
 
     for(int p=0; p<50; p++){
      randNumber = random(1, 7);
        switch  (randNumber){
          case 1: equalizer1();delay(70);
          case 2: equalizer2();delay(70);
          case 3: equalizer3();delay(70);
          case 4: equalizer4();delay(70);
          case 5: equalizer5();delay(70);
          case 6: equalizer6();delay(70);
          case 7: equalizer7();delay(70);
        
         }
     }
 
}
