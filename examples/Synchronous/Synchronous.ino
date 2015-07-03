#include <Sensirion.h>        // http://playground.arduino.cc/Code/Sensirion
#include <SensirionSHT.h>     // https://github.com/sekdiy/SensirionSHT

// create an instance of SensirionSHT on pins 4 and 5
SensirionSHT Sensor = SensirionSHT(4, 5);

// one second delay between updates
float period = 1.0f;
float lastTime = 0.0f;

void setup() {
  // setup serial communication
  Serial.begin(9600);
}

void loop() {
  // track current time
  float currentTime = millis() / 1000.0f;
  float duration = currentTime - lastTime;

  // wait a whole period
  if (duration >= period) {

    // prepare for next cycle
    lastTime = currentTime;

    // update measurement
    Sensor.tick(duration);

    // check for valid temperature data
    if (Sensor.hasTemperature()) {
      Serial.print("current temperature: ");
      Serial.print(Sensor.getTemperature());
      Serial.println("°C");
    }

    // check for valid humidity data
    if (Sensor.hasHumidity()) {
      Serial.print("current humidity: ");
      Serial.print(Sensor.getHumidity());
      Serial.println("%");
    }

    // check for valid dew point data
    if (Sensor.hasDewpoint()) {
      Serial.print("current dew point: ");
      Serial.print(Sensor.getDewpoint());
      Serial.println("°C");
    }

    // new values should arrive every three seconds
    if (!Sensor.isOutdated()) {
      Serial.println("Results are fresh! :)");
    }

    /*
     * anything that should run every period can go here
     */
  }

  /*
   * anything that should always run can go here
   */

}
