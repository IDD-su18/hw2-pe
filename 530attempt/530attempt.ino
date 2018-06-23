/*
Code based off of example vy Brian McEvoy.
24hourengineer.com
Program is distributable for personal use.
*/


// Inputs. Buttons may be addressed by name but the program expects all buttons after the pinky
// to be numbered sequentially.
int pinkyButton = 5;
int ringButton = 6;
int middleButton = 9;
int indexButton = 10;
int thumbButton = 11;

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
boolean stickySpecialLock = LOW;



void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial.begin(9600);
  Serial.println("Up and runnning");

  randomSeed(analogRead(0));
  
  pinMode(pinkyButton, INPUT_PULLUP);
  pinMode(ringButton, INPUT_PULLUP);
  pinMode(middleButton, INPUT_PULLUP);
  pinMode(indexButton, INPUT_PULLUP);
  pinMode(thumbButton, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
    acquiringPresses = checkButtonArray();
  
  if (acquiringPresses && onlyFarThumbPressed(farTButton)){
    doMouseSTUFF();
  }
  
  if (acquiringPresses){
    delay(debounceDelay);                           // Instead of a true software debounce this will wait a moment until the first button press has settled.
    typingChord();                      // Wait and see which keys are touched. When they are all released print the correct letter.
    updateShiftKeys();          // Change the prefixChord value if any of the 'locks' are set. Example, Num Lock or Caps Lock.
    sendKeyPress();                     // Using the buttons pressed during the typingChord function determine how to handle the chord.
    delay(debounceDelay);                           // The other half of the software "debounce"
    for (int i = 0; i < 7; i++){    // Once a keypress has been sent the booleans should be reset.
      latchingButtons[i] = LOW;
    }
    chordValue = 0;
  }
}
