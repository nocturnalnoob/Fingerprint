#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

const int RX_PIN = 2;
const int TX_PIN = 3;

SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t id;

const int FINGERPRINT_IMAGE_DELAY = 2000;

void setup() {
  Serial.begin(9600);
  delay(10); // Reduce delay in setup

  Serial.println("\n\nFingerprint sensor enrollment");

  finger.begin(57600);
  if (!finger.verifyPassword()) {
    Serial.println("Did not find fingerprint sensor :(");
    return; // Exit setup
  }
}

uint8_t readNumber() {
  while (!Serial.available());
  return Serial.read() - '0';
}

void loop() {
  if (Serial.available() > 0) {
    Serial.println("Ready to enroll a fingerprint for this car");
    Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    id = readNumber();
    if (id == 0) { // ID #0 not allowed, try again!
      return;
    }
    Serial.print("Enrolling ID # -1 ");
    Serial.println(id);
    while (!getFingerprintEnroll());
  }
}

uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #");
  Serial.println(id);

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
    case FINGERPRINT_PACKETRECIEVEERR:
    case FINGERPRINT_FEATUREFAIL:
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Error converting image");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(FINGERPRINT_IMAGE_DELAY);

  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }

  Serial.print("ID ");
  Serial.println(id);

  Serial.println("Place same finger again");
  while (!getFingerprintEnroll());
}

uint8_t getFingerprintEnroll() {


  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
    case FINGERPRINT_PACKETRECIEVEERR:
    case FINGERPRINT_FEATUREFAIL:
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Error converting image");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.print("Creating model for #");
  Serial.println(id);
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Car Unlocked and Ignition Started!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Incorrect key!!");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Authorization  error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Undefined error");
    return p;
  }
}



