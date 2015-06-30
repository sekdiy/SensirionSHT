/**
 * Sensirion SHT library
 *
 * @author sekdiy
 * @date 01.07.2015
 * @version 2
 */

// Compatibility with the Arduino 1.0 library standard
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <SensirionSHT.h>

/**
 *
 */
SensirionSHT::SensirionSHT(unsigned int dataPin, unsigned int clockPin, float period)
  : Sensirion(dataPin, clockPin), _period(period), _duration(period) {
}

bool SensirionSHT::hasTemperature() {
  return !isnan(this->_temperature);
}

float SensirionSHT::getTemperature() {
  return this->_temperature;                                    // return last temperature value
}

bool SensirionSHT::hasHumidity() {
  return !isnan(this->_humidity);
}

float SensirionSHT::getHumidity() {
  return this->_humidity;                                       // return last humidity value
}

bool SensirionSHT::hasDewpoint() {
  return this->hasTemperature() && this->hasHumidity();
}

float SensirionSHT::getDewpoint() {
  return this->calcDewpoint(this->_humidity, this->_temperature);   // return last dew point value
}

bool SensirionSHT::isOutdated() {
  return !this->_fresh;                                         // return iff results aren't fresh any more
}

unsigned int SensirionSHT::tick(float duration) {
  unsigned int error = 0;

  this->_duration += duration;                                  // update duration inbetween measurements
  this->_fresh = false;                                         // mark previous results as outdated

  if (this->_period - this->_duration < 0.01) {                 // has the waiting duration reached the period length?
    this->_ready = false;                                       // mark measurement cycle as started
    this->_mode = TEMP;                                         // switch to temperature for next measurement
    error = this->meas(this->_mode, &this->_data, NONBLOCK);    // initiate a new measurement
    this->_duration = 0.0f;                                     // reset duration for next waiting period
  }

  if ((!this->_ready) && (S_Meas_Rdy == this->measRdy()))       // if measurement is running and sensor data becomes available
  {
    switch (this->_mode)                                        // only one kind of value can be transmitted at a given time
    {
      case TEMP:                                                // temperature measurement:
        this->_temperature = this->calcTemp(this->_data);       // store temperature value
        this->_mode = HUMI;                                     // switch to humidity for next measurement
        error = this->meas(this->_mode, &this->_data, NONBLOCK);// initiate a new measurement
        break;
      case HUMI:                                                // humidity measurement
        this->_humidity = this->calcHumi(this->_data, this->_temperature);            // store humidity value
        this->_ready = true;                                    // mark measurement cycle as completed
        this->_fresh = true;                                    // mark new results as fresh
        break;
    }
  }

  return error;
}
