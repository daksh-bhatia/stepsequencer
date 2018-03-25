const int channel = 1;

int buttonPin[6] = { 7, 8, 9, 10, 33, 34};
int ledPin [4] = { 2, 3, 4, 5};

int tempo = 0;
int tempoPot = A5;

boolean lastButtonState[6] = { LOW, LOW, LOW, LOW, LOW, LOW};
boolean buttonState[6] = { LOW, LOW, LOW, LOW, LOW, LOW};

boolean stepState[3][4] = {
  { false, false, false, false},
  { false, false, false, false},
  { false, false, false, false},
};

int currentChannel = 0;
int currentStep = 0;
int midiNote[4] = {61, 62, 63, 64};
unsigned long lastStepTime = 0;

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(buttonPin[i], INPUT);

  }
  for (int i = 0; i < 4; i++) {
    pinMode(ledPin[i], OUTPUT);

  }
}

void loop() {
  updateChannel();
  checkButtons();
  sequence();
  setLeds();
}

void updateChannel() {
  for (int i = 4; i < 6; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPin[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (i == 4) {
        currentChannel++;
        if (currentChannel > 2) {
          currentChannel = 0;
        }
      }
      if (i == 5) {
        currentChannel--;
        if (currentChannel < 0) {
          currentChannel = 2;
        }
      }

    }

  }
}

void checkButtons() {
  for (int i = 0; i < 4; i++) {
    lastButtonState[i] = buttonState[i];
    buttonState[i] = digitalRead(buttonPin[i]);

    if (buttonState[i] == HIGH && lastButtonState[i] == LOW) {
      if (stepState[currentChannel][i] == false) {
        stepState[currentChannel][i] = true;
      } else if (stepState[currentChannel][i] == true) {
        stepState[currentChannel][i] = false;
      }
      //
      //      if(stepState[i] == true) {
      //      digitalWrite(ledPin[i], HIGH);
      //    } else if(stepState[i] == false) {
      //        digitalWrite(ledPin[i], LOW);
      //    }
    }
  }
}

void sequence() {

  tempo = map(analogRead(tempoPot), 0, 1023, 3, 500);

  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...

    for (int i = 0; i < 3; i++) {

      if (stepState[i][currentStep] == HIGH) {
        usbMIDI.sendNoteOn(midiNote[i], 80, 1);
      }
    }

    currentStep = currentStep + 1;         //increment to the next step
    if (currentStep > 3) {
      currentStep = 0;
    }
    //    digitalWrite(ledPin[currentStep], HIGH); //turn on the new led

    lastStepTime = millis();               //set lastStepTime to the current time
  }
}

void setLeds() {
  for (int i = 0; i < 4; i++) {

    if (currentStep == i) {
      analogWrite(ledPin[i], 255);
    }
    else if (stepState[currentChannel][i] == HIGH) {
      analogWrite(ledPin[i], 30);

    }
    else {
      digitalWrite(ledPin[i], LOW);

    }
  }
}




