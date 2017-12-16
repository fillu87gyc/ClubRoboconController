#include <SoftwareSerial.h>
#include <PS3USB.h>

typedef unsigned char unchar;
//rx tx
SoftwareSerial XBee(7, 6);

class Message
{
  public:
    Message();
    void out();
    void make();
  private:
    unchar data[8];
};

enum num { Lx, Ly, Rx, Ry, sw1, sw2, sum };

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

//instance
USB Usb;
PS3USB PS3(&Usb);
Message msg;
//0-255 -> 0-100
int limit(int a, int max, int min)
{
  int ave=(max+min)/2;
  if(a>max)return max;
  if(a<min)return min;
  if(a<ave+5&&a>ave-5)return ave;
  return a;
}
int limit(int a, int max, int min,int errorrange)
{
  int ave=(max+min)/2;
  if(a>max)return max;
  if(a<min)return min;
  if(a<ave+errorrange&&a>ave-errorrange)return ave;
  return a;
}


Message::Message()
{
  //init
  for (int i = 0; i < 8; i++)
    msg.data[i] = 0;
}

void Message::make()
{
  //joystick
  msg.data[Lx] = limit((int)(PS3.getAnalogHat(LeftHatX)/2.55),100,0);
  msg.data[Ly] = 100 - limit((int)(PS3.getAnalogHat(LeftHatY)/2.55), 100, 0);
  msg.data[Rx] = limit((int)(PS3.getAnalogHat(RightHatX)/2.55), 100, 0);
  msg.data[Ry] = 100 - limit((int)(PS3.getAnalogHat(RightHatY)/2.55), 100, 0);
  //button
  msg.data[sw1] = (unchar)PS3.ButtonState;
  msg.data[sw2] = (0xFF00 & PS3.ButtonState) >> 8;

  //Check Sum
  msg.data[sum] = msg.data[Lx] + msg.data[Ly] + msg.data[Rx] + msg.data[Ry] + msg.data[sw1] + msg.data[sw2];
}

void Serial_out(unchar data)
{
  XBee.write(data);
//  Serial.print(data, DEC);
//  Serial.print(' ');
}

void Message::out()
{
  Serial_out('S');
  Serial_out(0xEB);
  Serial_out(msg.data[Lx]);
  Serial_out(msg.data[Ly]);
  Serial_out(msg.data[Rx]);
  Serial_out(msg.data[Ry]);
  Serial_out(msg.data[sw1]);
  Serial_out(msg.data[sw2]);
  Serial_out(msg.data[sum]);
  Serial_out(0xAF);
  Serial_out(0x0D);
  Serial_out(0x0A);
  delay(20);
}
void standBy_check()
{
  Serial.print("check\n\r");
}
void setup()
{
  Serial.begin(115200);
  XBee.begin(38400);
  delay(2000);
  if (Usb.Init() == -1)
  {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("PS3 USB Library Started"));
  standBy_check();
}

void loop()
{
  Usb.Task();
  msg.make();
  msg.out();
}

