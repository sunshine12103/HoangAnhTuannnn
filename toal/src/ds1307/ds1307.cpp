#include "lib.h"
#include "receiveData.h"
namespace main {
    namespace ds1307 {
        RTC_DS1307 rtc;
        time thoiGian;

        void setup() {
            Wire.begin();
            if (!rtc.begin()) {
                Serial.println("Couldn't find RTC");
                while (1);
            }
            if (!rtc.isrunning()) {
                Serial.println("RTC is NOT running!");
                rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
            }
        }

        void readTime() {
            DateTime now = rtc.now();
            thoiGian.hour = now.hour();
            thoiGian.minute = now.minute();
            thoiGian.second = now.second();
        }

        bool caculatetime(int currentHour, int currentMinute, int startHour, int startMinute, int endHour, int endMinute) {
            int currentTotalMinutes = currentHour * 60 + currentMinute;
            int startTotalMinutes = startHour * 60 + startMinute;
            int endTotalMinutes = endHour * 60 + endMinute;

            if (startTotalMinutes <= endTotalMinutes) {
                return currentTotalMinutes >= startTotalMinutes && currentTotalMinutes <= endTotalMinutes;
            } else {
                return currentTotalMinutes >= startTotalMinutes || currentTotalMinutes <= endTotalMinutes;
            }
        }

        void caculateOFF() {
            if (caculatetime(thoiGian.hour, thoiGian.minute, receiveData::hourOn, receiveData::minuteOn, receiveData::hourOff, receiveData::minuteOff)) {
                main::controlDC::controlDC(1);
                Serial.println("CON CAC");

            } else {
                main::controlDC::controlDC(0);
                main::controlDC::esc.writeMicroseconds(1000);
                Serial.println("LON");
            }
        }
    }
}
