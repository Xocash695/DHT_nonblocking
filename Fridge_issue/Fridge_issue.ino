#include <dht_nonblocking.h>

//www.elegoo.com
//2018.10.25


#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );



/*
 * Initialize the serial port.
 */
void setup( )
{
  Serial.begin( 9600);
  Serial.print(" This device is aimed toward fixing defrost issues with fridges");
  Serial.print("Please Refer to https://github.com/Xocash695/Fridge-Defrost.git for more info");

}



/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}



/*
 * Main program loop.
 */
void loop( )
{
  float temperature;
  float humidity;
  float setlow = 5.00;
  float sethigh = 10.00;
  pinMode(12, OUTPUT);

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity ) == true )
  { 
    if (temperature <= setlow) {
     digitalWrite(12, HIGH); 
     for (int i = 0; i < 90000; i++) {
        float temperature;
        Serial.println("DeFrost");
        delay(10000);
        if (temperature >= sethigh) {
          break;
        }
     }
     
     
    } else {
     
        digitalWrite(12, LOW);
        Serial.print( "T = " );
        Serial.print( temperature, 1 );
        Serial.print( " deg. C, H = " );
        Serial.print( humidity, 1 );
        Serial.println( "%" ); 
    }

  }
}
