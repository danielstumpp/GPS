#include <SPI.h>
#include <RH_RF95.h>
#include <SoftwareSerial.h>

// miso 12
// mosi 11
// sck 13
#define RFM95_CS  4
#define RFM95_RST 2
#define RFM95_INT 3

#define TLED 8

#define GPS_TX 7
#define GPS_RX 8
 
#define RF95_FREQ 433.0
 
// radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

#define PACKET_SIZE 82
char packet[PACKET_SIZE];
String gpsBuffer;
char byteIn;
bool transmitReady;

// gps serial
SoftwareSerial GPS(GPS_TX, GPS_RX);

// blink our pretty test led
void testBlink(int n, int d)
{
  if (n <=0) return;
  if (d <= 0) return;

  for (int i=0; i<n; i++)
  {
    digitalWrite(TLED, HIGH);
    delay(d);
    digitalWrite(TLED, LOW);
    delay(d);
  }
}
 
void setup() 
{
  // initialize pins
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  pinMode(TLED, OUTPUT);
  testBlink(1, 500);
 
  // wait for serial
  while (!Serial);
  Serial.begin(9600); // start serial
  delay(100);

  GPS.begin(9600); // start gps
  delay(100);
 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  // init and check radio systems

  while (!rf95.init()) 
  {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
 
  if (!rf95.setFrequency(RF95_FREQ)) 
  {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 
  // Set transmit power to 23 dBm
  rf95.setTxPower(23, false);

  transmitReady = false;
  gpsBuffer.reserve(82); // allocate memory
  GPS.listen(); // start listening
}

void loop()
{
  // GPS Reading
  while (GPS.available()) 
  {
    byteIn = GPS.read(); // read 1 byte
    gpsBuffer += char(byteIn); // add byte to buffer
    if (byteIn == '\n') 
    { // end of line
      transmitReady = true; // ready to transmit
    }
  }

  // Transmitting
  if (transmitReady) 
  {
    if (gpsBuffer.startsWith("$GPGGA")) // only transmit what's needed
    {
      gpsBuffer.toCharArray(packet, PACKET_SIZE); // copy gps data to packet
      packet[PACKET_SIZE - 1] = 0; // null terminate the packet
  
      Serial.print("PACKET: ");
      Serial.println(packet);
      rf95.send((uint8_t*)packet, PACKET_SIZE); // cast pointer to unsigned character and send
      rf95.waitPacketSent(); // wait for send to finish
      
      // check for a response
      uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      Serial.println("Waiting for reply..."); delay(10);
      if (rf95.waitAvailableTimeout(1000))
      { 
        // Should be a reply message for us now   
        if (rf95.recv(buf, &len))
        {
          Serial.print("Got reply: ");
          Serial.println((char*)buf);
          String str((char*)buf);
          if (str.startsWith("CMD")) {
            testBlink(3, 80);
          }
          Serial.print("RSSI: ");
          Serial.println(rf95.lastRssi(), DEC);    
        }
        else
        {
          Serial.println("Receive failed");
        }
      }
      else
      {
        Serial.println("No reply, is there a listener around?");
      }
    } 
    else 
    {
      Serial.println("none");
    }

    delay(1000); // breathing room
    gpsBuffer = ""; // clear gps buffer
    transmitReady = false; // read more gps data
  }
}
