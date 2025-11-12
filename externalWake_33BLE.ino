/*
  Nano 33 BLE - Deep Sleep with Button Wake + Proximity Sensor (mbed native)
  
  This code demonstrates deep sleep with external interrupt wake:
  - Wakes up and turns GREEN LED on for 2 seconds
  - Reads proximity sensor every 100ms while awake
  - Goes to DEEP SLEEP (LED off) until button is pressed
  - Button on D2 wakes the board
  - Repeats indefinitely
  
  Hardware: 
  - Arduino Nano 33 BLE
  - Button connected to D2 and GND (internal pullup used)
  - Onboard APDS9960 proximity sensor
  
  Libraries Required (install via Library Manager):
  - Arduino_APDS9960
  
  Note: Uses built-in mbed sleep functions instead of ArduinoLowPower library
  
  DEMO TEACHING POINT:
  =====================
  When viewing Serial Plotter, students will notice:
  - Proximity readings appear only during the 2-second AWAKE period
  - NO readings during deep sleep (even if you wave your hand)
  - This demonstrates that peripherals are powered down during sleep
  - Power savings come at the cost of sensor functionality
  - Real-world applications must balance power vs. responsiveness
  
  Try this: Wave your hand over the board during sleep - nothing happens!
            Press button to wake, then wave hand - sensor responds!
*/

#include <mbed.h>
#include <Arduino_APDS9960.h>

using namespace mbed;
using namespace rtos;

// RGB LED pins on Nano 33 BLE (Active LOW - LOW=on, HIGH=off)
// #define LED_RED    22
// #define LED_GREEN  23
// #define LED_BLUE   24

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
  
  // Initialize proximity sensor
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS9960 sensor!");
    while (1); // Halt if sensor fails
  }
  
  Serial.println("Deep Sleep Button Wake + Proximity Demo (mbed native)");
  Serial.println("Press button on D2 to wake from sleep");
  Serial.println("Open Serial Plotter to see proximity data");
  Serial.println("-------------------------------------------------------");
  delay(2000); // Give time to open Serial Plotter
}

void loop() {
  // ===== AWAKE PERIOD =====
  Serial.println("Awake");
  wokenUp = false; // Reset wake flag
  
  // Turn GREEN LED on (LOW = on)
  digitalWrite(LED_GREEN, LOW);
  
  // Read proximity sensor multiple times while awake
  unsigned long awakeStart = millis();
  while (millis() - awakeStart < AWAKE_TIME) {
    
    // Check if proximity data is available
    if (APDS.proximityAvailable()) {
      int proximity = APDS.readProximity();
      
      // Format for Serial Plotter (label:value)
      Serial.print("Proximity:");
      Serial.println(proximity);
    }
    
    delay(SAMPLE_RATE);
  }
  
  // ===== DEEP SLEEP PERIOD =====
  Serial.println("Going to sleep - sensor will NOT respond!");
  Serial.flush(); // Ensure message is sent before sleeping
  
  // Turn GREEN LED off (HIGH = off)
  digitalWrite(LED_GREEN, HIGH);
  
  // Enter deep sleep - will wake on button interrupt
  // Using mbed sleep function with interrupt wake capability
  sleep();
  
  // After wake, wait for button to be released and debounce
  while (digitalRead(BUTTON_PIN) == LOW) {
    delay(10);
  }
  delay(100);
  
  Serial.println("Woken up by button press!");
}