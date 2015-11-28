/* ===================================================
 *  Programmer: Jackson Ritchey
 *  Demo firmware.
 *  Controls motor and reads data off of a range finder
 * =================================================== */

// driver for i2c
#include <Wire.h>

// driver for getting data off of lidarlitev2
#include <LIDARLite.h>

// driver for controlling a servo
#include <Servo.h>

// slip characters
typedef enum
{
  SLIP_END = 0xC0,
  SLIP_ESC = 0xDB,
  SLIP_ESC_END = 0xDC,
  SLIP_ESC_ESC = 0xDD
} SLIP_TYPES;

// encode data into a slip packet
int EncodeSlip(const unsigned char* data_raw, unsigned char* data_encoded, unsigned int len); 

Servo myservo;  // create servo object to control a servo

// angular position of servo
unsigned long pos = 0;   

// sign for controlling pos of servo
int sign = 1;

LIDARLite myLidarLite;

void setup() {
  Serial.begin(115200);
  myLidarLite.begin();
  myLidarLite.beginContinuous();
  pinMode(3, INPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {

  // get a few samples and average them
  unsigned long distance = 0;
  for(int i = 0; i < 5; i++)
  {
    distance += myLidarLite.distanceContinuous()*10; // convert from cm to mm
    delay(1);
  }
  distance /= 5;

  pos+=sign; // move servo
  myservo.write(pos);
  
  if(pos == 180 || pos == 0) // change direction
  {
    sign *= -1;
  }

  //  convert angular pos into expected format
  unsigned long ang_pos = pos * 1000/180;

  // create a data packet
  unsigned char buf[64];
  memcpy(buf, &ang_pos, sizeof(long));
  memcpy(buf+sizeof(long), &distance, sizeof(long));

  // encode the packet into slip
  unsigned char slip_buf[64];
  int len = EncodeSlip(buf, slip_buf, 8);

  // send the slip packet through serial port
  Serial.write(slip_buf, len);
  Serial.flush();
  
  
  delay(15);
}


// endode into slip format
int EncodeSlip(const unsigned char* data_raw, unsigned char* data_encoded, unsigned int len)
{
  int index = 0;
  for(int i = 0; i < len; i++)
  {
    if(data_raw[i] == SLIP_END)
    {
      data_encoded[index++] = SLIP_ESC;
      data_encoded[index++] = SLIP_ESC_END;
    }
    else if(data_raw[i] == SLIP_ESC)
    {
      data_encoded[index++] = SLIP_ESC;
      data_encoded[index++] = SLIP_ESC_ESC;
    }
    else 
    {
      data_encoded[index++] = data_raw[i];
    }
  }
  data_encoded[index++] = SLIP_END;

  return index;
}

