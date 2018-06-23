// Sourced from LadyAda on https://blog.adafruit.com/2009/10/20/example-code-for-multi-button-checker-with-debouncing/


#define DEBOUNCE 50  // Debounce delay
// use this somewhere
#define CONFIRMATION_TIME 50

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {5, 6, 9, 10, 11}; 

#define NUMBUTTONS 5

// we will track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];
char letter;
int mapTo;
unsigned long timeDiff;
int rowNumber = 0;
int rowPresses = 3; //rowPresses should automatically be on rowNumber 0, or 1 on the physical board
// mod 3 --> 3 states: 0, 1, 2 to map to a rowNumber;

int row1LED = 12;
int row2LED = 13;

void setup() {
  byte i;
  // set up serial port
  Serial.begin(9600);
  // pin13 LED
  pinMode(row1LED, OUTPUT);
  pinMode(row2LED, OUTPUT);
  digitalWrite(row2LED, LOW);
  digitalWrite(row1LED, LOW);

  
  // Make input & enable pull-up resistors on switch pins
  for (i=0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    digitalWrite(buttons[i], HIGH);
  }
}

void check_switches() {
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  int button;
  int lastButton;
  byte index;
  if (millis() < lasttime) {
     lasttime = millis(); // we wrapped around, lets just try again
  }
 
 if ((lasttime + DEBOUNCE) > millis())  {
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
  switch (mapTo) {
    case 0:
      if (rowNumber == 0) { letter = 'a'; }
      else if (rowNumber == 1) { letter = 'k'; }
      else if (rowNumber == 2) { letter = 'u'; }
      break;
    case 1:
      if (rowNumber == 0) { letter = 'b'; }
      else if (rowNumber == 1) { letter = 'l'; }
      else if (rowNumber == 2) { letter = 'v'; }
      break;
    case 10:
      if (rowNumber == 0) { letter = 'c'; }
      else if (rowNumber == 1) { letter = 'm'; }
      else if (rowNumber == 2) { letter = 'w'; }
      break;
    case 2:
      if (rowNumber == 0) { letter = 'd'; }
      else if (rowNumber == 1) { letter = 'n'; }
      else if (rowNumber == 2) { letter = 'x'; }
      break;
    case 20:
      if (rowNumber == 0) { letter = 'e'; }
      else if (rowNumber == 1) { letter = 'o'; }
      else if (rowNumber == 2) { letter = 'y'; }
      break;
    case 21:
      if (rowNumber == 0) { letter = 'f'; }
      else if (rowNumber == 1) { letter = 'p'; }
      else if (rowNumber == 2) { letter = 'z'; }
      break;
    case 3:
      if (rowNumber == 0) { letter = 'g'; }
      else if (rowNumber == 1) { letter = 'q'; }
      else if (rowNumber == 2) { letter = '.'; }
      break;
    case 30:
      if (rowNumber == 0) { letter = 'h'; }
      else if (rowNumber == 1) { letter = 'r'; }
      else if (rowNumber == 2) { letter = '.'; }
      break;
    case 31:
      if (rowNumber == 0) { letter = 'i'; }
      else if (rowNumber == 1) { letter = 's'; }
      else if (rowNumber == 2) { letter = ','; }
      break;
    case 32:
      if (rowNumber == 0) { letter = 'j'; }
      else if (rowNumber == 1) { letter = 't'; }
      else if (rowNumber == 2) { letter = '!'; }
      break;
    case 40:
      letter = ' ';
      break;
    case 41:
      letter = '\n';
      break;
    case 42:
      letter = '\t';
      break;
    default:
      letter = '??';
      break;
  }
  int mapTo = 0; //reset mapTo
}

void loop() {
  // when we check the switches we'll get the current state
  check_switches();      
  int numPresseses = 0;
  byte storedStates[NUMBUTTONS];
  
  
  //for some reason the first index gives a random number, so stored[1] [2] contain the buttons
  int stored[3]; 
  for (int i = 0; i < NUMBUTTONS; i++) {
    if (justreleased[i]) {
        numPresseses++;
        stored[numPresseses] = i;
        justreleased[i] = 0; //need to reset so it doesn't repeat in monitor
        storedStates[i] = 1; //migrate justpressed values to another array if numPresseses != 2
    }
    else {
      storedStates[i] = 0;
    }
  }

  if (numPresseses == 2 && (stored[1] != 4 && stored[0] != 4)) { 
    // have to press on them the same exact time, and make sure it's not a row button
    //    Serial.print("found 2 presses: ");
    //    for (int i = 1 ; i < numPresseses+1 ; i++) {
    ////      Serial.print(stored[i]);
    ////      Serial.print(", ");
    //    }
    mapTo = max(stored[1] * 10 + stored[2], stored[2]*10 + stored[1]);
    
    findLetter();
    Serial.print(letter);
  }
  
  else {
  
    for (int i = 0; i < NUMBUTTONS; i++) {

      if (storedStates[i]) { //since numPresseses !=2, the storedStates array becomes useful
        
         if (i != 4) { //keys are being entered
           storedStates[i] = 0;           
           mapTo =  i;
           findLetter();
           Serial.print(letter);
         }
         
         else { // rows are being toggled
            rowPresses += 1;
            rowNumber = rowPresses % 3; 
            if (rowNumber == 0) {
              digitalWrite(row1LED, LOW);
              digitalWrite(row2LED, LOW);
            }
            if (rowNumber == 1) {
              digitalWrite(row1LED, HIGH);
              digitalWrite(row2LED, LOW);
            }

            if (rowNumber == 2) {
              digitalWrite(row1LED, HIGH);
              digitalWrite(row2LED, HIGH);
            }
         }
      }
      // remember, check_switches() will CLEAR the 'just pressed' flag
      if (justreleased[i]) {
          //Serial.print(i, DEC);
          Serial.println(" Just released");
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
