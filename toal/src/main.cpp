#include "lib.h"

void setup(){
  main::setup::setup();
  //main::controlDC::setHomeDC();
}
void loop(){
  main::receiveData::receiveSerialData();
//   main::ds1307::readTime();
//   //main::controlDC::controlDC(main::setup::lake);
//   Serial.println("Time: ");
//   Serial.print(main::ds1307::thoiGian.hour);
//   Serial.print(":");
//   Serial.print(main::ds1307::thoiGian.minute);
//   Serial.println();
//  //main::controlDC::controlDC(1);


//   main::ds1307::caculateOFF();
delay(1000);
}