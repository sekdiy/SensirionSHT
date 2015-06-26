// Compatibility with the Arduino 1.0 library standard
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "SensirionSHT.h"

SensirionSHT::SensirionSHT(unsigned int dataPin, unsigned int clockPin, float period): Sensirion(dataPin, clockPin) {
  this->_period = period;
  this->_duration = period;
}

float SensirionSHT::getTemperature() {
  return this->_temperature;
}

float SensirionSHT::getHumidity() {
  return this->_humidity;
}

float SensirionSHT::getDewpoint() {
  return this->_dewpoint;
}

void SensirionSHT::tick(float duration) {
  this->_duration += duration;

  if (this->_period - this->_duration < 0.01) {
    this->_duration = 0;
    this->_active = false;     
  }

  if (!this->_active) {
    this->meas(this->_mode, &this->_data, NONBLOCK);
    this->_active = true;
  } 
  else if (this->measRdy()) {
    switch (this->_mode) {
      case TEMP:
          this->_temperature = this->calcTemp(this->_data);
          this->_mode = HUMI;
        break;
      case HUMI:
          this->_humidity = this->calcHumi(this->_data, this->_temperature);
          this->_dewpoint = this->calcDewpoint(this->_humidity, this->_temperature);
          this->_mode = TEMP;
        break;
    }
  }  
}

