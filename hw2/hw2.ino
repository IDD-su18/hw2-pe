// Sourced from LadyAda on https://blog.adafruit.com/2009/10/20/example-code-for-multi-button-checker-with-debouncing/


#define DEBOUNCE 100  // Debounce delay
 
// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {9, 10, 11, 12, 13}; 
 
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
 
// we will track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];
char letter;
int mapTo;
int rowNumber = 0;
int rowPresses = 3; //rowPresses should automatically be on rowNumber 0, or 1 on the physical board
// mod 3 --> 3 states: 0, 1, 2 to map to a rowNumber;
 
void setup() {
  byte i;
 
  // set up serial port
  Serial.begin(9600);
  // pin13 LED
  pinMode(13, OUTPUT);
 
  // Make input & enable pull-up resistors on switch pins
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    digitalWrite(buttons[i], HIGH);
  }
}

void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  int button;
  int lastButton;
  byte index;
  if (millis() < lasttime) {
     lasttime = millis(); // we wrapped around, lets just try again
  }
 
  if ((lasttime + DEBOUNCE) > millis()) {
    return; // not enough time has passed to debounce
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
 
  for (index = 0; index < NUMBUTTONS; index++) {
    justpressed[index] = 0;       // when we start, we clear out the "just" indicators
    justreleased[index] = 0;
 
    currentstate[index] = digitalRead(buttons[index]);   // read the button
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}

void findLetter() { //hardcode the mapping!
  if(rowNumber == 0) { //access letters A,B,C,D,E,F,G,H,I
    if(mapTo == 1) {
      letter = 'a';
    }
    else if(mapTo == 2) {
      letter = 'b';
    }
    else if(mapTo == 3) {
      letter = 'c';
    }
    else if(mapTo == 4) {
      letter = 'd';
    }
    else if(mapTo == 12 || mapTo == 21) {
      letter = 'e';
    }
    else if(mapTo == 13 || mapTo == 31) {
      letter = 'f';
    }
    else if(mapTo == 14 || mapTo == 41) {
      letter = 'g';
    }
    else if(mapTo == 23 || mapTo == 32) {
      letter = 'h';
    }
    else if(mapTo == 34 || mapTo == 43) {
      letter = 'i';
    }
  }

  else if (rowNumber == 1) { //access letters J,K,L,M,N,O,P,Q,R
    if(mapTo == 1) {
      letter = 'j';
    }
    else if(mapTo == 2) {
      letter = 'k';
    }
    else if(mapTo == 3) {
      letter = 'l';
    }
    else if(mapTo == 4) {
      letter = 'm';
    }
    else if(mapTo == 12 || mapTo == 21) {
      letter = 'n';
    }
    else if(mapTo == 13 || mapTo == 31) {
      letter = 'o';
    }
    else if(mapTo == 14 || mapTo == 41) {
      letter = 'p';
    }
    else if(mapTo == 23 || mapTo == 32) {
      letter = 'q';
    }
    else if(mapTo == 34 || mapTo == 43) {
      letter = 'r';
    }
  }

  else { //rowNumber = 2 //access letters S,T,U,V,W,X,Y,Z
    if(mapTo == 1) {
      letter = 's';
    }
    else if(mapTo == 2) {
      letter = 't';
    }
    else if(mapTo == 3) {
      letter = 'u';
    }
    else if(mapTo == 4) {
      letter = 'v';
    }
    else if(mapTo == 12 || mapTo == 21) {
      letter = 'w';
    }
    else if(mapTo == 13 || mapTo == 31) {
      letter = 'x';
    }
    else if(mapTo == 14 || mapTo == 41) {
      letter = 'y';
    }
    else if(mapTo == 23 || mapTo == 32) {
      letter = 'z';
    }
    else if(mapTo == 34 || mapTo == 43) {
      letter = 10; //line feed, change to 32 for space
    }
    
  }
  int mapTo = 0; //reset mapTo
}
  
void loop() {
  check_switches();      // when we check the switches we'll get the current state
  int noPresses = 0;
  byte storedStates[NUMBUTTONS];
  int stored[3]; //for some reason the first index gives a random number, so stored[1] [2] contain the buttons
  for (int i = 0; i < NUMBUTTONS; i++) {
    if (justpressed[i]) {
        noPresses++;
        stored[noPresses] = i;
        justpressed[i] = 0; //need to reset so it doesn't repeat in monitor
        storedStates[i] = 1; //migrate justpressed values to another array if noPresses != 2
      
    }
    else {
      storedStates[i] = 0;
    }
  }

  if (noPresses == 2 && (stored[1] != 0 && stored[0] != 0)) { //have to press on them the same exact time, and make sure it's not a row button
//    Serial.print("found 2 presses: ");
    for (int i = 1 ; i < noPresses+1 ; i++) {
//      Serial.print(stored[i]);
//      Serial.print(", ");
    }
//    Serial.println(" was pressed ");
    int interm1 = stored[1] * 10;
    mapTo = interm1 + stored[2];
    findLetter();
    Serial.print(letter);
    }
    
 else {
  for (byte i = 0; i < NUMBUTTONS; i++) {
      if (storedStates[i]) { //since noPresses !=2, the storedStates array becomes useful
         if (i !=0) { //keys are being entered
//         Serial.println(i, DEC);
         storedStates[i] = 0;
         mapTo = (int) i;
         findLetter();
         Serial.print(letter);
         }
         else { // rows are being toggled
          rowPresses += 1;
          rowNumber = rowPresses % 3; 
          Serial.print("Currently on row ");
          Serial.println(rowNumber);
          storedStates[i] = 0;
         }
      }
      // remember, check_switches() will CLEAR the 'just pressed' flag
    if (justreleased[i]) {
      //Serial.print(i, DEC);
      //Serial.println(" Just released");
      justreleased[i] = 0;
      // remember, check_switches() will CLEAR the 'just pressed' flag
    }
    if (pressed[i]) {
      //Serial.print(i, DEC);
      // Serial.println(" pressed");
      // button = (int) i;
      // is the button pressed down at this moment
    }
  }
 }
}
