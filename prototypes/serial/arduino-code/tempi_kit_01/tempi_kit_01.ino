#include <Messenger2.h>
#include <TinkerKit.h>

Messenger message = Messenger();
TKPotentiometer pot_I0(I0);
TKPotentiometer pot_I1(I1);
TKPotentiometer pot_I2(I2);
TKLed led_O0(O0);
TKLed led_O1(O1);
TKLed led_O2(O2);
unsigned long interval_poll = 50;
unsigned long last_poll_time = 0;
boolean is_polling = false;

void send_ok(const char *command)
{
  Serial.print("ok ");
  Serial.println(command);
  Serial.flush();
}

void send_read_values()
{
  // I0
  Serial.print("I0");
  Serial.print(" ");
  Serial.print(pot_I0.get(), DEC); // [0-1023]
  Serial.println();
  // I1
  Serial.print("I1");
  Serial.print(" ");
  Serial.print(pot_I1.get(), DEC); // [0-1023]
  Serial.println();
  // I2
  Serial.print("I2");
  Serial.print(" ");
  Serial.print(pot_I2.get(), DEC); // [0-1023]
  Serial.println();
  // TODO: other ones
  Serial.flush();
}

void handleMessage()
{
  if (message.checkString("start"))
  { // start polling
    is_polling = true;
    send_ok("start");
  }
  else if (message.checkString("stop"))
  { // stop polling
    is_polling = false;
    send_ok("stop");
  }
  else if (message.checkString("interval"))
  {
    int value = message.readInt();
    if (value < 0 || value > 100000)
    {
      Serial.println("error out_of_range");
      Serial.flush();
    }
    else
    {
      interval_poll = (unsigned int) value;
      send_ok("interval");
    }
  }
  else if (message.checkString("r"))
  { // Read
    send_read_values();
  }
  else if (message.checkString("led0"))
  {
    int value = message.readInt();
    led_O0.brightness(value); // [0-1023]
    send_ok("led0");
  }
  else if (message.checkString("led1"))
  {
    int value = message.readInt();
    led_O1.brightness(value); // [0-1023]
    send_ok("led1");
  }
  else if (message.checkString("led2"))
  {
    int value = message.readInt();
    led_O2.brightness(value); // [0-1023]
    send_ok("led2");
  }
  else if (message.checkString("ping")) // ping
  {
    Serial.println("pong");
    Serial.flush();
  }
  else
  {
    Serial.println("error no such handler");
    Serial.flush();
  }
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
  if (is_polling)
  {
    if ((millis() - last_poll_time) > interval_poll)
    {
      last_poll_time = millis();
      send_read_values();
    }
  }
}

