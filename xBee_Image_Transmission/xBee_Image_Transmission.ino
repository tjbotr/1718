#include <Adafruit_VC0706.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX


// SD card chip select line varies among boards/shields:
// Adafruit SD shields and modules: pin 10
#define chipSelect 10
#if ARDUINO >= 100

//camera TX connected to pin 4, camera RX to pin 5:
SoftwareSerial cameraconnection = SoftwareSerial(4, 5);
#else
NewSoftSerial cameraconnection = NewSoftSerial(4, 5);
#endif

Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);    //////declaring cam variable
git pus
int indicator_done = 0;

void setup() {
  XBee.begin(1200);

  pinMode(7, OUTPUT); ///////indicator
  pinMode(6, OUTPUT);

  Serial.begin(1200);
  Serial.println("VC0706 Camera snapshot test");

 

 // When using hardware SPI, the SS pin MUST be set to an
  // output (even if not connected or used).  If left as a
  // floating input w/SPI on, this can cause lockuppage.
#if !defined(SOFTWARE_SPI)
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  if (chipSelect != 10) pinMode(10, OUTPUT); // SS on Uno.
#endif
#endif

}

void loop() {
  
    // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }

  // Try to locate the camera
  if (cam.begin())
  {
    Serial.println("Camera Found:");
  }
  else
  {
    Serial.println("No camera found?");
    return;
  }

  // Set the picture size
  cam.setImageSize(VC0706_640x480);        // biggest//////////////////////////////////////////////////////////////////////////////////////////////////
  //cam.setImageSize(VC0706_320x240);        // medium
 // cam.setImageSize(VC0706_160x120);          // small

  //Read the size back from the camera
 // uint8_t imgsize = cam.getImageSize();
  Serial.print("Image size: ");
 // if (imgsize == VC0706_640x480) Serial.println("640x480");
 // if (imgsize == VC0706_320x240) Serial.println("320x240");
//  if (imgsize == VC0706_160x120) Serial.println("160x120");

  Serial.println("Snap in 5 secs...");
  delay(5000);

  if (! cam.takePicture())
    Serial.println("Failed to snap!");
  else
  {
    Serial.println("Picture taken!");
    digitalWrite(7, HIGH);
    delay(50);
    digitalWrite(7, LOW);
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Create an image with the name IMAGExx.JPG
  char filename[14];
  strcpy(filename, "EMMAG000.JPG");
  for (int i = 0; i <= 4; i++) {
    filename[5] = '0' + i / 10;
    filename[6] = '0' + i % 10;
    filename[7] = '0' + i % 10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  // Open the file for writing
  File imgFile = SD.open(filename, FILE_WRITE);

  // Get the size of the image (frame) taken
 /// uint16_t jpglen = cam.frameLength();//////////////////////////////do not remove
  uint16_t jpglen = 1300000;  //1300000
  Serial.print("jpglen: "); //////////////////////////////////////////////added
  Serial.print(jpglen); ///////////////////////////////////////////////////////////added
  Serial.print("Storing ");
  Serial.print(jpglen, DEC);
  Serial.print(" byte image.");
 


  int32_t time = millis();
 // pinMode(8, OUTPUT);
  // Read all the data up to # bytes!
  byte wCount = 0; // For counting # of writes
  while (jpglen > 0)
  {
    indicator_done =1;
    // read 32 bytes at a time;
    uint8_t *buffer;
    uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
    buffer = cam.readPicture(bytesToRead);
    imgFile.write(buffer, bytesToRead);
    XBee.write(buffer, bytesToRead);

    //Serial.print("Read ");  Serial.print(bytesToRead, DEC); Serial.println(" bytes");
    jpglen -= bytesToRead;
  //  Serial.write(imgFile);////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  }
  imgFile.close();

  time = millis() - time;
  Serial.println("done!");
  Serial.print(time); Serial.println(" ms elapsed");

  delay(2000);
  if(indicator_done!=0){
    digitalWrite(7, HIGH);
    delay(2000);
    digitalWrite(7, LOW);
    delay(2000);
  }
 
  Serial.print("Reseting after 10 seconds.");
  delay(1000); ///wait for 10 seconds before resetting
  pinMode(8, OUTPUT);
  
}
