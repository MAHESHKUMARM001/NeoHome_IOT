#include <NeoHome.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Set up GPS Serial connection
HardwareSerial mySerial(1);
TinyGPSPlus gps;

double latitude, longitude;

NeoHome neohome;
const char* deviceId = "NECZUJYfR1f5lO2rMIgl";
const char* wifiSSID = "HASHAN";
const char* wifiPassword = "hashanprojects";
const int ledPin = 2; // LED connected to GPIO 2
const int dhtPin = 4; // DHT11 connected to GPIO 4
DHT dht(dhtPin, DHT11);

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT); // Set LED pin as output
    digitalWrite(ledPin, LOW); // Ensure LED is off initially
    dht.begin();
    mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX pin = 16, TX pin = 17
    neohome.begin(wifiSSID, wifiPassword, deviceId);
}

void loop() {
    // Read value from virtual pin 4
    // String value = neohome.read(deviceId, 4);
    // Serial.println("Read value: " + value);

    // // Control LED based on read value
    // if (value == "1") {
    //     digitalWrite(ledPin, HIGH); // Turn LED on
    //     Serial.println("LED ON");
    // } else if (value == "0") {
    //     digitalWrite(ledPin, LOW); // Turn LED off
    //     Serial.println("LED OFF");
    // }

    // Read temperature from DHT11
    // float temperature = dht.readTemperature();
    // if (isnan(temperature)) {
    //     Serial.println("Failed to read temperature from DHT11");
    // } else {
    //     // Convert temperature to String with 1 decimal place
    //     String tempString = String(temperature, 2);
    //     neohome.write(deviceId, 1, tempString);
    //     neohome.write(deviceId, 2, "Temp "+tempString+"\nLine 6\nLine 7\nLine 8");
    //     // neohome.write(deviceId, 5, "9.1778,77.5351");
    //     Serial.println("Published+ temperature: " + tempString);
    // }

    while (mySerial.available() > 0) {
      if (gps.encode(mySerial.read())) {
        if (gps.location.isValid()) {
            latitude = gps.location.lat();
            longitude = gps.location.lng();
            String slatitute = String(latitude, 6);
            String slongitude = String(longitude, 6);
            Serial.print("Latitude: ");
            Serial.print(latitude, 6);
            Serial.print(", Longitude: ");
            Serial.println(longitude, 6);
            neohome.write(deviceId, 5, slatitute+","+slongitude);
        }
      }
    }

    // neohome.write(deviceId, 2, "Temp "+tempString+"\nLine 6\nLine 7\nLine 8");

    delay(100); // Short delay to avoid overloading CPU
}



