/**
 * SensirionSHT is an Arduino library that provides easy, non-blocking access to
 * Sensirion SHT1x temperature and humidity sensors (and compatible).
 *
 * @author sekdiy (https://github.com/sekdiy/SensirionSHT)
 * @date 02.07.2015
 * @version See git comments for changes.
 */

#ifndef SENSIRIONSHT_H
#define SENSIRIONSHT_H

#include <math.h>
#include <Sensirion.h>                  // http://playground.arduino.cc/Code/Sensirion

class SensirionSHT: public Sensirion {
  public:

    SensirionSHT(unsigned int dataPin,  //!< Arduino pin that the sensor's data line is connected to.
                 unsigned int clockPin, //!< Arduino pin that the sensor's clock line is connected to.
                 float period = 3.0f    //!< Measurement period (in seconds), since the sensor needs time to aquire a value.
                );                      //!< Initializes instance attributes, enforcing a lower limit to the period duration.

    bool hasTemperature();              //!< Checks for errors or invalid temperature measurement value. @return True iff a valid temperature result is available
    bool hasHumidity();                 //!< Checks for errors or invalid humidity measurement value. @return True iff a valid humidity result is available
    bool hasDewpoint();                 //!< Checks for errors or invalid dew point measurement value. @return True iff a valid dew point result is available
    bool isOutdated();                  //!< Checks for errors or outdated results. @return False iff a valid new measurement result arrived within the most recent tick period.

    float getTemperature();             //!< Returns the current temperature value, only sensible if hasTemperature() returns true.
    float getHumidity();                //!< Returns the current humidity value, only sensible if hasHumidity() returns true.
    float getDewpoint();                //!< Returns the current dew point value, only sensible if hasTemperature() and hasHumidity() return true.

    unsigned int getError();            //!< Returns the error from the last measurement (according to the definitions in the Sensirion library).

    /** The tick() method does the actual work.
     * A measurement cycle consists of an initial temperature measurement and a followup humidity measurement.
     * For timing and accuracy information please see the documentation of your sensor.
     *
     * Once a period is completed, a new measurement cycle is started.
     * As soon as a measurement result is available, the temperature value is stored and the humidity value is requested.
     * Once the humidity value becomes available and has been stored too, the values are marked as fresh and we're ready for the next cycle.
     *
     * Call this method periodically and always provide the tick duration (time elapsed since the last call).
     *
     * @param   duration    The duration of the current tick period (in seconds).
     * @return  An error value according to the definitions in the Sensirion library (i.e. 0 on succes).
     */
    unsigned int tick(float duration);

  protected:
    bool _fresh = false;                //!< Are fresh results in?
    bool _ready = false;                //!< Is measurement currently running?

    float _period;                      //!< Waiting period inbetween measurements
    float _duration;                    //!< Time waited during current period
    float _temperature = NAN;           //!< Calculated temperature
    float _humidity = NAN;              //!< Calculated humidity

    unsigned int _mode;                 //!< Measurement mode (TEMP or HUMI)
    unsigned int _data;                 //!< Raw measurement result
    unsigned int _error;                //!< Most recent error
};

#endif  // SENSIRIONSHT_H
