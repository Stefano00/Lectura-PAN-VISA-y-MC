#define DEBUG(a) Serial.println(a);
 
void setup()
{
   Serial.begin(9600);
}
 
 
void loop()
{
   if (Serial.available())
   {
      String data = Serial.readStringUntil('\n');
      DEBUG(data);
   }
}
