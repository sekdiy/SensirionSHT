/**
 * Sensirion SHT library
 *
 * @author sekdiy
 * @date 02.07.2015
 * @version 3
 */

#ifndef SENSIRIONSHT_H
#define SENSIRIONSHT_H

#include <Sensirion.h>                  // http://playground.arduino.cc/Code/Sensirion

/**
 * SensirionSHT
 *
 * Provides non-blocking access to Sensirion SHT series temperature/humidity sensors (SHT1x and compatible).
 *
 * An SHT sensor connected to 'dataPin' and 'clockPin' (see sensor documentation) is polled every 'period' (default: 3s).
 *
 * The dedicated method 'tick(float duration)' has to be called periodically (e.g. every second) in order to update the values.
 */
class SensirionSHT: public Sensirion
{
  public:
    SensirionSHT(unsigned int dataPin, unsigned int clockPin, float period = 3.0f);

    bool hasTemperature();              // check for succesful temperature measurement cycle
    bool hasHumidity();                 // check for succesful humidity measurement cycle
    bool hasDewpoint();                 // check for succesful temperature and humidity measurement cycles
    bool isOutdated();                  // check if last measurement cycle completed more than one tick ago

    float getTemperature();             // get calculated temperature
    float getHumidity();                // get calculated humidity
    float getDewpoint();                // get calculated dew point

    unsigned int tick(float duration);  // update measurement
    unsigned int getError();            // get most recent error

  protected:
    bool _fresh = false;                // are fresh results in?
    bool _ready = false;                // is measurement currently running?

    float _period;                      // waiting period inbetween measurements
    float _duration;                    // time waited during current period
    float _temperature = NAN;           // calculated temperature
    float _humidity = NAN;              // calculated humidity

    unsigned int _mode;                 // measurement mode (TEMP or HUMI)
    unsigned int _data;                 // raw measurement result
    unsigned int _error;                // most recent error
};

#endif  // SENSIRIONSHT_H
