/*
  AVR Arduino - Sleep with Button Wake Template
  
  This code demonstrates sleep with external interrupt wake:
  - Wakes up and turns LED on for 2 seconds
  - Goes to power-down sleep until button is pressed
  - Button on D2 (INT0) wakes the board
  - Repeats indefinitely
  
  Hardware: 
  - Arduino Nano, Uno, or any ATmega328P board
  - Button connected to D2 and GND (internal pullup used)
  
  Library Required: NONE - uses built-in AVR sleep functions
  
  Note: Only pins D2 (INT0) and D3 (INT1) can wake from sleep on AVR boards.
  
  DEMO TEACHING POINT:
  =====================
  When the board is asleep:
  - LED is off
  - Serial doesn't respond
  - Board consumes minimal power (~0.1mA vs ~20mA awake)
  - ONLY the button interrupt can wake it
  - This demonstrates true power-down sleep mode
  
  Try this: Press button to wake, watch LED turn on for 2 seconds,
            then board goes back to sleep waiting for next button press.
*/

#include <avr/sleep.h>
#include <avr/power.h>

// LED pin (built-in LED on most AVR boards)
#define LED_PIN 13

// Button pin (MUST be D2 or D3 for hardware interrupt on AVR)
#define BUTTON_PIN 2

// Timing constants
#define AWAKE_TIME 2000  // 2 seconds awake

// Wake flag
volatile bool wokenUp = false;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial && millis() < 3000); // Wait for serial or timeout
  
  // Configure LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Configure button pin with internal pullup
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  Serial.println("AVR Power-Down Sleep with Button Wake Demo");
  Serial.println("Press button on D2 to wake from sleep");
  Serial.println("-------------------------------------------");
  delay(1000);
}

void loop() {
  // ===== AWAKE PERIOD =====
  Serial.println("Awake");
  wokenUp = false;
  
  // Turn LED on
  digitalWrite(LED_PIN, HIGH);
  
  // Stay awake for 2 seconds
  delay(AWAKE_TIME);
  
  // ===== SLEEP PERIOD =====
  Serial.println("Going to sleep");
  Serial.println("Press button to wake...");
  Serial.flush(); // Ensure message is sent before sleeping
  
  // Turn LED off
  digitalWrite(LED_PIN, LOW);
  
  // Enter sleep mode - will wake on button press
  enterSleep();
  
  // After wake, debounce button
  delay(200);
  
  Serial.println("Woken up by button press!");
}

// Interrupt Service Routine for button press
void wakeUp() {
  // Detach interrupt to prevent multiple triggers during wake
  detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));
  wokenUp = true;
}

// Function to enter sleep mode
void enterSleep() {
  // Attach interrupt for button wake (FALLING = button press)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), wakeUp, FALLING);
  
  // Configure sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Deepest sleep mode
  sleep_enable();                        // Enable sleep mode
  
  // Disable peripherals to save more power
  power_adc_disable();    // ADC off
  power_spi_disable();    // SPI off
  power_timer0_disable(); // Timer0 off (WARNING: breaks millis/delay!)
  power_timer1_disable(); // Timer1 off
  power_timer2_disable(); // Timer2 off
  power_twi_disable();    // I2C/TWI off
  
  // Enter sleep mode
  sleep_mode();
  
  // ===== CPU SLEEPS HERE =====
  // Wakes up when button interrupt triggers
  
  // After wake, disable sleep and re-enable peripherals
  sleep_disable();
  
  // Re-enable peripherals
  power_all_enable();
}
