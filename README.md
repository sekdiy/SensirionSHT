# SensirionSHT

## Purpose ##

**SensirionSHT** is an Arduino library that provides easy,  non-blocking access to [Sensirion SHT1x](http://www.sensirion.com/en/products/humidity-temperature) temperature and humidity sensors (and compatible).

## Simple Example ##

**SensirionSHT** works completely non-blocking:

```c++
void loop() {
  // update measurement
  Sensor.tick(delayTimeInSeconds);

  // new values should occur every three seconds
  Serial.println(Sensor.getTemperature());

  //
  // any other code can go here
  //

  // delay expects milliseconds
  delay(delayTimeInSeconds * 1000);
}
```

In the above example, an object `Sensor` is assumed.
This `Sensor` is updated periodically (every `delayTimeInSeconds`). The calls to `Sensor` are completely non-blocking, i.e. they return immediately.

See the [**Simple**](examples/Simple/Simple.ino) example (included with this library) for the complete code.

## Multiple Sensors Example ##

You could even use multiple sensors (connected to their respective clock and data pins):

```c++
// create a couple of SensirionSHT instances
SensirionSHT Sensor1 = SensirionSHT(1, 2);
SensirionSHT Sensor2 = SensirionSHT(3, 4);
SensirionSHT Sensor3 = SensirionSHT(5, 6);
...
// update them all
Sensor1.tick(duration);
Sensor2.tick(duration);
Sensor3.tick(duration);
```

## *Synchronous* Example ##

The library provides a way to use sensors and other code *synchronously*. Here's another example:

```c++
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
      Serial.println("°C");
    }

    //
    // anything that should run periodically goes here
    //
  }

  //
  // anything else goes here
  //

}
```

Each of the calls to `Sensor` is actually checking for arrival of a [signal](https://en.wikipedia.org/wiki/Reactive_system), keeping the program from [spinning](https://en.wikipedia.org/wiki/Busy_waiting) while waiting for the sensor (as the SHT series can take a couple hundred milliseconds per measurement).

See the [**Synchronous**](examples/Synchronous/Synchronous.ino) example (included with this library) and also the [**IrrigationController**](https://github.com/sekdiy/IrrigationController) project (where this library is being used with a scheduler, timers, interrupt handlers, real-time clock, etc.) for more.

## Installation ##

Just check out the [**SensirionSHT**](https://github.com/sekdiy/SensirionSHT) Repository on GitHub (or download the ZIP archive) and copy it to your Arduino's library folder (usually a folder called 'libraries' within your Arduino sketchbook).

Now do the same with the [**Sensirion**](http://playground.arduino.cc/Code/Sensirion) library by Markus Schatzl and Carl Jackson, since **SensirionSHT** extends (and thus requires) it.

After (re)launching the Arduino IDE, **SensirionSHT** (as well as **Sensirion**) will appear in your Sketchbook/Examples menu.

Alternatively you can use Arduino's `Add .ZIP Library...` menu option.

## Documentation ##

There's a copy of the [SHT1x datasheet](http://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/Humidity/Sensirion_Humidity_SHT1x_Datasheet_V5.pdf) from Sensirion in the folder `doc/`.

Additionally, the library contains a folder `doc/doxygen/` for  *html* documentation generated via [Doxygen](https://github.com/doxygen/doxygen) with some adaptions by [Velron](https://github.com/Velron/doxygen-bootstrapped).

In order to generate this documentation yourself, simply execute `doxygen doc/doxygen/Doxyfile` from within the `SensirionSHT/` folder.
