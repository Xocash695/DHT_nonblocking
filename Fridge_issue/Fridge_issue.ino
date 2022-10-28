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
int incomingByte = 0; 
bool manual = false;
int option;
bool safety = true;

/*
 * Initialize the serial port.
 */
void setup()
{ 
  
  Serial.begin( 9600);
  Serial.println("This device is aimed toward fixing defrost issues with fridges");
  Serial.println("Please Refer to https://github.com/Xocash695/Fridge-Defrost.git for more info");
  Serial.println("VERISON: 1.0");
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
      if (safety == true) {
        delay(30000);
        manual = false;
      }
    } else {
      Serial.println("MANUAL: OFF");
    }
    if (safety == true) {
      Serial.println("SAFETY: ON ");
    } else {
      Serial.println("SAFETY: OFF");
    }
    Serial.print( "TEMPERATURE = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, HUMIDITY = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" ); 
    Serial.print("CYCLE COUNT:");
    Serial.println(count); 
    if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    Serial.print("OPTION ENTERED: ");
    Serial.println(incomingByte, DEC);
    if (incomingByte ==  1) {
       Serial.println("ENTER SETLOW VALUE");
       setlow = Serial.read();
       Serial.println("ENTER SETHIGH VALUE");
       sethigh = Serial.read();
       Serial.println("NOTE ALL SETLOW AND SETHIGH WILL RETURN TO DEFAULT VALUES UPON RESTART!");

    }
    if (incomingByte ==  2) {
       Serial.println("MANUAL DEFROST TURN ON AND OFF");
       Serial.println("ENTER 1 TO TURN ON DEFROST, ENTER 2 TO TURN OFF DEFROST, ENTER 3 FOR AUTO");
       option = Serial.read();
       switch (option) {
        case 1: 
          Serial.println("MANUAL DREFROST TURNED ON "); 
          manual = true;
          Defrost = true;
          break;
        case 2: 
          Serial.println("MANUAL DREFROST TURNED OFF "); 
          manual = true;
          Defrost = false;
        case 3: 
          Serial.println("AUTO TURNED ON");
          manual = false;
       }
         
    }
    if (incomingByte == 3) {
       Serial.println("SAFETY FOR MANUAL: ENTER 1 TO TURN ON, ENTER 2 TO TURN OFF");
       option = Serial.read();
       if (option == 1) {
          Serial.println("SAFETY HAS BEEN TURNED ON");
          safety = true;
       } else if (option == 2) {
          Serial.println("SAFETY HAS BEEN TURNED OFF");
          Serial.println("THIS MODE IS NOT RECOMMENDED!");
          safety = false;
       }
         
    
    }
      if (incomingByte == 4) {
       Serial.println("RESETTING!");
       safety = true;
       count = 0; 
       setlow = -10.00;
       sethigh = 10.00;
       Defrost = false;
       manual = false;
       Serial.println("FINISHED RESET!");
      }
         
    
    }

    
  }


    

}
