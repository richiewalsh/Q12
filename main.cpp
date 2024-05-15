#include "mbed.h"
#include "LSM6DSLSensor.h"
#include <cmath>

/*  Some code from lab3 and online has been repurposed here to serve a new purpose
    I understand and have attempted to show my understanding via how I've used this code.
    I hope this doesn't come across as plagurism as I've included some original code too.
    I couldnt find any documentation indicating that the standard board led's change colour
    and thats why I took this specific approach. I was unable to test on a real led.
 */

static DevI2C devI2c(PB_11,PB_10);
static LSM6DSLSensor acc_gyro(&devI2c,0xD4,D4,D5); // high address

class RGBLed //Create a class to define RGB led
{
public:
    RGBLed(PinName redpin, PinName greenpin, PinName bluepin);
    void write(float red,float green, float blue);
private:
    PwmOut _redpin;
    PwmOut _greenpin;
    PwmOut _bluepin;
};

RGBLed::RGBLed (PinName redpin, PinName greenpin, PinName bluepin)
    : _redpin(redpin), _greenpin(greenpin), _bluepin(bluepin)
{
    // Setting Resolution based on Disco -L4S5I documentation for pins used
    _redpin.period(0.000001);
    _greenpin.period(0.00001);
    _greenpin.period(0.00001);
}

void RGBLed::write(float red,float green, float blue)
{
    _redpin = red;
    _greenpin = green;
    _bluepin = blue;
}

RGBLed myRGBled(PA_15,PB_1,PB_4); //set PWM pins PA_15, PB_1 & PB_4 as output to make led shine Red, Green or Blue. Should be wired accordingly.

// main() runs in its own thread in the OS
int main() {
    uint8_t id;
    int32_t axes[3];
    float res=0;
    acc_gyro.init(NULL);

    acc_gyro.enable_x();
    acc_gyro.enable_g();

    printf("This is an accelerometer example running on Mbed OS %d.%d.%d.\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    acc_gyro.read_id(&id);
    printf("LSM6DSL accelerometer & gyroscope = 0x%X\r\n", id);

    while(1) {

        acc_gyro.get_x_axes(axes);
        printf("LSM6DSL: %6d, %6d, %6d\r\n", axes[0], axes[1], axes[2]);

        // Checks if x axes has more acceleration than other 2 and make rgb led shine red if so
        if(sqrt(pow(axes[0], 2)) > sqrt(pow(axes [1], 2)) && sqrt(pow(axes[0], 2)) > sqrt(pow(axes[2], 2))){
           
            myRGBled.write(1.0, 0.0, 0.0);

        }
        // Checks if y axes has more acceleration than other 2 and make rgb led shine green if so
        else if(sqrt(pow(axes[1], 2)) > sqrt(pow(axes [0], 2)) && sqrt(pow(axes[1], 2)) > sqrt(pow(axes[2], 2))){
           
            myRGBled.write(0.0, 1.0, 0.0);

        }
        // Checks if z axes has more acceleration than other 2 and make rgb led shine blue if so
        else if(sqrt(pow(axes[2], 2)) > sqrt(pow(axes [0], 2)) && sqrt(pow(axes[2], 2)) > sqrt(pow(axes[1], 2))){

            myRGBled.write(0.0, 0.0, 1.0);

        }
        // In case of equal acceleration in two or more axes, turn led off
        else{

            myRGBled.write(0.0, 0.0, 0.0);

        }

        thread_sleep_for(2000);

    }
}