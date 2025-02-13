#include "lib.h"
namespace main{
    namespace setup{
        void setup(){
            Serial.begin(115200);
            main::receiveData::setupReceive();
            main::controlDC::setupDC();
            main::controlDC::setupBLDC();
            main::ds1307::setup();
        }
    }
}