/*
 * Sensirion SHT library
 *
 * Author: sekdiy (https://github.com/sekdiy/Relay)
 */

#if ARDUINO > 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Sensirion.h>                                          // http://playground.arduino.cc/Code/Sensirion
#include <SensirionSHT.h>

SensirionSHT::SensirionSHT(unsigned int dataPin, unsigned int clockPin, float period)
  : Sensirion(dataPin, clockPin),                               //!< call parent constructor
    _duration(period)                                           //!< start at the beginning of a period
{
  this->_period = (period >= 3.0f ? period : 3.0f);             //!< from experience, shorter periods can be problematic
}

bool SensirionSHT::hasTemperature() {
  return !(this->_error || isnan(this->_temperature));          //!< no error and a valid temperature?
}

float SensirionSHT::getTemperature() {
  return this->_temperature;                                    //!< return last temperature value
}

bool SensirionSHT::hasHumidity() {
  return !(this->_error || isnan(this->_humidity));             //!< no error and a valid humidity?
}

float SensirionSHT::getHumidity() {
  return this->_humidity;                                       //!< return last humidity value
}

bool SensirionSHT::hasDewpoint() {
  return this->hasTemperature() && this->hasHumidity();         //!< no error and valid temperature and humidity?
}

float SensirionSHT::getDewpoint() {
  return this->calcDewpoint(this->_humidity, this->_temperature);   //!< return last dew point value
}

bool SensirionSHT::isOutdated() {
  return (this->_error || !this->_fresh);                       //!< error or results aren't fresh any more?
}

unsigned int SensirionSHT::tick(float duration) {
  this->_duration += duration;                                  //!< update duration inbetween measurements
  this->_fresh = false;                                         //!< mark previous results as outdated

  if (this->_period - this->_duration <= 0.01) {                //!< has the waiting duration reached the period length?
    this->_ready = false;                                       //!< mark measurement cycle as started
    this->_mode = TEMP;                                         //!< switch to temperature
    this->_error = this->meas(this->_mode, &this->_data, NONBLOCK); //!< initiate measurement (non-blocking)
    this->_duration = 0.0f;                                     //!< reset duration for next waiting period
  }

  if ((!this->_ready) && (S_Meas_Rdy == this->measRdy())) {     //!< if measurement is running and sensor data becomes available
    switch (this->_mode)                                        //!< only one kind of value can be transmitted at a given time
    {
      case TEMP:                                                //!< temperature measurement:
        this->_temperature = this->calcTemp(this->_data);       //!< store temperature value
        this->_mode = HUMI;                                     //!< switch to humidity
        this->_error = this->meas(this->_mode, &this->_data, NONBLOCK); //!< initiate followup measurement (non-blocking)
        break;
      case HUMI:                                                //!< humidity measurement:
        this->_humidity = this->calcHumi(this->_data, this->_temperature); //!< store humidity value
        this->_ready = true;                                    //!< mark measurement cycle as completed
        this->_fresh = true;                                    //!< mark new results as fresh
        break;
    }
  }

  return this->_error;                                          //!< pass on possible sensor communication errors
}

unsigned int SensirionSHT::getError() {
      return this->_error;                                        //!< return most recent error
}
