#include "WiFiEsp.h"
#include "WiFiEspUdp.h"
#include <Wire.h>
#include <MPU6050.h>

#define CLIENT_ID 1

#define WIFI_SSID "Your WiFi SSID"
#define WIFI_PASS "Your WiFi Pass"
int status = WL_IDLE_STATUS;

#define UDP_SERVER "Your server's IP"
#define UDP_PORT 4588
#define UDP_CLIENTPORT 4577
#define UDP_PACKETSIZE 8
byte udp_packet[UDP_PACKETSIZE];
byte udp_incomingpacket[UDP_PACKETSIZE];

WiFiEspUDP udp;

#define ECG_LOPLUSPIN 10
#define ECG_LOMINUSPIN 12
#define ECG_OUTPUTPIN A0
int ecg_data = 0;
byte ecg_pulserate = 0;
bool ecg_didbeat = false;

#define BTN_IMOKAYPIN 4
#define BTN_IMNOTOKAYPIN 2
uint8_t btn_imokayprevious = LOW;
uint8_t btn_imnotokayprevious = LOW;

#define BZR_PIN 8
unsigned long bzr_alarmuntil = 0;

MPU6050 mpu;

unsigned long cycle_time = 0;
unsigned long cycle2_time = 0;
unsigned long cycle3_time = 0;

void setup() {
  pinMode(ECG_LOPLUSPIN, INPUT);
  pinMode(ECG_LOMINUSPIN, INPUT);

  pinMode(BTN_IMOKAYPIN, INPUT_PULLUP);
  pinMode(BTN_IMNOTOKAYPIN, INPUT_PULLUP);

  pinMode(BZR_PIN, OUTPUT);

  Serial.begin(9600);
  Serial1.begin(115200);

  delay(500);

  Serial.println("ZOCareTracker-Client started.");

  // Connect to WiFi
  WiFi.init(&Serial1);
  
  if(WiFi.status() == WL_NO_SHIELD) {
    Serial.println("ESP8266 not connected.");
    while (1);
  }

  while (status != WL_CONNECTED) {
    Serial.print("Connecting to: ");
    Serial.println(WIFI_SSID);
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  }

  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());

  while (Serial1.available() > 0) { Serial1.read(); }
  udp.begin(UDP_CLIENTPORT);

  udp_packet[1] = CLIENT_ID;
  Serial.print("Client ID: ");
  Serial.println(CLIENT_ID);

  // Start GY-521
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G))
  {
    Serial.println("GY-521 not connected.");
    delay(1000);
  }

  mpu.setAccelPowerOnDelay(MPU6050_DELAY_3MS);
  
  mpu.setIntFreeFallEnabled(true);
  mpu.setIntZeroMotionEnabled(false);
  mpu.setIntMotionEnabled(false);
  
  mpu.setDHPFMode(MPU6050_DHPF_5HZ);

  mpu.setFreeFallDetectionThreshold(17);
  mpu.setFreeFallDetectionDuration(2);	
}

void loop() {
  // Check if any button pressed
  if (digitalRead(BTN_IMOKAYPIN) == LOW && btn_imokayprevious == HIGH)
  {
    udp_packet[0] = 2;
    udp_packet[2] = 2;
    udp_sendpacket();

    bzr_alarmuntil = 0;

    digitalWrite(BZR_PIN, HIGH);
    delay(5);
    digitalWrite(BZR_PIN, LOW);
  }

  if (digitalRead(BTN_IMNOTOKAYPIN) == LOW && btn_imnotokayprevious == HIGH)
  {
    udp_packet[0] = 2;
    udp_packet[2] = 3;
    udp_sendpacket();

    bzr_alarmuntil = 0;

    digitalWrite(BZR_PIN, HIGH);
    delay(5);
    digitalWrite(BZR_PIN, LOW);
  }

  btn_imokayprevious = digitalRead(BTN_IMOKAYPIN);
  btn_imnotokayprevious = digitalRead(BTN_IMNOTOKAYPIN);

  // Fall detection
  if(mpu.readActivites().isFreeFall){
    udp_packet[0] = 3;
    udp_packet[2] = 2;
    udp_sendpacket();
    bzr_alarmuntil = millis() + 60000;
  }

  if(millis() <= bzr_alarmuntil){
    for(byte i=0;i<10;i++) {
      digitalWrite(BZR_PIN, HIGH);
      delay(1);
      digitalWrite(BZR_PIN, LOW);
      delay(1);
    }
  }

  int udp_incomingpacketsize = udp.parsePacket();
  if (udp_incomingpacketsize) {
    int udp_incomingpacketlen = udp.read(udp_incomingpacket, UDP_PACKETSIZE);
    if (udp_incomingpacketlen > 0) {
      udp_incomingpacket[udp_incomingpacketlen] = 0;
      switch(udp_incomingpacket[0]){
        case 4:
          bzr_alarmuntil = millis() + 3600000;
          break;
      }
    }
  }

  // ECG
  if (millis() - cycle_time >= 10){
    if ((digitalRead(ECG_LOPLUSPIN) != 1) || (digitalRead(ECG_LOMINUSPIN) != 1)) {
      ecg_data = analogRead(ECG_OUTPUTPIN);
      if(ecg_data >= 410){
        if(!ecg_didbeat){
          ecg_pulserate++;
          ecg_didbeat = true;
        }
      }else{
        ecg_didbeat = false;
      }
    }
    cycle_time += 10;
  }

  // Send data type 0
  if (millis() - cycle2_time >= 60000){
    udp_packet[0] = 0;
    udp_packet[2] = ecg_pulserate;
    udp_packet[3] = millis() & 0xFF;
    udp_packet[4] = (millis() >> 8) & 0xFF;
    udp_packet[5] = (millis() >> 16) & 0xFF;
    udp_packet[6] = (millis() >> 24) & 0xFF;
    udp_packet[7] = (char) mpu.readTemperature();
    udp_sendpacket();

    ecg_pulserate = 0;
    cycle2_time += 60000;
  }

  // Reset ESP8266
  // This issue is regarding my own microcontroller, you can remove this part if you don't have any problems sending packets without needing to reset every hour.
  if (millis() - cycle3_time >= 3600000){
    udp.stop();
    WiFi.reset();
    status = WL_IDLE_STATUS;
    while (status != WL_CONNECTED) {
      status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    }
    udp.begin(UDP_CLIENTPORT);
    cycle3_time += 3600000;
  }
}

void udp_sendpacket() {
  udp.beginPacket(UDP_SERVER, UDP_PORT);
  udp.write(udp_packet, UDP_PACKETSIZE);
  udp.endPacket();

  memset(udp_packet, 0, sizeof(udp_packet));
}
