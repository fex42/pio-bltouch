#include <Arduino.h>
#include <Servo.h>

// ==== Configuration Section ====

// ---- Host communication configuration ----
#define COMM_BPS 9600 // Baudrate for host communication
#define COMM_EOL '\n' // End-of-line terminal symbol 

// ---- BLTouch wiring ----
#define PIN_CONTROL 3 // Arduino pin for BLTouch control wire
#define PIN_Z_MIN 2 // Arduino pin for z-min signal

// ---- BLTouch control ----
#define CMD_PUSH_DOWN 10 // Degrees for Push-pin Down command
#define CMD_PUSH_UP 90 // Degrees for Push-pin Up command
#define CMD_SELF_TEST 120 // Degrees for Self test command
#define CMD_ALARM_RELEASE 160 // Degrees for Alarm Release & Push-pin UP command

// ==============================

Servo blControl;

const String CMD_DOWN("d");
const String CMD_UP("u");
const String CMD_TEST("t");
const String CMD_ALARM("a");
const String CMD_CONFIG("c");
const String CMD_HELP("h");

#define L String
#define ENDL String('\n')


void onZMinTrigger() {
  Serial.println( HIGH == digitalRead(PIN_Z_MIN) ? "Z-Min is HIGH" : "Z-Min is LOW" );
}


void printHelp() {
  Serial.println(
      L("Usage:") + ENDL +
      L("\t'") + CMD_HELP + L("': Print usage / list of valid commands.")  + ENDL +
      L("\t'") + CMD_CONFIG + L("': Print current configuration.")  + ENDL +
      L("\t'") + CMD_DOWN + L("': Execute Push-pin Down / Probe start.")  + ENDL +
      L("\t'") + CMD_UP + L("': Execute Push-pin Up / Probe finish.")  + ENDL +
      L("\t'") + CMD_TEST + L("': Execute self-test.")  + ENDL +
      L("\t'") + CMD_ALARM + L("': Execute Alarm Release & Push-pin UP.")  + ENDL
  );
}

void printWelcome() {
  Serial.println("==== BLTouch tester started ====");
  printHelp();
}

void printConfig() {
    Serial.println(
      L("Current configuration:") + ENDL +
      L("\tBaudrate: ") + L(COMM_BPS)+ L(" bps")  + ENDL +
      L("\tBLTouch control wire pin: ")+ L(PIN_CONTROL) + ENDL +
      L("\tZ-Min signal pin: ") + L(PIN_Z_MIN) + ENDL +

      L("\tPush-pin Down: ") + L(CMD_PUSH_DOWN) + L(" deg") + ENDL +
      L("\tPush-pin Up: ") + L(CMD_PUSH_UP) + L(" deg") + ENDL +
      L("\tSelf test: ") + L(CMD_SELF_TEST) + L(" deg") + ENDL +
      L("\tAlarm Release & Push-pin UP: ") + L(CMD_ALARM_RELEASE) + L(" deg") + ENDL
    );
}

void setup() {
  // Initialize host communication
  Serial.begin(COMM_BPS);

  // Initialize servo for BLTouch control
  blControl.attach(PIN_CONTROL);

  // Initialize Z-Min handler
  pinMode(PIN_Z_MIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_Z_MIN), onZMinTrigger, CHANGE);

  printWelcome();
}

void loop() {
  String cmd = Serial.readStringUntil(COMM_EOL);
  cmd.trim();

  if(!cmd.length())
    return;

  if(CMD_DOWN.equalsIgnoreCase(cmd)) {
    Serial.println("Executing Push-pin Down / Probe start");
    blControl.write(CMD_PUSH_DOWN);
  }
  else if(CMD_UP.equalsIgnoreCase(cmd)) {
    Serial.println("Executing Push-pin Up / Probe finished");
    blControl.write(CMD_PUSH_UP);
  }
  else if(CMD_TEST.equalsIgnoreCase(cmd)) {
    Serial.println("Executing self-test");
    blControl.write(CMD_SELF_TEST);
  }
  else if(CMD_ALARM.equalsIgnoreCase(cmd)) {
    Serial.println("Executing Alarm Release & Push-pin UP");
    blControl.write(CMD_ALARM_RELEASE);
  }
  else if(CMD_CONFIG.equalsIgnoreCase(cmd)) {
    printConfig();
  }
  else if(CMD_HELP.equalsIgnoreCase(cmd)) {
    printHelp();
  }
  else {
    Serial.println( L("Unknown command '") + cmd + L("'. Use '") + CMD_HELP + L("' for list of valid commands.") );
  }
}
