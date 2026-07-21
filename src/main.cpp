#include <Arduino.h>
#include <SoftwareSerial.h>

/*-------------------------------- Pin Definitions --------------------------------*/
#define Smoke_sensor 2
#define Flame_sensor 6
#define Green_led 3
#define Red_led 5
#define Buzzer 4
#define GSM_RX_PIN 7
#define GSM_TX_PIN 8

SoftwareSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN);

/* ---------------------- GSM / SMS Settings ---------------------- */
const char* phoneNumber = "YOUR_PHONE_NUMBER"; // Include country code, e.g. 91XXXXXXXXXX
const char* alarmMessage = "FIRE EMERGENCY. Evacuate now!";
bool gsmReady = false;

/* ---------------------- Sensor Status Variables ---------------------- */
bool flame_status = false;
bool smoke_status = true;

bool gsmWaitForResponse(const char* expected, unsigned long timeout) {
  gsmSerial.listen();
  String response = "";
  unsigned long start = millis();

  while (millis() - start < timeout) {
    while (gsmSerial.available()) {
      char c = gsmSerial.read();
      response += c;
      if (response.indexOf(expected) != -1) {
        Serial.println(response);
        return true;
      }
    }
    delay(20);
  }

  Serial.print("GSM response: ");
  Serial.println(response);
  return false;
}

bool initGSM() {
  Serial.println("Initializing GSM module...");
  gsmSerial.listen();
  gsmSerial.println("AT");
  if (!gsmWaitForResponse("OK", 3000)) {
    return false;
  }

  gsmSerial.println("ATE0");
  if (!gsmWaitForResponse("OK", 3000)) {
    return false;
  }

  gsmSerial.println("AT+CMGF=1");
  if (!gsmWaitForResponse("OK", 3000)) {
    return false;
  }

  Serial.println("GSM module ready.");
  return true;
}

bool sendSMS(const char* recipient, const char* message) {
  gsmSerial.listen();
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(recipient);
  gsmSerial.println("\"");

  if (!gsmWaitForResponse(">", 5000)) {
    return false;
  }

  gsmSerial.print(message);
  gsmSerial.write(26); // Ctrl+Z
  return gsmWaitForResponse("OK", 10000);
}

/* ------------------------ Fire & Smoke detect Function ------------------------- */
bool fire_smoke_detect() {
  flame_status = digitalRead(Flame_sensor);
  smoke_status = digitalRead(Smoke_sensor);
  if (!flame_status || !smoke_status) {
    return true;
  } else {
    return false;
  }
}

/* -------------------------- Alarm Trigger Function ----------------------------- */
void trigger_Alarm() {
  static unsigned long prevmillis = 0;
  static bool buzzer_state = false;
  const unsigned long beep_interval = 100;

  digitalWrite(Red_led, HIGH);
  digitalWrite(Green_led, LOW);

  if (millis() - prevmillis >= beep_interval) {
    prevmillis = millis();
    buzzer_state = !buzzer_state;
    digitalWrite(Buzzer, buzzer_state);
  }
}

/* ------------------------- Setup Function --------------------------------*/
void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);

  pinMode(Smoke_sensor, INPUT);
  pinMode(Flame_sensor, INPUT);
  pinMode(Green_led, OUTPUT);
  pinMode(Red_led, OUTPUT);
  pinMode(Buzzer, OUTPUT);

  digitalWrite(Green_led, HIGH);
  digitalWrite(Red_led, LOW);
  digitalWrite(Buzzer, LOW);

  gsmReady = initGSM();
  if (!gsmReady) {
    Serial.println("GSM initialization failed. Check wiring and baud rate.");
  }
}

/* ---------------------------- Loop Function ---------------------------------- */
void loop() {
  if (fire_smoke_detect()) {
    if (gsmReady) {
      sendSMS(phoneNumber, alarmMessage);
    }

    while (true) {
      trigger_Alarm();
    }
  }

  delay(100);
}
