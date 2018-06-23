/*
Code based off of example vy Brian McEvoy.
24hourengineer.com
Program is distributable for personal use.
*/

#define NUM_INPUTS 5


// Inputs. Buttons may be addressed by name but the program expects all buttons after the pinky
// to be numbered sequentially.
int pinkyButton = 9;
int ringButton = 10;
int middleButton = 11;
int indexButton = 12;
int thumbButton = 13;

int prefixChord = 0;            // 1 = shift (F). 2 = numlock (N). 3 = special (CN). 4 = function keys
int chordValue = 0;
int randomNumber01;
int randomNumber02;
int randomNumber03;
int randomNumber04;
int debounceDelay = 20;

// Booleans
boolean buttons[5];     // Pinky is [0] and far thumb is [6]
boolean latchingButtons[5];
boolean acquiringPresses = LOW;
boolean calculateKey = LOW;

int rowNumber = 0;
int rowPresses = 3; 


int row1LED = 5;
int row2LED = 6;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  Serial.println("Up and runnning");

  //randomSeed(analogRead(0));
  
  pinMode(pinkyButton, INPUT_PULLUP);
  pinMode(ringButton, INPUT_PULLUP);
  pinMode(middleButton, INPUT_PULLUP);
  pinMode(indexButton, INPUT_PULLUP);
  pinMode(thumbButton, INPUT_PULLUP);
  pinMode(row1LED, OUTPUT);
  pinMode(row2LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  acquiringPresses = checkButtonArray();
  
  if (acquiringPresses){
    delay(debounceDelay);                           // Instead of a true software debounce this will wait a moment until the first button press has settled.
    typingChord();                      // Wait and see which keys are touched. When they are all released print the correct letter.
    sendKeyPress();                     // Using the buttons pressed during the typingChord function determine how to handle the chord.
    delay(debounceDelay);                           // The other half of the software "debounce"
    for (int i = 0; i < NUM_INPUTS ; i++){    // Once a keypress has been sent the booleans should be reset.
      latchingButtons[i] = LOW;
    }
    chordValue = 0;
  }
}

boolean checkButtonArray(){
  // Update the buttons[] array with each scan. Set the acquiringPresses bit HIGH if any switch is pressed.
  for (int i = 0; i < NUM_INPUTS; i++){
    // idk what this is doing
    boolean buttonState = !digitalRead(pinkyButton + i);
    if (buttonState){
      buttons[i] = HIGH;
    } else{
      buttons[i] = LOW;
    }
  }
  for (int i = 0; i < NUM_INPUTS; i++){
    if (buttons[i]){
      return HIGH;
    }
  }
  return LOW;
}

void typingChord(){
  while (acquiringPresses){
    for (int i = 0; i < NUM_INPUTS; i++){
      if (buttons[i] == HIGH){
        latchingButtons[i] = HIGH;
      }
    }
    acquiringPresses = checkButtonArray();
  }
}

void sendKeyPress(){
  for (int i = 0; i < NUM_INPUTS; i++){
    if (latchingButtons[i] == HIGH){
      chordValue = chordValue + customPower(2, i);
      
    }
  }

  // if the leftmost key was pressed, toggle the row and leds
  if (chordValue == 16) {
    Serial.println("yo");
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
  else {
    Serial.print(char(findLetter(chordValue)));
  }
}

int customPower(int functionBase, int functionExponent){
  int functionResult = 1;
  for (int i = 0; i < functionExponent; i++){
    functionResult = functionResult * functionBase;
  }
  
  return functionResult;
}

int findLetter(int chordValue) { //hardcode the mapping!
//  Serial.print("chord value = ");
//  Serial.println(chordValue);
  switch (chordValue) {
    case 1:
      if (rowNumber == 0) { return 'a'; }
      else if (rowNumber == 1) { return 'k'; }
      else if (rowNumber == 2) { return 'u'; }
      break;
    case 2:
      if (rowNumber == 0) { return 'b'; }
      else if (rowNumber == 1) { return 'l'; }
      else if (rowNumber == 2) { return 'v'; }
      break;
    case 3:
      if (rowNumber == 0) { return 'c'; }
      else if (rowNumber == 1) { return 'm'; }
      else if (rowNumber == 2) { return 'w'; }
      break;
    case 4:
      if (rowNumber == 0) { return 'd'; }
      else if (rowNumber == 1) { return 'n'; }
      else if (rowNumber == 2) { return 'x'; }
      break;
    case 5:
      if (rowNumber == 0) { return 'e'; }
      else if (rowNumber == 1) { return 'o'; }
      else if (rowNumber == 2) { return 'y'; }
      break;
    case 6:
      if (rowNumber == 0) { return 'f'; }
      else if (rowNumber == 1) { return 'p'; }
      else if (rowNumber == 2) { return 'z'; }
      break;
    case 8:
      if (rowNumber == 0) { return 'g'; }
      else if (rowNumber == 1) { return 'q'; }
      else if (rowNumber == 2) { return '.'; }
      break;
    case 9:
      if (rowNumber == 0) { return 'h'; }
      else if (rowNumber == 1) { return 'r'; }
      else if (rowNumber == 2) { return '.'; }
      break;
    case 10:
      if (rowNumber == 0) { return 'i'; }
      else if (rowNumber == 1) { return 's'; }
      else if (rowNumber == 2) { return ','; }
      break;
    case 12:
      if (rowNumber == 0) { return 'j'; }
      else if (rowNumber == 1) { return 't'; }
      else if (rowNumber == 2) { return '!'; }
      break;
    case 17:
      return ' ';
      break;
    case 18:
      return '\n';
      break;
    case 20:
      return '\t';
      break;
    default:
      return '\0';
      break;
  }
}


