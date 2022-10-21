#include <dht_nonblocking.h>

//www.elegoo.com
//2018.10.25


#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

int count = 0; 
float setlow = -10.00;
float sethigh = 10.00;
bool Defrost = false;
bool cycle = false;

/*
 * Initialize the serial port.
 */
void setup()
{ 
  
  Serial.begin( 9600);
  Serial.println(" This device is aimed toward fixing defrost issues with fridges");
  Serial.println("Please Refer to https://github.com/Xocash695/Fridge-Defrost.git for more info");

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
  pinMode(12, OUTPUT);

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity ) == true )
  { 
    if (temperature <= setlow) {
     digitalWrite(12, HIGH);    
     Defrost = true;
     
    } else if (temperature >= sethigh) {
         digitalWrite(12, LOW);
         Defrost = false;
        
    }
    Serial.println("_______________________________________");
    if (Defrost == true) {
      Serial.println("Defrost: ON");
      cycle = true; 
      
    } else {
      Serial.println("Defrost: OFF");
      if (cycle == true){
        count++;
        cycle = false;
      }
    }
    Serial.print( "Temperature = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, Humidity = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" ); 
    Serial.print("The Count is :");
    Serial.println(count);

  }
}
