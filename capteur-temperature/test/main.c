#include <unity.h>

void setUp(void) {
  // initialize serial communication
    Serial.begin(115200);
    int timeoutInitSerial = 100;
    while (timeoutInitSerial-- > 0)
    {
        if (Serial)
            break;
        delay(10);
    }
}

int main( int argc, char **argv) {
    UNITY_BEGIN();
    UNITY_END();
}
