/*
  Nano 33 BLE - Light Sleep with Button Wake + Temp/Humidity Sensor
  
  This code demonstrates light sleep with external interrupt wake:
  - Wakes up and turns GREEN LED on for 2 seconds
  - Reads temperature and humidity every 100ms while awake
  - Goes to LIGHT SLEEP (LED off) until button is pressed
  - Button on D2 wakes the board
  - Repeats indefinitely
  
  Hardware: 
  - Arduino Nano 33 BLE
  - Button connected to D2 and GND (internal pullup used)
  - Onboard HTS221 temperature & humidity sensor
  
  Libraries Required (install via Library Manager):
  - Arduino_HTS221
  
  Note: This uses LIGHT SLEEP (not deep sleep/SYSTEMOFF) which:
  - Maintains RAM and continues from where it left off
  - Uses less power than fully awake but more than SYSTEMOFF
  - Wakes quickly and reliably on button press
  - Perfect for student demonstrations
  
  DEMO TEACHING POINT:
  =====================
  When viewing Serial Plotter, students will notice:
  - Temperature and humidity readings appear only during the 2-second AWAKE period
  - NO readings during sleep (even if you breathe on the sensor)
  - Button instantly wakes the board (no reset)
  - This demonstrates that peripherals are powered down during sleep
  - Power savings come at the cost of sensor functionality
  
  Try this: Breathe on the board during sleep - nothing happens!
            Press button to wake, then breathe on sensor - temp/humidity respond!
*/

#include <mbed.h>
#include <Arduino_HTS221.h>

using namespace mbed;
using namespace rtos;

// RGB LED pins on Nano 33 BLE (Active LOW - LOW=on, HIGH=off)
#define LED_RED    22
#define LED_GREEN  23
#define LED_BLUE   24

// Button pin
#define BUTTON_PIN 2

// Timing constants
#define AWAKE_TIME 2000    // 2 seconds awake
#define SAMPLE_RATE 100    // Read sensor every 100ms

// Wake flag
volatile bool wokenUp = false;

// Interrupt handler function
void buttonPressed() {
  wokenUp = true;
}

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial && millis() < 3000); // Wait for serial or timeout after 3s
  
  // Configure LED pins as outputs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  // Turn off all LEDs initially (HIGH = off for this board)
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  
  // Configure button pin with internal pullup and attach interrupt
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressed, FALLING);
  
  // Initialize temperature & humidity sensor
  if (!HTS.begin()) {
    Serial.println("Error initializing HTS221 sensor!");
    while (1); // Halt if sensor fails
  }
  
  Serial.println("Light Sleep Button Wake + Temp/Humidity Demo (mbed native)");
  Serial.println("Press button on D2 to wake from sleep");
  Serial.println("Open Serial Plotter to see temperature and humidity data");
  Serial.println("------------------------------------------------------------");
  delay(2000); // Give time to open Serial Plotter
}

void loop() {
  // ===== AWAKE PERIOD =====
  Serial.println("Awake");
  wokenUp = false; // Reset wake flag
  
  // Turn GREEN LED on (LOW = on)
  digitalWrite(LED_GREEN, LOW);
  
  // Read temp & humidity sensor multiple times while awake
  unsigned long awakeStart = millis();
  while (millis() - awakeStart < AWAKE_TIME) {
    
    // Read temperature and humidity
    float temperature = HTS.readTemperature();
    float humidity = HTS.readHumidity();
    
    // Format for Serial Plotter (label:value label:value)
    Serial.print("Temperature:");
    Serial.print(temperature);
    Serial.print(",Humidity:");
    Serial.println(humidity);
    
    delay(SAMPLE_RATE);
  }
  
  // ===== SLEEP PERIOD =====
  Serial.println("Going to sleep - sensor will NOT respond!");
  Serial.flush(); // Ensure message is sent before sleeping
  
  // Turn GREEN LED off (HIGH = off)
  digitalWrite(LED_GREEN, HIGH);
  
  // Enter light sleep - will wake on button interrupt
  // Using mbed sleep function with interrupt wake capability
  sleep();
  
  // After wake, wait for button to be released and debounce
  while (digitalRead(BUTTON_PIN) == LOW) {
    delay(10);
  }
  delay(100);
  
  Serial.println("Woken up by button press!");
}
