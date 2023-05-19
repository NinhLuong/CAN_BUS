#include <ESP32CAN.h>
#include <CAN_config.h>

/* the variable name CAN_cfg is fixed, do not change */
CAN_device_t CAN_cfg;

void setup() {
    Serial.begin(9600);
    Serial.println("iotsharing.com CAN demo");
    /* set CAN pins and baudrate */
    CAN_cfg.speed=CAN_SPEED_1000KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_5;
    CAN_cfg.rx_pin_id = GPIO_NUM_4;
    /* create a queue for CAN receiving */
    CAN_cfg.rx_queue = xQueueCreate(10,sizeof(CAN_frame_t));
    //initialize CAN Module
    ESP32Can.CANInit();
    
}

void loop() {
    
    CAN_frame_t rx_frame;
    //receive next CAN frame from queue
    if(xQueueReceive(CAN_cfg.rx_queue,&rx_frame, 3*portTICK_PERIOD_MS)==pdTRUE){
      
      //do stuff!
      Serial.println("frame2");
      if(rx_frame.FIR.B.FF==CAN_frame_std){
        Serial.println("New standard frame");
        printf("New standard frame");
      }
      else{
        printf("New extended frame");
        Serial.println("New extended frame");}
      if(rx_frame.FIR.B.RTR==CAN_RTR){
        printf(" RTR from 0x%08x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        //Serial.println(" RTR from 0x%08x, DLC %d\r\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        }
      else{
        printf(" from 0x%08x, DLC %d\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        //Serial.println(" from 0x%08x, DLC %d\n",rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        for(int i = 0; i < 8; i++){
          printf("%c\t", (char)rx_frame.data.u8[i]);
       //   Serial.println("%c\t", (char)rx_frame.data.u8[i]);
        }
        printf("\n");
      }
    }
    else
    {
      //Serial.println("frame");
      rx_frame.FIR.B.FF = CAN_frame_std;
      rx_frame.MsgID = 1;
      rx_frame.FIR.B.DLC = 8;
      rx_frame.data.u8[0] = 'h';
      rx_frame.data.u8[1] = 'e';
      rx_frame.data.u8[2] = 'l';
      rx_frame.data.u8[3] = 'l'; 
      rx_frame.data.u8[4] = 'o';
      rx_frame.data.u8[5] = 'c';
      rx_frame.data.u8[6] = 'a';
      rx_frame.data.u8[7] = 'n';

      
      ESP32Can.CANWriteFrame(&rx_frame);
      //Serial.println("New standard frame2");
    }
}
//ESP32-CAN-Sender-Receiver.ino//==================================================================================//
//#include <CAN.h>
//#define TX_GPIO_NUM   21  // Connects to CTX
//#define RX_GPIO_NUM   22  // Connects to CRX
////==================================================================================//
//void setup() {
//  Serial.begin (115200);
//  while (!Serial);
//  delay (1000);
//  Serial.println ("CAN Receiver/Receiver");
//  // Set the pins
//  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
//  // start the CAN bus at 500 kbps
//  if (!CAN.begin (500E3)) {
//    Serial.println ("Starting CAN failed!");
//    while (1);
//  }
//  else {
//    Serial.println ("CAN Initialized");
//  }
//}
////==================================================================================//
//void loop() {
//  canReceiver();
//}
//
//void canReceiver() {
//  // try to parse packet
//  int packetSize = CAN.parsePacket();
//  Serial.println(packetSize);
//  if (packetSize) {
//    // received a packet
//    Serial.print ("Received ");
//    if (CAN.packetExtended()) {
//      Serial.print ("extended ");
//    }
//    if (CAN.packetRtr()) {
//      // Remote transmission request, packet contains no data
//      Serial.print ("RTR ");
//    }
//    Serial.print ("packet with id 0x");
//    Serial.print (CAN.packetId(), HEX);
//    if (CAN.packetRtr()) {
//      Serial.print (" and requested length ");
//      Serial.println (CAN.packetDlc());
//    } else {
//      Serial.print (" and length ");
//      Serial.println (packetSize);
//      // only print packet data for non-RTR packets
//      while (CAN.available()) {
//        Serial.print ((char) CAN.read());
//      }
//      Serial.println();
//    }
//    Serial.println();
//  }
//}
//==================================================================================//

//#include <CAN.h>
//
//#define TX_GPIO_NUM   4  // Connects to CTX
//#define RX_GPIO_NUM   5  // Connects to CRX
//
////==================================================================================//
//
//void setup() {
//  Serial.begin (115200);
//  while (!Serial);
//  delay (1000);
//
//  Serial.println ("CAN Receiver/Receiver");
//
//  // Set the pins
//  CAN.setPins (RX_GPIO_NUM, TX_GPIO_NUM);
//
//  // start the CAN bus at 500 kbps
//  if (!CAN.begin (500E3)) {
//    Serial.println ("Starting CAN failed!");
//    while (1);
//  }
//  else {
//    Serial.println ("CAN Initialized");
//  }
//}
//
////==================================================================================//
//
//void loop() {
//  canSender();
//  //canReceiver();
//}
//
////==================================================================================//
//
//void canSender() {
//  // send packet: id is 11 bits, packet can contain up to 8 bytes of data
//  Serial.print ("Sending packet ... ");
//
//  //CAN.beginPacket (0x12);  //sets the ID and clears the transmit buffer
//  CAN.beginExtendedPacket(0xabcdef);
//  CAN.write ('1'); //write data to buffer. data is not sent until endPacket() is called.
//  CAN.write ('2');
//  CAN.write ('3');
//  CAN.write ('4');
//  CAN.write ('5');
//  //Serial.println("2");
//  CAN.write ('6');
//  CAN.write ('7');
//  CAN.write ('8');
//  //Serial.println("3");
//  CAN.endPacket();
//  Serial.println("4");
//
//  //RTR packet with a requested data length
//  CAN.beginPacket (0x12, 3, true);
//  CAN.endPacket();
//
//  Serial.println ("done");
//
//  delay (1000);
//}
//
//void canReceiver() {
//  // try to parse packet
//  int packetSize = CAN.parsePacket();
//  Serial.println(packetSize);
//  if (packetSize) {
//    // received a packet
//    Serial.print ("Received ");
//    if (CAN.packetExtended()) {
//      Serial.print ("extended ");
//    }
//    if (CAN.packetRtr()) {
//      // Remote transmission request, packet contains no data
//      Serial.print ("RTR ");
//    }
//    Serial.print ("packet with id 0x");
//    Serial.print (CAN.packetId(), HEX);
//    if (CAN.packetRtr()) {
//      Serial.print (" and requested length ");
//      Serial.println (CAN.packetDlc());
//    } else {
//      Serial.print (" and length ");
//      Serial.println (packetSize);
//      // only print packet data for non-RTR packets
//      while (CAN.available()) {
//        Serial.print ((char) CAN.read());
//      }
//      Serial.println();
//    }
//    Serial.println();
//  }
//}


//==================================================================================//
