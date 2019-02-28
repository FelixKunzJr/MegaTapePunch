/*
  MIT License
  Copyright (c) 2019 Felix Kunz
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/


int zero = 0b00000000;

int data = 0;

int storage[127][9];



void setup() {

  DDRA = 0b11111111; //Port direction
  DDRC = 0b00000011; //Port direction
  DDRB = 0b00000000; //Port direction
  PORTB = 0b1111111; //Set Pullup

  Serial.begin(9600);
  store();



}



void loop() {



  int JumperGraphical = !digitalRead(51);       // Mode to print serial input as graphical text
  int JumperTerminal = !digitalRead(50);        // Mode to print serial input in ascii-format
  int JumperButton = !digitalRead(53);          // Mode to print a given string "char st[]" as graphical text.

  if (JumperGraphical) {                      //Select modes
    GraphicalMode();
  }


  if (JumperTerminal) {

    TerminalMode();
  }


  if (JumperButton) {
    int plot = !digitalRead(52);  // Check if button to Print is pressed
    if (plot) {
      ButtonMode();
    }
  }
}


void(* resetFunc) (void) = 0;     //Function to Reset via software





int ButtonMode() {



  int ascii;
  char st[] = "HELLO ENTER";          //String which is to be Printed

  int lx = sizeof(st) - 1;
//Serial.println(lx);


  for (int x = 0; x < lx; x++) {
    ascii = (int)st[x];
    lochen(ascii);


  }
  skip();


  resetFunc();    //Reset after each finished Action to prevent Bugs

}

int TerminalMode() {
  if (Serial.available() > 0) {
    data = Serial.read();           // read the incoming byte

    if (data == 10) {              //Remove Enter stroke
      data = 0;
    } else if (data == 62) {      // the angle bracket (">") can be used to print some tape to ensure all holes are outside of the machine
      skip();
      data = 1;
    }
    PORTA =  data;
    fire();
  }




}


int GraphicalMode() {




  if (Serial.available() > 0) {
    data = Serial.read();

    if (data == 10) {       //Remove Enter stroke
      data = 0;
      goto DL;
    } else if (data == 62) {    // the angle bracket (">") can be used to print some tape to ensure all holes are outside of the machine
      skip();
      data = 1;
      goto DL;
    } else if (data == 95) {    //convert underline "_" to space instead of ascii
      space();
      goto DL;
    } else {


      lochen(data);

    }
DL:
    Serial.println(data, DEC);    //Print ascii text
  }

}


int lochen(int value) {

  if (value > 96) {           //Convert everything into lower space
    value = value - 32;

  }


  int X[7];


  X[0] = storage[value][0];   //Get the max. 8 bytes from storage
  X[1] = storage[value][1];
  X[2] = storage[value][2];
  X[3] = storage[value][3];
  X[4] = storage[value][4];
  X[5] = storage[value][5];
  X[6] = storage[value][6];
  X[7] = storage[value][7];
  X[8] = storage[value][8];   //Gets the length from storage. It indicates how many colums are used to print a letter

  exec(X);

}



int exec(int Byte[]) {

  int l = Byte[8];

  for (int i = 0; i < l; i++) {           // Print for each column of a letter
    PORTA = Byte[i];
    fire();

  }

  PORTA = zero;
  PORTC = 0b0000000;
  delay(10);
  PORTC = 0b0000010;
  delay(10);
  PORTC = 0b0000000;

}



void fire() {                         //triggers the puncher and waits for the Print Ready (PR) signal.
  delay(10);

  PORTC = 0b0000000;
  delay(10);
  PORTC = 0b0000010;
  delay(10);
  PORTC = 0b0000000;
  PORTA = zero;
  delay(10);


wait:

  int PR = digitalRead(35);

  if (!PR) {

    goto wait;
  }



}

void skip() {                           //Function is used to print some tape to ensure all holes are outside of the machine
  for (int t = 0; t < 50; t++) {
    delay(10);
    PORTC = 0b0000010;
    delay(10);
    PORTC = 0b0000000;



  }
}

void space() {
  delay(10);
  PORTA = zero;
  PORTC = 0b0000000;
  delay(10);
  PORTC = 0b0000010;
  delay(10);
  PORTC = 0b0000000;
  PORTA = zero;
  delay(10);


wait:

  int PR = digitalRead(35);

  if (!PR) {
    goto wait;
  }

}



void store() {              //Storage for all Letters.

  storage[32][0] = 0b00000000;    //First Column
  storage[32][1] = 0b00000000;    //Second Column
  storage[32][2] = 0b00000000;    //Third Column
  storage[32][3] = 0b00000000;    //Fourth Column
  storage[32][4] = 0b00000000;    //Fifth Column
  storage[32][5] = 0b00000000;    //Sixth Column
  storage[32][6] = 0b00000000;    //Seventh Column
  storage[32][7] = 0b00000000;    //Eigth Column

  storage[32][8] = 0b00000110;            //Is used to express how many columns are used for each letter.



  storage[40][0] = 0b11111111;
  storage[40][1] = 0b11111111;
  storage[40][2] = 0b11000011;
  storage[40][3] = 0b11000011;
  storage[40][4] = 0b00000000;
  storage[40][5] = 0b00000000;
  storage[40][6] = 0b00000000;
  storage[40][7] = 0b00000000;

  storage[40][8] = 0b00000100;


  storage[41][0] = 0b11000011;
  storage[41][1] = 0b11000011;
  storage[41][2] = 0b11111111;
  storage[41][3] = 0b11111111;
  storage[41][4] = 0b00000000;
  storage[41][5] = 0b00000000;
  storage[41][6] = 0b00000000;
  storage[41][7] = 0b00000000;

  storage[41][8] = 0b00000100;


  storage[48][0] = 0b01111110;
  storage[48][1] = 0b11100011;
  storage[48][2] = 0b11010011;
  storage[48][3] = 0b11001011;
  storage[48][4] = 0b11000111;
  storage[48][5] = 0b01111110;
  storage[48][6] = 0b00000000;
  storage[48][7] = 0b00000000;

  storage[48][8] = 0b00000110;


  storage[49][0] = 0b11000001;
  storage[49][1] = 0b11000001;
  storage[49][2] = 0b11111111;
  storage[49][3] = 0b11111111;
  storage[49][4] = 0b00000001;
  storage[49][5] = 0b00000001;
  storage[49][6] = 0b00000000;
  storage[49][7] = 0b00000000;

  storage[49][8] = 0b00000110;


  storage[50][0] = 0b01100011;
  storage[50][1] = 0b11000111;
  storage[50][2] = 0b11001111;
  storage[50][3] = 0b11011011;
  storage[50][4] = 0b11110011;
  storage[50][5] = 0b01100011;
  storage[50][6] = 0b00000000;
  storage[50][7] = 0b00000000;

  storage[50][8] = 0b00000110;



  storage[51][0] = 0b11011011;
  storage[51][1] = 0b11011011;
  storage[51][2] = 0b11011011;
  storage[51][3] = 0b11111111;
  storage[51][4] = 0b01111110;
  storage[51][5] = 0b00000000;
  storage[51][6] = 0b00000000;
  storage[51][7] = 0b00000000;

  storage[51][8] = 0b00000101;


  storage[52][0] = 0b00011100;
  storage[52][1] = 0b00111100;
  storage[52][2] = 0b01101100;
  storage[52][3] = 0b11001100;
  storage[52][4] = 0b11111111;
  storage[52][5] = 0b11111111;
  storage[52][6] = 0b00001100;
  storage[52][7] = 0b00000000;

  storage[52][8] = 0b00000111;


  storage[53][0] = 0b11110110;
  storage[53][1] = 0b11110011;
  storage[53][2] = 0b11011011;
  storage[53][3] = 0b11011011;
  storage[53][4] = 0b11011111;
  storage[53][5] = 0b11001110;
  storage[53][6] = 0b00000000;
  storage[53][7] = 0b00000000;

  storage[53][8] = 0b00000110;


  storage[54][0] = 0b01111110;
  storage[54][1] = 0b11111111;
  storage[54][2] = 0b11011011;
  storage[54][3] = 0b11011111;
  storage[54][4] = 0b01011110;
  storage[54][5] = 0b00000000;
  storage[54][6] = 0b00000000;
  storage[54][7] = 0b00000000;

  storage[54][8] = 0b00000101;


  storage[55][0] = 0b11000000;
  storage[55][1] = 0b11000000;
  storage[55][2] = 0b11001111;
  storage[55][3] = 0b11011111;
  storage[55][4] = 0b11110000;
  storage[55][5] = 0b11100000;
  storage[55][6] = 0b00000000;
  storage[55][7] = 0b00000000;

  storage[55][8] = 0b00000110;


  storage[56][0] = 0b01110110;
  storage[56][1] = 0b11111111;
  storage[56][2] = 0b11011011;
  storage[56][3] = 0b11011011;
  storage[56][4] = 0b11111111;
  storage[56][5] = 0b01110110;
  storage[56][6] = 0b00000000;
  storage[56][7] = 0b00000000;

  storage[56][8] = 0b00000110;


  storage[57][0] = 0b01111010;
  storage[57][1] = 0b11111011;
  storage[57][2] = 0b11001011;
  storage[57][3] = 0b11111111;
  storage[57][4] = 0b01111110;
  storage[57][5] = 0b00000000;
  storage[57][6] = 0b00000000;
  storage[57][7] = 0b00000000;

  storage[57][8] = 0b00000101;


  storage[65][0] = 0b00011111;
  storage[65][1] = 0b00111111;
  storage[65][2] = 0b01111000;
  storage[65][3] = 0b11011000;
  storage[65][4] = 0b11011000;
  storage[65][5] = 0b01111000;
  storage[65][6] = 0b00111111;
  storage[65][7] = 0b00011111;

  storage[65][8] = 0b00001000;

  storage[66][0] = 0b11111111;
  storage[66][1] = 0b11111111;
  storage[66][2] = 0b11011011;
  storage[66][3] = 0b11011011;
  storage[66][4] = 0b01111110;
  storage[66][5] = 0b01100110;
  storage[66][6] = 0b00000000;
  storage[66][7] = 0b00000000;

  storage[66][8] = 0b00000110;


  storage[67][0] = 0b01111110;
  storage[67][1] = 0b01111110;
  storage[67][2] = 0b11000011;
  storage[67][3] = 0b11000011;
  storage[67][4] = 0b11100111;
  storage[67][5] = 0b01100110;
  storage[67][6] = 0b00000000;
  storage[67][7] = 0b00000000;

  storage[67][8] = 0b00000110;


  storage[68][0] = 0b11111111;
  storage[68][1] = 0b11111111;
  storage[68][2] = 0b11000011;
  storage[68][3] = 0b01111110;
  storage[68][4] = 0b00111100;
  storage[68][5] = 0b00000000;
  storage[68][6] = 0b00000000;
  storage[68][7] = 0b00000000;

  storage[68][8] = 0b00000101;


  storage[69][0] = 0b11111111;
  storage[69][1] = 0b11111111;
  storage[69][2] = 0b11011011;
  storage[69][3] = 0b11011011;
  storage[69][4] = 0b11000011;
  storage[69][5] = 0b00000000;
  storage[69][6] = 0b00000000;
  storage[69][7] = 0b00000000;

  storage[69][8] = 0b00000101;


  storage[70][0] = 0b11111111;
  storage[70][1] = 0b11111111;
  storage[70][2] = 0b11011000;
  storage[70][3] = 0b11011000;
  storage[70][4] = 0b11000000;
  storage[70][5] = 0b00000000;
  storage[70][6] = 0b00000000;
  storage[70][7] = 0b00000000;

  storage[70][8] = 0b00000101;


  storage[71][0] = 0b01111110;
  storage[71][1] = 0b01111110;
  storage[71][2] = 0b11000011;
  storage[71][3] = 0b11001011;
  storage[71][4] = 0b11101011;
  storage[71][5] = 0b01101111;
  storage[71][6] = 0b00000000;
  storage[71][7] = 0b00000000;

  storage[71][8] = 0b00000110;


  storage[72][0] = 0b11111111;
  storage[72][1] = 0b11111111;
  storage[72][2] = 0b00011000;
  storage[72][3] = 0b00011000;
  storage[72][4] = 0b11111111;
  storage[72][5] = 0b11111111;
  storage[72][6] = 0b00000000;
  storage[72][7] = 0b00000000;

  storage[72][8] = 0b00000110;


  storage[73][0] = 0b11111111;
  storage[73][1] = 0b11111111;
  storage[73][2] = 0b00000000;
  storage[73][3] = 0b00000000;
  storage[73][4] = 0b00000000;
  storage[73][5] = 0b00000000;
  storage[73][6] = 0b00000000;
  storage[73][7] = 0b00000000;

  storage[73][8] = 0b00000010;


  storage[74][0] = 0b00000110;
  storage[74][1] = 0b00000111;
  storage[74][2] = 0b00000011;
  storage[74][3] = 0b00000011;
  storage[74][4] = 0b11111111;
  storage[74][5] = 0b11111110;
  storage[74][6] = 0b00000000;
  storage[74][7] = 0b00000000;

  storage[74][8] = 0b00000110;


  storage[75][0] = 0b11111111;
  storage[75][1] = 0b11111111;
  storage[75][2] = 0b00011000;
  storage[75][3] = 0b00111100;
  storage[75][4] = 0b01111110;
  storage[75][5] = 0b11100111;
  storage[75][6] = 0b10000001;
  storage[75][7] = 0b00000000;

  storage[75][8] = 0b00000111;


  storage[76][0] = 0b11111111;
  storage[76][1] = 0b11111111;
  storage[76][2] = 0b00000011;
  storage[76][3] = 0b00000011;
  storage[76][4] = 0b00000011;
  storage[76][5] = 0b00000011;
  storage[76][6] = 0b00000000;
  storage[76][7] = 0b00000000;

  storage[76][8] = 0b00000110;


  storage[77][0] = 0b11111111;
  storage[77][1] = 0b11111111;
  storage[77][2] = 0b01100000;
  storage[77][3] = 0b00110000;
  storage[77][4] = 0b01100000;
  storage[77][5] = 0b11111111;
  storage[77][6] = 0b11111111;
  storage[77][7] = 0b00000000;

  storage[77][8] = 0b00000111;


  storage[78][0] = 0b11111111;
  storage[78][1] = 0b11111111;
  storage[78][2] = 0b00111000;
  storage[78][3] = 0b00011100;
  storage[78][4] = 0b11111111;
  storage[78][5] = 0b11111111;
  storage[78][6] = 0b00000000;
  storage[78][7] = 0b00000000;

  storage[78][8] = 0b00000110;


  storage[79][0] = 0b01111110;
  storage[79][1] = 0b01111110;
  storage[79][2] = 0b11000011;
  storage[79][3] = 0b11000011;
  storage[79][4] = 0b11000011;
  storage[79][5] = 0b11000011;
  storage[79][6] = 0b01111110;
  storage[79][7] = 0b01111110;

  storage[79][8] = 0b00001000;

  storage[80][0] = 0b11111111;
  storage[80][1] = 0b11111111;
  storage[80][2] = 0b11001100;
  storage[80][3] = 0b11001100;
  storage[80][4] = 0b11111100;
  storage[80][5] = 0b01111000;
  storage[80][6] = 0b00000000;
  storage[80][7] = 0b00000000;

  storage[80][8] = 0b00000110;


  storage[81][0] = 0b01111110;
  storage[81][1] = 0b01111110;
  storage[81][2] = 0b10000001;
  storage[81][3] = 0b10000001;
  storage[81][4] = 0b10000101;
  storage[81][5] = 0b10000010;
  storage[81][6] = 0b01111101;
  storage[81][7] = 0b01111100;

  storage[81][8] = 0b0001000;


  storage[82][0] = 0b11111111;
  storage[82][1] = 0b11111111;
  storage[82][2] = 0b11001100;
  storage[82][3] = 0b11001100;
  storage[82][4] = 0b11001110;
  storage[82][5] = 0b11111111;
  storage[82][6] = 0b01110011;
  storage[82][7] = 0b00000001;

  storage[82][8] = 0b00001000;


  storage[83][0] = 0b01110010;
  storage[83][1] = 0b11111011;
  storage[83][2] = 0b11011011;
  storage[83][3] = 0b11011011;
  storage[83][4] = 0b11011111;
  storage[83][5] = 0b01001110;
  storage[83][6] = 0b00000000;
  storage[83][7] = 0b00000000;

  storage[83][8] = 0b00000110;


  storage[84][0] = 0b11000000;
  storage[84][1] = 0b11000000;
  storage[84][2] = 0b11000000;
  storage[84][3] = 0b11111111;
  storage[84][4] = 0b11111111;
  storage[84][5] = 0b11000000;
  storage[84][6] = 0b11000000;
  storage[84][7] = 0b11000000;

  storage[84][8] = 0b00001000;


  storage[85][0] = 0b11111110;
  storage[85][1] = 0b11111111;
  storage[85][2] = 0b00000011;
  storage[85][3] = 0b00000011;
  storage[85][4] = 0b00000011;
  storage[85][5] = 0b11111111;
  storage[85][6] = 0b11111110;
  storage[85][7] = 0b00000000;

  storage[85][8] = 0b0000111;


  storage[86][0] = 0b11111000;
  storage[86][1] = 0b11111110;
  storage[86][2] = 0b00000011;
  storage[86][3] = 0b00000001;
  storage[86][4] = 0b00000011;
  storage[86][5] = 0b11111110;
  storage[86][6] = 0b11111000;
  storage[86][7] = 0b00000000;

  storage[86][8] = 0b0000111;


  storage[87][0] = 0b11111111;
  storage[87][1] = 0b11111111;
  storage[87][2] = 0b00000110;
  storage[87][3] = 0b00001100;
  storage[87][4] = 0b00000110;
  storage[87][5] = 0b11111111;
  storage[87][6] = 0b11111111;
  storage[87][7] = 0b00000000;

  storage[87][8] = 0b00000111;


  storage[88][0] = 0b11000011;
  storage[88][1] = 0b11100111;
  storage[88][2] = 0b01111110;
  storage[88][3] = 0b00111100;
  storage[88][4] = 0b00111100;
  storage[88][5] = 0b01111110;
  storage[88][6] = 0b11100111;
  storage[88][7] = 0b11000011;

  storage[88][8] = 0b00001000;


  storage[89][0] = 0b11100000;
  storage[89][1] = 0b11111000;
  storage[89][2] = 0b00011111;
  storage[89][3] = 0b00011111;
  storage[89][4] = 0b11111000;
  storage[89][5] = 0b11100000;
  storage[89][6] = 0b00000000;
  storage[89][7] = 0b00000000;

  storage[89][8] = 0b00000110;

  storage[90][0] = 0b11000011;
  storage[90][1] = 0b11000111;
  storage[90][2] = 0b11001111;
  storage[90][3] = 0b11011011;
  storage[90][4] = 0b11110011;
  storage[90][5] = 0b11100011;
  storage[90][6] = 0b00000000;
  storage[90][7] = 0b00000000;

  storage[90][8] = 0b00000110;

}
