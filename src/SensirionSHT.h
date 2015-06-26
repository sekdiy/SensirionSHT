#include <Sensirion.h>                // http://playground.arduino.cc/Code/Sensirion

class SensirionSHT: public Sensirion 
{
  public:
    SensirionSHT(unsigned int dataPin, unsigned int clockPin, float period = 5.0f);

    float getTemperature();
    float getHumidity();
    float getDewpoint();

    void tick(float duration);
    
  protected:
    float _period;
    float _duration = 0.0f;
    bool _active = false;
    unsigned int _mode = TEMP;
    unsigned int _data = 0;

    float _temperature = 1e-99f;
    float _humidity = 1e-99f;
    float _dewpoint = 1e-99f;
};


