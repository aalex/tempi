#include <Messenger2.h>
#include <TinkerKit.h>

Messenger message = Messenger();
TKPotentiometer pot_I0(I0);
TKLed led_O0(O0);
static const unsigned long interval_hello = 2000;
unsigned long last_hello_time = 0;

void handleMessage()
{
  if (message.checkString("r"))
  { // Read
      Serial.print("I0");
      Serial.print(" ");
      Serial.print(pot_I0.get(), DEC); // [0-1023]
      Serial.println();
  }
  else if (message.checkString("w"))
  {
    if (message.checkString("O0"))
    {
      int value = message.readInt();
      led_O0.brightness(value); // [0-1023]
    }
    else
    {
      Serial.println("error no such led");
    }
  }
  else if ( message.checkString("ping")) // ping
  {
    Serial.println("pong");
  }
  else
  {
    Serial.println("error no such handler");
  }
  Serial.flush();
}

void setup()
{
  // Initiate Serial Communication
  Serial.begin(9600); //115200); 
  message.attach(handleMessage);
  Serial.println("hello");
  Serial.flush();
}

void loop()
{
  while (Serial.available())
  {
    message.process(Serial.read());
  }
  //if ((millis() - last_hello_time) > interval_hello)
  //{
    //Serial.println("hello");
  //  last_hello_time = millis();
  //  Serial.flush();
  //}
}
