const int SOUND_SENSOR = A0; //pin for the Grove Sound Sensor
//const int LAMP_RELAY = 2; //pin for the Grove Relay
const int THRESHOLD = 700; //the sound level that will be treated as a 'clap'
const int SOUND_SAMPLE_LENGTH = 200; //the amount of ms to wait before determining to turn off/on
const int CLAP_DURATION_WINDOW = 1500; //the amount of ms max to make the number of claps specified (ms)
const int CLAPS_FOR_TRIGGER = 1; //the number of claps for the relay to trigger

const int LED = 7;

//kind of used the same way as 'delay' but does not pause code.
//I use this because I have multiple 'delays' running in my original code.
//this 'delay' will make sure the relay does not switch on and off to fast.
//The current time is set to 1000 ms min (in code below)
unsigned long lastLampRelayLoop = 0;
int soundSensorReading = 0;
int soundLength = 0;
int previousSoundLength = 0;
int soundSampleLength = SOUND_SAMPLE_LENGTH;
int clapDurationWindow = CLAP_DURATION_WINDOW;
int currentClaps = 0;
int relayState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  soundSensorReading = analogRead(SOUND_SENSOR);

  if (soundSensorReading >= THRESHOLD) { 
    soundLength++;
    Serial.println(soundLength);
  } else if (soundSensorReading < THRESHOLD && soundSensorReading >= THRESHOLD - 300) {
//    Serial.println("none");
  } else {
    if (soundLength > 1) {
      previousSoundLength = soundLength;
    }

    soundLength = 0;
  }

  if (soundSampleLength == SOUND_SAMPLE_LENGTH) {
    soundSampleLength = 0;
    Serial.println("sample");
//    Serial.println("sound length = " + soundLength);

    if (previousSoundLength > 1) { 
      clapDurationWindow = 0;
      currentClaps++;

//      if (currentClaps == CLAPS_FOR_TRIGGER) {
        //relayState = !relayState;

        if (millis()-lastLampRelayLoop >= 500) {
          relayState = !relayState;
          Serial.println("convert");
          digitalWrite(LED, relayState);
          lastLampRelayLoop = millis();
        }
//      }

      previousSoundLength = 0;
    }
  }

  if (clapDurationWindow >= CLAP_DURATION_WINDOW) {
    currentClaps = 0; 
  }

  if (clapDurationWindow <= CLAP_DURATION_WINDOW) {
    clapDurationWindow++;
  }

  if (soundSampleLength < SOUND_SAMPLE_LENGTH) {
    soundSampleLength++; 
  }

  delay(1);
}
