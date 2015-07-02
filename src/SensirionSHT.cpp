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
 * Constructor
 *
 * @param unsigned int dataPin
 * @param unsigned int clockPin
 * @param float period
 */
SensirionSHT::SensirionSHT(unsigned int dataPin, unsigned int clockPin, float period)
  : Sensirion(dataPin, clockPin), _duration(period) {
  this->_period = (period >= 3.0f ? period : 3.0f);             // from experience, shorter periods can be problematic
}

/**
 * hasTemperature
 *
 * Checks for errors or invalid measurement value,
 * returns true iff a valid temperature result is available.
 *
 * @return bool
 */
bool SensirionSHT::hasTemperature() {
  return !(this->_error || isnan(this->_temperature));          // no error and a valid temperature?
}

/**
 * getTemperature
 *
 * Returns the current temperature value,
 * only sensible if hasTemperature() returns true.
 *
 * @return float
 */
float SensirionSHT::getTemperature() {
  return this->_temperature;                                    // return last temperature value
}

/**
 * hasHumidity
 *
 * Checks for errors or invalid measurement value,
 * returns true iff a valid humidity result is available.
 *
 * @return bool
 */
bool SensirionSHT::hasHumidity() {
  return !(this->_error || isnan(this->_humidity));             // no error and a valid humidity?
}

/**
 * getHumidity
 *
 * Returns the current humidity value,
 * only sensible if hasHumidity() returns true.
 *
 * @return float
 */
float SensirionSHT::getHumidity() {
  return this->_humidity;                                       // return last humidity value
}

/**
 * hasDewpoint
 *
 * Checks for errors or invalid measurement values,
 * returns true iff a valid dew point result is available.
 *
 * @return bool
 */
bool SensirionSHT::hasDewpoint() {
  return this->hasTemperature() && this->hasHumidity();         // no error and valid temperature and humidity?
}

/**
 * getDewpoint
 *
 * Calculates the dew point from the current temperature and humidity values,
 * only sensible if hasDewpoint() returns true.
 *
 * @return float
 */
float SensirionSHT::getDewpoint() {
  return this->calcDewpoint(this->_humidity, this->_temperature);   // return last dew point value
}

/**
 * isOutdated
 *
 * Checks for errors or outdated results,
 * returns false iff a valid new measurement result arrived within the most recent tick period.
 *
 * @return bool
 */
bool SensirionSHT::isOutdated() {
  return !(this->_error || this->_fresh);                       // return iff results aren't fresh any more
}

/**
 * tick
 *
 * This method does the actual work.
 *
 * Periodically the tick duration counter is updated.
 * Once a period is completed, a new measurement cycle is started.
 * A measurement cycle consists of an initial temperature measurement and a followup humidity measurement.
 * For timing and accuracy information please see the documentation of your sensor.
 *
 * As soon as a measurement result is available, the temperature value is stored and the humidity value is requested.
 * Once the humidity value becomes available and has been stored too, the values are marked as 'fresh' and we're 'ready' for the next cycle.
 *
 * Returns zero on succesfull execution or an error value according to the definitions in the Sensirion library.
 *
 * @param float duration
 * @return unsigned int
 */
unsigned int SensirionSHT::tick(float duration) {
  this->_duration += duration;                                  // update duration inbetween measurements
  this->_fresh = false;                                         // mark previous results as outdated

  if (this->_period - this->_duration <= 0.01) {                // has the waiting duration reached the period length?
    this->_ready = false;                                       // mark measurement cycle as started
    this->_mode = TEMP;                                         // switch to temperature
    this->_error = this->meas(this->_mode, &this->_data, NONBLOCK); // initiate measurement (non-blocking)
    this->_duration = 0.0f;                                     // reset duration for next waiting period
  }

  if ((!this->_ready) && (S_Meas_Rdy == this->measRdy())) {     // if measurement is running and sensor data becomes available
    switch (this->_mode)                                        // only one kind of value can be transmitted at a given time
    {
      case TEMP:                                                // temperature measurement:
        this->_temperature = this->calcTemp(this->_data);       // store temperature value
        this->_mode = HUMI;                                     // switch to humidity
        this->_error = this->meas(this->_mode, &this->_data, NONBLOCK); // initiate followup measurement (non-blocking)
        break;
      case HUMI:                                                // humidity measurement:
        this->_humidity = this->calcHumi(this->_data, this->_temperature); // store humidity value
        this->_ready = true;                                    // mark measurement cycle as completed
        this->_fresh = true;                                    // mark new results as fresh
        break;
    }
  }

  return this->_error;                                          // pass on possible sensor communication errors
}

/**
 * getError
 *
 * Returns the last error state (according to the definitions in the Sensirion library).
 *
 * @return bool
 */
unsigned int SensirionSHT::getError() {
    return this->_error;                                        // return most recent error
}
