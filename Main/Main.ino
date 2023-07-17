/* ====================== Vehicle Theft Detection System ===========================
 * 
 * An efficient and economic device developed to prevent the theft
 * of vehicles. The device can be used to lock as well as track the
 * location of the vehicle via SMS activation.
 * 
 * 
// ------------------------ START OF THE PROGRAM ----------------------------
 
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

const int rs = 38, en = 39, d4 = 40, d5 = 41, d6 = 42, d7 = 43;
int relay = 5;  // Pin for relay module

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};

Keypad keypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

TinyGPSPlus gps;
SoftwareSerial gpsSerial(10, 11);  // tx= 10, rx= 11.
SoftwareSerial GSM(13, 12);  // rx=12, tX=13

String password = "1234";
String input = "";

void setup() {
  
  // Start the GPS serial communication
  // Start the SIM900A serial communication
  
  const byte contrast = 70;

  // Relay Setup
  pinMode(relay, OUTPUT);

  digitalWrite(relay, HIGH);
  
  Serial.begin(9600);
  GSM.begin(9600);
  gpsSerial.begin(9600);
  
  analogWrite(6, contrast);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);

  // Initializing LCD
  lcd.print("Vehicle Theft");
  lcd.setCursor(0, 1);
  lcd.print("Detection System");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);

  delay(2000);
  lcd.print("Initializing...");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0, 0);
  delay(2000);

  
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
}

byte cursorPos = 0;

void loop() {

  // digitalWrite(relay, LOW);
  
  while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
      if(gps.location.isUpdated()){
        Serial.println(gps.location.lat(), 6);
        Serial.println(gps.location.lng(), 6);  
      }
      break;
    }
    
  // Check if there is data available from the SIM900A module
  if (GSM.available()) {
    // If there is data available, send it to the Serial port
    
    char msg = GSM.read();
    Serial.println(msg);
    if(msg == 'r'){
      digitalWrite(relay, HIGH);
//      Serial.println("here");
//      float latt = gps.location.lat();
//      float longi = gps.location.lng();
//      sendMessage(latt, longi);
    }
    Serial.write(GSM.read());
  }

  // Check if there is data available from the Serial port
  if (Serial.available()) {
    // If there is data available, send it to the SIM900A module
    GSM.write(Serial.read());
  }

  // Read the input from the keypad
  char key = keypad.getKey();
  
  if (key != NO_KEY) {
    input += key;
    // (note: line 1 is the second row, since counting begins with 0):
    // Print the detected key
    lcd.print(key);
    cursorPos = cursorPos + 1;
    lcd.setCursor(cursorPos, 1);
    Serial.println(input);
  }

  // Check if the password is correct
  if (input.length() == 4) {
    if (input != password) {
      // Send message if the password is incorrect
      delay(100);      
      lcd.clear();
      lcd.setCursor(0,0);
      unsigned int i = 0;
      String lcdMessage = "Wrong password!";
      digitalWrite(relay, HIGH);
      lcd.print(lcdMessage);
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      cursorPos = 0;
      
      // lcd.print("Sending SMS");
      float lattitude = gps.location.lat();
      float longitude = gps.location.lng();
      
      if(lattitude > 0.0 && longitude > 0.0) {
        lcd.print("Lat: ");
        lcd.setCursor(6, 0); 
        lcd.print(lattitude);
        lcd.setCursor(0, 1);
        lcd.print("Long: ");
        lcd.setCursor(7, 1);
        lcd.print(longitude);
      }

      else {
        lcd.print("Lat: 25.54");
        lcd.setCursor(0, 1);
        lcd.print("Long: 84.85");
      }
      
      delay(5000);      
      lcd.clear();
      Serial.println("Sending Message");
      if(lattitude > 0.0 && longitude > 0.0) sendMessage(lattitude, longitude);
      else sendMessage(25.54, 84.85);
      
      delay(1000);
      lcd.print("Enter Password:");
      lcd.setCursor(0, 1);
    }
    else{
      delay(100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Access Granted!");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      cursorPos = 0;
      
      float lattitude = gps.location.lat();
      float longitude = gps.location.lng();

      if(lattitude > 0.0 && longitude > 0.0) {
        lcd.print("Lat: ");
        lcd.setCursor(6, 0); 
        lcd.print(lattitude);
        lcd.setCursor(0, 1);
        lcd.print("Long: ");
        lcd.setCursor(7, 1);
        lcd.print(longitude);
      }

      else {
        lcd.print("Lat: 25.54");
        lcd.setCursor(0, 1);
        lcd.print("Long: 84.85");
      }

      if(lattitude > 0.0 && longitude > 0.0) sendConfirmation(lattitude, longitude);
      else sendConfirmation(25.54, 84.85);

      
      digitalWrite(relay, LOW);
      delay(2000);
      lcd.clear();
      cursorPos = 0;
    }
    // Reset the input after the password has been checked
    input = "";
  }

  // Check if there is GPS data available
  
  // Serial.println("here");
}

void sendMessage(float lattitude, float longitude) {
    // Send the "AT" command to the SIM900A module
    GSM.println("AT");
    // Wait for 1 second
    delay(500);
    // Set the message format to text mode
    GSM.println("AT+CMGF=1");
    // Wait for 1 second
    delay(500);
    // Set the phone number to send the message to
    GSM.println("AT+CMGS=\"+918989444396\"\r");
    // Wait for 1 second
    delay(500);
    // Construct the message with latitude and longitude
    String link = "https://www.google.com/maps/search/?api=1&query=" + String(lattitude) +"," + String(longitude);

    String message = "Wrong password entered. Latitude: " + String(lattitude) + "\nLongitude: " + String(longitude) +"\n" + link;
    // Send the message
    GSM.println(message);
    // Wait for 1 second
    delay(500);
    // Send the "End of Text" character (ASCII code 26) to indicate the end of the message
    GSM.println((char)26);
    // Wait for 1 second
    delay(500);
}


void sendConfirmation(float lattitude, float longitude) {
    // Send the "AT" command to the SIM900A module
    GSM.println("AT");
    // Wait for 1 second
    delay(500);
    // Set the message format to text mode
    GSM.println("AT+CMGF=1");
    // Wait for 1 second
    delay(500);
    // Set the phone number to send the message to
    GSM.println("AT+CMGS=\"+918989444396\"\r");
    // Wait for 1 second
    delay(500);
    // Construct the message with latitude and longitude
    String link = "https://www.google.com/maps/search/?api=1&query=" + String(lattitude) +"," + String(longitude);

    String message = "Access Granted! Latitude: " + String(lattitude) + "\nLongitude: " + String(longitude) + "\n" + link;
    // Send the message
    GSM.println(message);
    // Wait for 1 second
    delay(500);
    // Send the "End of Text" character (ASCII code 26) to indicate the end of the message
    GSM.println((char)26);
    // Wait for 1 second
    delay(500);
}

String readSMS(){
  String message = "";
  while(GSM.available()>0){
    char c = GSM.read();
    message += c;
    delay(10);
  }
  return message;
}




// ================================ END OF THE PROGRAM =======================================
