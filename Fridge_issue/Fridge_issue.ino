#include <dht_nonblocking.h>

//www.elegoo.com
//2018.10.25


#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

int count = 0; 
float setlow = 5.00;
float sethigh = 10.00;
bool Defrost = false;
bool cycle = false;
int incomingByte = 0; 
bool manual = false;
int option = 0;

/*
 * Initialize the serial port.
 */
void setup()
{ 
  
  Serial.begin( 9600);
  Serial.println("This device is aimed toward fixing defrost issues with fridges");
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
    if (temperature <= setlow && manual == false) {
      Defrost = true;
     
    } else if (temperature >= sethigh && manual == false) {
      Defrost = false;
        
    }
    Serial.println("_______________________________________");
    if (Defrost == true) {
      Serial.println("DEFROST: ON");
      digitalWrite(12, HIGH);
      cycle = true; 
      
    } else {
      Serial.println("DEFROST: OFF");
      digitalWrite(12, LOW);
      if (cycle == true){
        count++;
        cycle = false;
      }
    }
    if (manual == true) {
      Serial.println("MANUAL: ON ");
    } else {
      Serial.println("MANUAL: OFF");
    }
    Serial.print( "TEMPERATURE = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, HUMIDITY = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" ); 
    Serial.print("CYCLE COUNT:");
    Serial.println(count); 
    Serial.print("ENTER 2 TO TURN ON DEFROST MANUALLY, ");
    Serial.print("ENTER 3 TO TURN OFF DEFROST MANUALLY, ");
    Serial.print("ENTER 4 TO NORMAL DEFROST, ");
    Serial.println("ENTER 5 TO RESET ALL SETTINGS TO DEFAULT");
    if (Serial.available() >= 1) {
    // read the incoming byte:
    incomingByte = Serial.parseInt();
    Serial.print("OPTION ENTERED: ");
    Serial.println(incomingByte, DEC);
    if (incomingByte ==  2) {
      Serial.println("MANUAL DREFROST TURNED ON "); 
      manual = true;
      Defrost = true;
    }
    if (incomingByte ==  3) {
         Serial.println("MANUAL DREFROST TURNED OFF "); 
         manual = true;
         Defrost = false;
    }  
    if (incomingByte == 4) {
        Serial.println("NORMAL MODE");
        manual = false;
    
    }
    if (incomingByte == 5) {
       Serial.println("RESETTING!");
       count = 0; 
       Defrost = false;
       manual = false;
       Serial.println("FINISHED RESET!");
     }
      
    
    }
    if (count > 2147483647) {
      count = 0;
    }

    
  }


    

}
