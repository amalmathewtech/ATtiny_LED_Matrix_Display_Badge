#define MAP_START      32

#define DISPLAY_WIDTH  4
#define DISPLAY_HEIGHT 5

// "pixels" per second
#define SPEED          12       

// the text to display
#define DISPLAY_STRING "  ATTINY LED MATRIX DISPLAY BADGE    "





// maps characters to their 4x5 grid 
unsigned long characterMap[59];

// set up a character in the characterMap
void Chr(char theChar, unsigned long value) {
  characterMap[theChar - MAP_START] = value;
}

// The offset of our string in the display
int offset = 0;
unsigned long lastMillis = 0;
unsigned long currentMillis = 0;
unsigned int timeout;

char myString[] = DISPLAY_STRING;
int length = sizeof(myString);

// render the string on the given offset
void renderString(char *theString, int offset) {
  int index = 0;
  while (theString[index]) {
    renderCharacter(theString[index], offset - index * (DISPLAY_WIDTH + 1));
    index++;
  }
}

// render a character on the given offset
void renderCharacter(char theChar, int charOffset) {
  if (charOffset <= -DISPLAY_WIDTH || charOffset > DISPLAY_WIDTH) {
    // off the 'screen' nothing to do
    return;
  }

  unsigned long graphic = characterMap[theChar - MAP_START];

  for (byte y = 0; y < DISPLAY_HEIGHT; y++) {
    for (byte x = 0; x < DISPLAY_WIDTH; x++) {
      
      setPixel(3 - x + charOffset, y, graphic & 0x1);
      graphic = graphic >> 1;
    }
  }
}

// light a pixel at the given coordinates
void setPixel(byte x, byte y, boolean ledStatus) {
  if (x >= 0 && x < DISPLAY_WIDTH) {
    if (y <= x) {
      x++;
    }
    setLed(y, x, ledStatus);
  }
}

// turn on the pins to light a LED
void setLed(byte vin, byte gnd, boolean ledStatus) {
  delay(1);
  pinMode(0, INPUT); 
  pinMode(1, INPUT); 
  pinMode(2, INPUT); 
  pinMode(3, INPUT); 
  pinMode(4, INPUT); 

  if(!ledStatus) return;

  pinMode(vin, OUTPUT);   
  pinMode(gnd, OUTPUT); 
  digitalWrite(vin, HIGH);
  digitalWrite(gnd, LOW); 
}

// runs at start
void setup() {
  // set up render map

  // Rows:   1---2---3---4---5---
  Chr('A', 0b10011001111110010110);
  Chr('B', 0b01111001011110010111);
  Chr('C', 0b11100001000100011110);
  Chr('D', 0b01111001100110010111);
  Chr('E', 0b11110001011100011111);
  Chr('F', 0b00010001011100011111);
  Chr('G', 0b01101001110100011110);
  Chr('H', 0b10011001111110011001);
  Chr('I', 0b11100100010001001110);
  Chr('J', 0b00100101010001001110);
  Chr('K', 0b10010101001101011001);
  Chr('L', 0b11110001000100010001);
  Chr('M', 0b10011001111111111001);
  Chr('N', 0b10011001110110111001);
  Chr('O', 0b01101001100110010110);
  Chr('P', 0b00010001011110010111);
  Chr('Q', 0b10000110110110010110);
  Chr('R', 0b10010101011110010111);
  Chr('S', 0b11111000111100011111);
  Chr('T', 0b01000100010001001110);
  Chr('U', 0b01101001100110011001);
  Chr('V', 0b00100101100110011001);
  Chr('W', 0b01101111111110011001);
  Chr('X', 0b10011001011010011001);
  Chr('Y', 0b00110100011010011001);
  Chr('Z', 0b11110010010010001111);
  Chr(' ', 0b00000000000000000000);
  Chr('1', 0b11100100010001100100);
  Chr('2', 0b11110001011010000111);
  Chr('3', 0b01111000011010000111);
  Chr('4', 0b01001111010101100100);
  Chr('5', 0b01111000011100011111);
  Chr('6', 0b01101001011100010110);
  Chr('7', 0b00010010010010001111);
  Chr('8', 0b01101001011010010110);
  Chr('9', 0b01101000111010010110);
  Chr('0', 0b01101001110110110110);

  // how long to wait between shifting the display
  timeout = 1000 / SPEED;
}

// loops continuously
void loop() {
  currentMillis = millis();

  renderString(myString, offset);

  if (currentMillis - lastMillis > timeout) {
    lastMillis = currentMillis;
    // shift string over one "pixel"
    offset++;
    // if it's past the length of the string, start over from the beginning
    if (offset > length * (DISPLAY_WIDTH + 1)) {
      offset = -DISPLAY_WIDTH;
    }
  }
}
