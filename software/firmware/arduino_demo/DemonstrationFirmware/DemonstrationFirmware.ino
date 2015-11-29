/* ========================= 
 *  
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

//#define MAX_POS 2000
//#define MIN_POS 1000

#define MAX_POS 2400
#define MIN_POS 544

#define NUM_TICKS ((MAX_POS-MIN_POS)/4)
#define AVG 2

#define PHI1 130

#define PHI2 40


Servo myservo;  // create servo object to control a servo

// angular position of servo
long pos = 0;   

// sign for controlling pos of servo
int sign = 1;

LIDARLite myLidarLite;

void setup() {
  Serial.begin(115200);
  myLidarLite.begin(1, true);
  myLidarLite.beginContinuous();
  pinMode(3, INPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

// get a few samples and average them
unsigned long distance = 0;

void loop() {

  pos+=(sign*(MAX_POS-MIN_POS)/NUM_TICKS); // move servo
  myservo.writeMicroseconds(MIN_POS+pos);
  
  if(pos <= 0 || pos >= (MAX_POS-MIN_POS)) // change direction
  {
    sign *= -1;
  }

  

  for(int i = 0; i < AVG; i++)
  {
    distance += myLidarLite.distanceContinuous()*10; // convert from cm to mm
    delay(1);
  }
  distance /= AVG;
  unsigned long ang_pos = 0;
  if(sign < 0)
  {
    //  convert angular pos into expected format
    ang_pos = ((pos+(PHI1)) * 1000) / (MAX_POS - MIN_POS);
  }
  else
  {
    ang_pos = ((pos-(PHI2)) * 1000) / (MAX_POS - MIN_POS);
  }
  
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

