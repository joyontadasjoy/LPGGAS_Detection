#include <AltSoftSerial.h>
AltSoftSerial gsmPort; // must be on specific pins (GSM TX-8 & GSM RX-9 for an UNO)
#include <NMEAGPS.h>
NMEAGPS gps;
gps_fix fix;
#define gpsPort Serial // GPS TX connected to Arduino RX pin 0
#include <dht.h>
dht DHT;
#define DHT11_PIN 3
#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,7,6,5,4);

int BUZZER=13;
int FlamePin= 2;
int Flame=HIGH;
float sensor=A0;
float gas_value;

void setup()
{ pinMode(BUZZER, OUTPUT);
pinMode(FlamePin, INPUT);
pinMode(sensor,INPUT);
gsmPort.begin(9600);
Serial.begin(9600);
lcd.begin(16,2);
//gpsPort.begin( 9600 ); // Not needed if GPS on pin 0
}

void loop()
{
Flame= digitalRead(FlamePin);
int chk = DHT.read11(DHT11_PIN);
gas_value=analogRead(sensor);
lcd.clear();
if(Flame==HIGH || DHT.temperature > 50 || gas_value >1000)
{
lcd.setCursor(0,0);
lcd.print("FIRE ALERT");
lcd.setCursor(0,1);
lcd.print("FIRE ALERT");
digitalWrite(BUZZER,HIGH);
if (gps.available( gpsPort ))
fix = gps.read();
if(fix.valid.location)
{
SendMessage();
if (gsmPort.available()>0)
Serial.write(gsmPort.read());
}
}
else
{ digitalWrite(BUZZER, LOW);
Serial.println(" No FIRE ");
Serial.print("Temperature = ");
Serial.println(DHT.temperature);
Serial.print("GAS VALUE = ");
Serial.println(gas_value);
lcd.setCursor(0,0);
lcd.print("NO FIRE");
lcd.setCursor(0,1);
lcd.print("ALL SECURED");
delay(2000);
}
}

void SendMessage()
{
gsmPort.println("AT+CMGF=1");
delay(1000);
gsmPort.println("AT+CMGS=\"+8801853114432\"\r");
delay(1000);
gsmPort.println("FIRE Alert");
gsmPort.print( fix.longitude(), 5);
gsmPort.print( ',' );
gsmPort.print( fix.latitude(), 5 );
delay(100);
gsmPort.println((char)26);
delay(1000);
}
