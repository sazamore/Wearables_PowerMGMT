/*
  AVR Arduino - Sleep with Timer Wake Template
  
  This code demonstrates a basic sleep/wake cycle:
  - Wakes up and turns LED on for 2 seconds
  - Goes to power-down sleep for ~5 seconds using Watchdog Timer
  - Repeats indefinitely
  
  Hardware: Arduino Nano, Uno, or any ATmega328P board
  Library Required: NONE - uses built-in AVR sleep functions
  
  Note: Watchdog timer has limited time options (8s max per cycle).
        For exact 5 seconds, we'll wake at 4s intervals.
*/

#include <avr/sleep.h>
#include <avr/wdt.h>

// LED pin (built-in LED on most AVR boards)
#define LED_PIN 13

// Timing constants
#define AWAKE_TIME 2000  // 2 seconds awake

// Watchdog interrupt flag
volatile bool watchdogFlag = false;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  while (!Serial && millis() < 3000); // Wait for serial or timeout
  
  // Configure LED pin as output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Disable watchdog on startup (in case of previous reset)
  MCUSR = 0;
  wdt_disable();
  
  Serial.println("AVR Power-Down Sleep Demo Started");
  Serial.println("----------------------------------");
}

void loop() {
  // ===== AWAKE PERIOD =====
  Serial.println("Awake");
  
  // Turn LED on
  digitalWrite(LED_PIN, HIGH);
  
  // Stay awake for 2 seconds
  delay(AWAKE_TIME);
  
  // ===== SLEEP PERIOD =====
  Serial.println("Going to sleep");
  Serial.flush(); // Ensure message is sent before sleeping
  
  // Turn LED off
  digitalWrite(LED_PIN, LOW);
  
  // Sleep for approximately 5 seconds using watchdog timer
  // (8s + 1s = closest we can get to 5s with WDT intervals)
  sleepWithWatchdog(WDTO_4S); // 4 seconds
  delay(10); // Small delay for stability
  sleepWithWatchdog(WDTO_1S); // 1 second (total ~5s)
  
  // After sleep, code continues here and loops back
}

// Watchdog Interrupt Service Routine
ISR(WDT_vect) {
  watchdogFlag = true; // Set flag when watchdog times out
}

// Function to enter sleep mode with watchdog timer wake
void sleepWithWatchdog(int timerPrescaler) {
  // Clear watchdog flag
  watchdogFlag = false;
  
  // Set up watchdog timer for interrupt (not reset)
  MCUSR &= ~(1 << WDRF);                     // Clear watchdog reset flag
  WDTCSR |= (1 << WDCE) | (1 << WDE);        // Enable watchdog change
  WDTCSR = (1 << WDIE) | timerPrescaler;     // Enable interrupt, set prescaler
  wdt_reset();                                // Reset watchdog timer
  
  // Configure sleep mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // Deepest sleep
  sleep_enable();                        // Enable sleep mode
  
  // Disable ADC to save power
  ADCSRA &= ~(1 << ADEN);
  
  // Enter sleep mode
  sleep_mode();
  
  // ===== CPU SLEEPS HERE =====
  // Wakes up when watchdog timer expires
  
  // After wake, disable sleep
  sleep_disable();
  
  // Re-enable ADC
  ADCSRA |= (1 << ADEN);
  
  // Disable watchdog
  wdt_disable();
}
