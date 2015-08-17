#include <Sensirion.h>        // http://playground.arduino.cc/Code/Sensirion
#include <SensirionSHT.h>     // https://github.com/sekdiy/SensirionSHT

// project constants
const float defaultValue = -999.99f;

// create an instance of SensirionSHT on pins 4 and 5 with 3 seconds measurement intervals
SensirionSHT Sensor = SensirionSHT(4, 5, 3);

// one second delay between updates
float period = 1.0f;
float lastTime = 0.0f;

void setup() {
  // setup serial communication
  Serial.begin(9600);
}

/**
 * In this example the scheduling period differs from the measurement period.
 *
 * While the synchronous block below is executed every second, Sensor is set to take
 * a new measurement every three seconds.
 * The fixed 'scheduler' (the loop function) waits for events from Sensor, then reacts.
 *
 * This is to demonstrate how a real world, timing-sensitive device like SensirionSHT
 * can be integrated into a reactive system.
 */
void loop() {
  // track current time
  float currentTime = millis() / 1000.0f;
  float duration = currentTime - lastTime;

  // synchronous block, executed every period
  if (duration >= period) {

    // prepare for next cycle
    lastTime = currentTime;

    // update measurement
    Sensor.tick(duration);

    // check for valid temperature data
    if (Sensor.hasTemperature()) {
      Serial.print("current temperature: ");
      Serial.print(Sensor.getTemperature());
      Serial.print("°C, ");
    }

    // check for valid humidity data
    if (Sensor.hasHumidity()) {
      Serial.print("current humidity: ");
      Serial.print(Sensor.getHumidity());
      Serial.print("%, ");
    }

    // check for valid dew point data
    if (Sensor.hasDewpoint()) {
      Serial.print("current dew point: ");
      Serial.print(Sensor.getDewpoint());
      Serial.print("°C, ");
    }

    // new values should arrive about every three seconds
    Serial.print("results are ");
    Serial.print(Sensor.isOutdated() ? "outdated" : "fresh");
    Serial.print(", error code: ");
    Serial.print(Sensor.getError());
    Serial.println(".");


    /*
     * anything that should run every period can go here
     */
  }

  /*
   * anything that should always run can go here
   */

}
