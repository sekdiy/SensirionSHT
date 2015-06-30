/**
 * Sensirion SHT library
 *
 * @author sekdiy
 * @date 01.07.2015
 * @version 2
 */

#ifndef SENSIRIONSHT_H
#define SENSIRIONSHT_H

#include <Sensirion.h>                // http://playground.arduino.cc/Code/Sensirion

class SensirionSHT: public Sensirion
{
  public:
    SensirionSHT(unsigned int dataPin, unsigned int clockPin, float period = 5.0f);

    bool hasTemperature();          // check for succesful temperature measurement cycle
    bool hasHumidity();             // check for succesful humidity measurement cycle
    bool hasDewpoint();             // check for succesful temperature and humidity measurement cycles
    bool isOutdated();              // check if last measurement cycle was longer than one tick ago

    float getTemperature();         // get calculated temperature
    float getHumidity();            // get calculated humidity
    float getDewpoint();            // get calculated dew point

    unsigned int tick(float duration);  // update measurement

  protected:
    float _period;                  // waiting period inbetween measurements
    float _duration;                // time waited during current period
    bool _fresh = false;            // measurement currently running
    bool _ready = false;            // updated calculations available
    unsigned int _mode;             // measurement mode (TEMP or HUMI)
    unsigned int _data;             // raw measurement result

    float _temperature = NAN;  // calculated temperature
    float _humidity = NAN;     // calculated humidity
};

#endif   // SENSIRIONSHT_H
