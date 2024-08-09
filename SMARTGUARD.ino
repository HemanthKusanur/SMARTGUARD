#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// WiFi credentials
const char* ssid = "Rog";
const char* password = "Qwerty@12345";

String gps = "https://www.google.com/maps/@13.1704419,77.5343526,17z?entry=ttu";

// Telegram credentials
#define BOTtoken "7430910794:AAHXdf4lB0UeX5RExzewt2JFO7CYN2CvQY4"
#define CHAT_ID "1609769681"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
// Pin definitions
const int LED1 = 13;         // Built-in LED pin
const int BUZZER_PIN = 12;   // GPIO12 for the buzzer
const int TRIGGER_PIN = 14;  // GPIO14 (D5) for ultrasonic trigger
const int ECHO_PIN = 15;     // GPIO15 (D8) for ultrasonic echo
const int SERVO_PIN = 2;     // GPIO2 (D4) for the servo

Adafruit_MPU6050 mpu;
Servo servo;

//SoftwareSerial gpsSerial(3, 1); // RX, TX for GPS

void setup() {
  Serial.begin(115200);
  //gpsSerial.begin(9600);
  
  Wire.begin(4, 5); // SDA, SCL for MPU6050
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  pinMode(LED1, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servo.attach(SERVO_PIN);
  
  
  configTime(0, 0, "pool.ntp.org");
  client.setTrustAnchors(&cert);

  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Test SSL connection to Telegram server
  Serial.print("Connecting to Telegram server...");
  if (!client.connect("api.telegram.org", 443)) {
    Serial.println("Failed to connect to Telegram server.");
    return;
  } else {
    Serial.println("Connected to Telegram server.");
  }

  // Test sending a message to verify the bot setup
  Serial.println("Testing Telegram Bot...");
  bool sent = bot.sendMessage(CHAT_ID, "System has Started!!", "");
  if (sent) {
    Serial.println("Initial message sent successfully!");
  } else {
    Serial.println("Failed to send the initial message. Check bot token and chat ID.");
  }
}

void moveToAngle(int angle) {
  servo.write(angle); // Move the servo to the specified angle
  delay(1000); // Wait for the servo to reach the position
}

long readUltrasonicDistance(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // Distance in cm

  return distance;
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  moveToAngle(0);

  Serial.print("Acceleration X: "); Serial.print(a.acceleration.x); Serial.print(", ");
  Serial.print("Y: "); Serial.print(a.acceleration.y); Serial.print(", ");
  Serial.print("Z: "); Serial.println(a.acceleration.z);

  Serial.print("Gyroscope X: "); Serial.print(g.gyro.x); Serial.print(", ");
  Serial.print("Y: "); Serial.print(g.gyro.y); Serial.print(", ");
  Serial.print("Z: "); Serial.println(g.gyro.z);

  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" *C");
  
  // Calculate the acceleration magnitude
  float acceleration = sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z);

  // Print acceleration magnitude
  Serial.print("Acceleration Magnitude: "); Serial.println(acceleration);

  // Read distance from ultrasonic sensor
  long distance = readUltrasonicDistance(TRIGGER_PIN, ECHO_PIN);

Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
  lcd.setCursor(0, 0);
  lcd.print("Vehicle :");
  lcd.setCursor(10, 0);
  lcd.print(distance);
  lcd.setCursor(0, 1);
  lcd.println("Happy Journey");

  if (distance >= 20 && distance <= 30) {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.println("Vehicle Ahead");
    lcd.setCursor(0, 1);
    lcd.println("  Be Careful!");
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(1000);
    lcd.clear();
  }
    if (distance >= 10 && distance <= 20) {
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.println("    Alert ");
    lcd.setCursor(0, 1);
    lcd.println("Applying Brakes!");
    moveToAngle(180);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(1000);
    lcd.clear();
  }

  // Check for a sudden jerk
  if (acceleration >= 11 && acceleration <= 40) { // Adjust the threshold as needed
    // Check if the distance is less than 10 cm
    if (distance > 0 && distance < 10) {
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.println("Accident Alert");
      lcd.setCursor(0, 1);
      lcd.println("Sending SOS!");
      moveToAngle(180);

      digitalWrite(LED1, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(5000);
      digitalWrite(LED1, LOW);
      digitalWrite(BUZZER_PIN, LOW);

      // Send Telegram message
      String message = "Accident Alert!\n";
      message += "Acceleration: " + String(acceleration) + "\n";
      message += "Distance: " + String(distance) + " cm\n";
      message += "Location: " + String(gps) + "\n";
      bool sent = bot.sendMessage(CHAT_ID, message, "");
      if (sent) {
        Serial.println("Alert message sent successfully!");
      } else {
        Serial.println("Failed to send the alert message. Check bot token and chat ID.");
      }
    }
  }

  // Optional: Blink the LED to indicate the system is running
  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED1, LOW);
  delay(500);
}