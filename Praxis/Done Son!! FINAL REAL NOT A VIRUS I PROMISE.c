//
// Project name: buggy
// ====================
//
// A simple skeleton for wattbot in C
//
// Author
//      Jim Herd, April 2006
//      Yvan Petillot 2009
//      Andrew Sharp 2015
//
// Description
//      Basic program to demonstrate moving the wattbot around
//      Wait for 3 seconds
//      Do the following sequence 3 times
//              Move the wattbot forward for 4 seconds then backwards for 4 seconds.
//      Turn left for 1 second
//      
// Target configuration
//    MCU         :   P18F4585
//    Oscillator  :   HS, 40.0000MHz
//    Software    :   Microchip C18 V3.02 Student Version
//                :   MPLAB V7.31
//
// Buggy spec
//    1. WattBot I vehicle with PIC18F4585 device
//    2. Full details of the vehicle are available on Vision
//    3. LEFT and RIGHT motors are defined looking from the back to the front of the vehicle.
//           The RIGHT motor is driven by PWM unit 1.
//           The LEFT motor is driven by PWM unit 2.
//    4. The processor on the vehicle can be either an 18F452 or an 18F4585
//
// General notes
//    1. All required code has been included in a single source file.

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// == Include Files ==
//
// Here we 'include' other files that have useful functions that we need.
// The files that we include are called 'header' files.
// Apparently some people call these 'copybooks'. See if you can find one.
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

// This header file allows us to program the PIC using meaningful words
#if defined(__18F452)
#include   <p18f452.h>    
#endif
#if defined(__18F4585)
#include    <p18f4585.h>    
#endif

#include    <delays.h>          // delays based on processor instructions.
#include    <timers.h>          // simplifies onboard timer modules.
#include    <pwm.h>                     // Pulse Width Modulation is used to vary the speed.
#include    <stdlib.h>          // General Utilities - search it if you are interested.
#include        <math.h>                //maths.
#include        <stdio.h>



// Here we define datatypes.
typedef  unsigned char  UINT8;
typedef  unsigned int   UINT16;

typedef enum {FORWARD, BACKWARD, CONTINUE} DIRECTION;
const double pi = 3.14159265359;

// Delay routines need the clock speed to calculate delays properly
#define     PIC_CLK      40000000 //MHz

#define     OFF_PWM      0              // base PWM value == stopped
#define     FULL_PWM     5              // scale factor to use 0->100% rather than 0->500 dutycycle
#define         M_WHEEL_DIST 20         // replace with the distance between the wattbot wheels on the ground

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// == Function Prototypes ==
//
// Calling a function before it is declared will end badly.
// Add function prototypes here.
// Adding comments here makes it easy to see what all of your functions can do
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void main (void);

void MoveTime(int power, unsigned int time);            // move robot in a straight line
void TurnTime(int power, unsigned int time);            // turn robot
void TurnAngle(float angle, int speed);                         // turn robot specific angle
void MoveDistance(float distance_mm,int speed);         // move robot forward a specific distance
void TurnArcRight(float diameter_mm, int speed, int angle);
void TurnArcLeft(float diameter_mm, int speed, int angle);
void ZigZag(int speed, int sections);
double Dist_mm(void);                                                           //Distance to object
void AvoidObs();
void OldAvoidObs();
void FindObj();
int MoveScan(int distance_mm, int speed, int interval_ms);

void SetPower(float left_power, float right_power);                                                     // set motor power as percentage
void SetSpeed(int left_speed, int right_speed);                                                 // set motor speed in m/s
void SetDirection(DIRECTION left_direction, DIRECTION right_direction); // set motor direction
void Stop(void);                                                                                                                // stop both motors

void DelayUs(UINT8 cnt);                                                // 0->255 microseconds delay
void DelayMs(UINT16 cnt);                                               // 0->65535 milliseconds delay
void DelayS(UINT8 cnt);                                                 // 0->255 seconds delay

void OpenBoth_PWM( UINT8 period );                                      // configure PWM channels
void SetDutyCyclePWM1(unsigned int dutycycle);          // set Right motor PWM duty
void SetDutyCyclePWM2(unsigned int dutycycle);          // set Left motor PWM duty

void ADC_init(void);                                                            // configure the ADC
UINT16  ADC_Read(UINT8 channel);                                        // read value from ADC

void init(void);                                                                        // set up the wattbot                                                           

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// == Main Program Body ==
//
// This is where you should put things that you want the wattbot to do.
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void main (void)
{
        // if you get errors when declaring variables try moving them to the start of a function
        int counter;

        // We need to run the 'init' function before doing anything
        // This function tells the wattbot's 'brain' (PIC) how to use its 'legs' (motors)    
        int i = 0;
        init();

        AvoidObs();
        //FindObj();

        Stop();
        
        // This is an infinite for loop, the code inside will run forever.
        // See if you can find out why it is important in this program!
    for(;;) {}
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// == Actions ==
//
// High level function describing wattbot actions.
//
// MoveTime(int power, unsigned int time)
//      Purpose:        move in a straight line at a speed for a time
//      Input:          power:  motor power from -100 to 100
//                              time:   time to move for
//
// TurnTime(int power, unsigned int time)
//      Purpose:        rotate wattbot around its center
//      Input:          power:  motor power from -100 to 100
//                              time:   time to rotate for
//
// TurnAngle(float angle, int speed)
//      Purpose:        rotate wattbot by a specified angle
//      Input:          angle:  angle to rotate in degrees
//                              speed:  speed at which to perform the turn
//      TurnAngle requires a calibrated SetSpeed function to work accurately
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//a function that moves and scans AT THE SAME TIME!!!


int MoveScan(int distance_mm, int speed, int interval_ms){
        float time_ms = ((float)distance_mm / speed) * 1000;
        float num_scans = time_ms/interval_ms;
        int ADC = 0;
        int i = 0;
        int j = 0;
        int ADC2 = 0;
        for (i; i<num_scans;i++){
                SetSpeed(speed,speed);
                DelayMs(interval_ms);
                ADC = ADC_Read(0);
                if(ADC < 266 && ADC > 204 && ADC2 > ADC){
                        return 1;
                }
                ADC2 = ADC;
                if (i % 100 == 0) {
                        SetSpeed(0, 0);
                        for(j=0; j<=10; j++) {
                                if(ADC < 266 && ADC > 204 && ADC2 > ADC){
                                        break;
                                }
                                TurnAngle(2,200);
                        }
                        for(j=0; j<=10; j++) {
                                if(ADC < 266 && ADC > 204 && ADC2 > ADC){
                                        break;
                                }
                                TurnAngle(-2,200);
                        }
                }
        }
        return 0;
}

void FindObj() {
        //position in center of square
        int found = 0;
        int x = 30;
        int s = 200;
        double Dist = 0.0;
        TurnAngle(145, 200);
        found = MoveScan(1150,200,10); 
        TurnAngle(45, 200);
        while(!found){
                int i = 0;

                //move x
                found = MoveScan(x,s,10);
                if(found == 1) break;
                //turn left
                TurnAngle(-90,150);
                //move x+50
                found = MoveScan(x+50,s,10);
                if(found == 1) break;
                //turn left
                TurnAngle(-90,150);
                //move x+50
                found = MoveScan(x+50,s,10);
                if(found == 1) break;
                //turn left
                TurnAngle(-90,150);
                //move x
                found = MoveScan(x,s,10);
                if(found == 1) break;
                //repeat

                x = x + 50;
        }
        Stop();
}

void oldFindObj(){
        int found = 0;
        float Dist = 0.0;
        int i = 0;
        while(found == 0) {
                if (found == 0) {
                        for(i = 0; i < 9; i++){
                                Dist = Dist_mm();
                                TurnAngle(10,200);
                                if(Dist >100 && Dist < 400)
                                        found = 1;
                                        //break;
                        }
                        MoveDistance(1900,200);
                        TurnAngle(180,200);
                        for(i = 0; i < 9 ; i++){
                                Dist = Dist_mm();
                                TurnAngle(10,200);
                                if(Dist >100 && Dist < 500)
                                        found = 1;
                                        //break;
                        }
                        MoveDistance(1900,200);
                        TurnAngle(180,200);
                        for(i = 0; i < 9; i++){
                                Dist = Dist_mm();
                                TurnAngle(10,200);
                                if(Dist >100 && Dist < 500)
                                        found = 1;
                                        //break;
                        }

                        MoveDistance(1900,200);
                        TurnAngle(180,200);
                        for(i = 0; i < 9; i++){
                                Dist = Dist_mm();
                                TurnAngle(10,200);
                                if(Dist >100 && Dist < 500)
                                        found = 1;
                                        //break;
                        }
                        //If the brick has not been found in this area robot to move to the center of the box and try to find the block
                        MoveDistance(1000,200);
                        TurnAngle(-90,200);
                        MoveDistance(1000,200);
                        for(i = 0; i < 36; i++){
                                Dist = Dist_mm();
                                TurnAngle(10,200);
                                if(Dist >100 && Dist < 500)
                                        found = 1;
                                        //break;
                        }
                        break;
                }
        }
        if (found == 1){
                while(Dist >= 100)
                {
                        SetSpeed(200,200);
                }
        }
}
void OldAvoidObs(){
    const int min_gap = 90;
    float dist = 0;
    int gap = -1;
    int start = -1;
    int end = -1;
    int count = 0;
    int direction = 1;
        int i = 0;
    for (;;) {
                dist = Dist_mm();
        //scan until a 90 degree gap has been found.
                count = 0;
                gap = 0;
                start = 0;
                end = 0;
        while (gap < min_gap && count <= 360) {
            if (ADC_Read(0) < 500) {
                start = count;
            }
            if (ADC_Read(0) > 250 || ADC_Read(0) < 50) {
                end = count;
            }
            gap = end - start;
            TurnAngle(1*direction, 200);
            count++;
        }

        //Turn to the middle of the gap
        TurnAngle((-direction*min_gap/2), 200);
    
        //Move while checking for changes

        for (i = 0; i <= 20; i++) {
            MoveDistance(10,200);
            if (dist < 200) break;
        }

        //swap turn direction (to avoid trending left/right.
        direction *= -1;
    }

}

//This function is desingned for the robot to aviod obtacles forever
void AvoidObs(){
        float wspeed = 0.0;             //A wheel speed worked out from the distance of the robot to an object
        float dist = 0.0;               //The distance of the robot to an object
        int count =0;                   //used to ocilate the directions of the wheels
        float initial = 0.0;    //Avoiding mistakes due to mulitiple distances giving the same voltage
        float final = 0.0;
        int i = 0;
        while (1) {
//Check if the sensor is detecting anything at all
                if(ADC_Read(0) > 114.576){
                        dist = Dist_mm();
//Checks if the voltage recieved is within 1v to 1.3v
                        if(ADC_Read(0) < 266 && ADC_Read(0) > 204){
//Checking if the voltage level is due to a very close distance or a very large distance
                                SetSpeed(0,0);
                                DelayMs(50);
//get an average of 20 readings just in case there are some liars. Don't really know if this actuall helps
                                for (i = 0; i < 20; i++) {
                                        initial += ADC_Read(0);
                                }
                                initial = initial/20;
                                MoveDistance(20, 200);
                                for (i = 0; i < 20; i++) {
                                        final += ADC_Read(0);
                                }
                                final = final/20;
//Flip
                                if (initial > final) {
                                        Stop();
                                        DelayMs(50);
                                        MoveDistance(50, -200);
                                        TurnAngle(180, 200);
                                } else {
                                        TurnAngle(30, 200);
                                }
                        }
//Medium distance case
                        else{
//Ocillates wheel speed to achieve maximum view
                                //wspeed = (dist/450) *100;
                                wspeed = 150;
                                if (count > 500) {
                                        DelayMs(1);
                                        SetSpeed(wspeed, 200);
                                }
                                else {
                                        DelayMs(1);
                                        SetSpeed(200, wspeed);
                                }
                        }
                }
//If the sensor isnt picking up anything go forward ocillating
                else{
                        wspeed = 150;
                        if (count > 500) {
                                DelayMs(1);
                                SetSpeed(wspeed, 200);
                        }
                        else {
                                DelayMs(1);
                                SetSpeed(200, wspeed);
                        }
                }
//Reset count when the count is 1000
                if (count > 1000) count = 0;
//Increment count as long as the robot is not doing a sharp turn
                if (wspeed > 100) count++;
        }
}

double Dist_mm(void) {
        int value = ADC_Read(0);
        double distance = 66584*(pow(value, -1.049));
        return distance;
}

void ZigZag(int speed, int sections)
{
        int i;
        int dist = 150;
        for (i = 0; i < sections; i++) 
        {
                MoveDistance(dist, speed);
                DelayMs(250);
                TurnAngle(-90, speed);
                DelayMs(250);
                MoveDistance(dist, speed);
                DelayMs(250);
                TurnAngle(90, speed);
                DelayMs(250);
                MoveDistance(dist, speed);
                DelayMs(250);
                TurnAngle(90, speed);
                DelayMs(250);
                MoveDistance(dist, speed);
                DelayMs(250);
                TurnAngle(-90, speed);
                DelayMs(250);
                dist += 50;
        }
        Stop();
}

void TurnArcRight(float radius_mm, int speed, int angle){
        //const double pi = 3.14159265359;
        float angle_rad = (angle * 2 * pi)/360;
        float left_dist = (radius_mm + 150) * angle_rad;
        float right_dist = radius_mm * angle_rad;
        float left_speed = speed;
        float time = left_dist / left_speed;
        float right_speed = right_dist / time;
        SetSpeed(left_speed, right_speed);
        time *= 1000;
        DelayMs(time);
        Stop();
}

void TurnArcLeft(float radius_mm, int speed, int angle){
        
        float angle_rad = (angle * 2 * pi)/360;
        float right_dist = (radius_mm + 150) * angle_rad;
        float left_dist = radius_mm * angle_rad;
        float right_speed = speed;
        float time = right_dist / right_speed;
        float left_speed = left_dist / time;
        SetSpeed(left_speed, right_speed);
        time *= 1000;
        DelayMs(time);
        Stop();
}

void MoveDistance(float distance_mm, int speed){
        float time = (distance_mm/speed) * 1000;
        time = (int)time;
        if (time < 0) 
                time *= -1;
        SetSpeed(speed,speed);
        DelayMs(time);
        Stop();
}

void MoveTime(int power, unsigned int time) {
}

void TurnTime(int power, unsigned int time) {
}

void TurnAngle(float angle, int speed) {
        //time : circumference of robots turning circle * how much out of 360 to turn / speed
        float time = ((495 * (angle/360)) / speed) *1000;  
        if (time < 0){
                time *= -1;
                SetSpeed(-speed,speed);
        } 
        else {
                SetSpeed(speed,-speed);
        }
        DelayMs(time);
        Stop();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// == Motor Fucntions ==
//
// Abstracted functions for motor control.
//
// SetPower(int left_power, int right_power)
//              Purpose:        set motor output as percentage power
//              Input:          left_power: value from -100 -> 100 for left motor power
//                                      right_power: value from -100 -> 100 for right motor power
//              If the power is set too low the motor will not turn and just whine
//              This is probably not good for it.
//              This usually happens below around 30% power but it varies for each wattbot
//
// SetSpeed(int left_speed, int right_speed)
//              Purpose:        set motor speed in meters per second
//              Input:          left_speed: speed in m/s the left motor should run
//                                      right_speed: speed in m/s the right motor should run
//              This function required calibration for an individual wattbot in order to work correctly!!
//
// SetDirection(DIRECTION left_motor, DIRECTION right_motor)
//              Purpose:        set motor direction
//              Input:          left motor direction enum
//                                      right motor direction enum
//              enums:          FORWARD: motor drives forwards
//                                      BACKWARD: motor drives backwards
//                                      CONTINUE: motor direction doesn't change (use to change only one motor)
//
//      Stop
//              Purpose:        this kills the motors
//
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void SetPower(float left_power, float right_power) {
        DIRECTION left_direction, right_direction;

        // check for a negative value and make the motor drive backwards
        if (left_power < 0) {
                left_direction = BACKWARD;
                left_power = -left_power;
        } else {
                left_direction = FORWARD;
        }
        
        if (right_power < 0) {
                right_direction = BACKWARD;
                right_power = -right_power;
        } else {
                right_direction = FORWARD;
        }

        // we found the motor directions so now we set them
        SetDirection(left_direction, right_direction);

        // the maximum dutycycle is 500, so scale by 5
        left_power = (left_power * 5) + 0.5;
        right_power = (right_power * 5) + 0.5;
        left_power = (int)left_power;
        right_power = (int)right_power;
        SetDutyCyclePWM2(left_power);
        SetDutyCyclePWM1(right_power);

}

void SetSpeed(int left_speed, int right_speed) {
        float left_power, right_power;

        // left_power is some function of left_speed
        left_power = 0.4481*left_speed+3.3247;
        // right_power is some function of right_speed
        right_power = 0.45*right_speed+3.2634;

        // you should calibrate your wattbot before using SetSpeed
        // modify the values of left_motor and right_motor 
        // before the PWM functions are called here
        SetPower(left_power, right_power);

}

void SetDirection(DIRECTION left_direction, DIRECTION right_direction) {
        if(left_direction != CONTINUE)                                                                  // this will keep the previous motor direction
                PORTBbits.RB5 = (left_direction == FORWARD) ? 0 : 1;            // this is a conditional operator or 'ternary if'
        if(right_direction != CONTINUE)                                                                 // do it again for the right motor
                PORTBbits.RB4 = (right_direction == FORWARD) ? 0 : 1;
}

void Stop(void) {
        SetDutyCyclePWM2(0);    // basically just set both motor speeds to 0.
        SetDutyCyclePWM1(0);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// == Delay Routines ==
//
// Designed by Shane Tolmie of KeyGhost corporation.  Freely distributable.
// Questions and comments to shane@keyghost.com
//
// DelayUs(UINT8 uS_value)
//              Purpose:        Microseconds delay
//              Inputs:         uS_value        -       number of microseconds to delay for (0 to 255)
//
// DelayMs(UINT16 cnt)
//              Purpose:        Milliseconds delay (not Megasecond!)
//              Inputs:         cnt                     -       number of miliseconds to delay for (0 to 65535)
//
// DelayS(UINT8 cnt)
//              Purpose:        Seconds delay
//              Inputs:         cnt                     -       number of seconds to delay for (0 to 255)
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

#if PIC_CLK == 40000000
    void DelayUs(UINT8 uS_value) {
                // delay for 10 instructions, 1 / (4e7/4 * 10) = 1us. defined in delay.h
        Delay10TCYx(uS_value);
    }
#elif PIC_CLK == 20000000
    void DelayUs(UINT8 uS_value) {
        Delay10TCYx(uS_value >> 1);
    }
#else
        #error delay.h - please define PIC_CLK correctly
#endif

void DelayMs(UINT16 cnt) {
        UINT8   i;
        do {
                i = 4;
                do {
                        DelayUs(250);
                } while(--i);
        } while(--cnt);
}

void DelayS(UINT8 cnt) {
        UINT8 i;
        do {
                i = 4;
                do {
                        DelayMs(250);
                } while(--i);
        } while(--cnt);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// == PWM == 
// 
// Set-up and control of PWM channels for setting motor speed
//
// OpenBoth_PWM(UINT8 period)
//              Purpose:        configure two PWM channels
//              Input:          period default is 124 -> 12.5us
//
// SetDutyCyclePWM1(UINT16 dutycycle)
//              Purpose:        set the duty of the first PWM channel (4585 = pin C2/17)
//              Input:          duty is calculated from the lowest 10 bits
//
// SetDutyCyclePWM2(UINT16 dutycycle)
//              Purpose:        set the duty of the first PWM channel (4585 = pin D4/27)
//              Input:          duty is calculated from the lowest 10 bits
//
// Period = (PR2 + 1 (125)) * 4 * Tosc (25ns) * TMR2PR (1)
// Duty Cycle = dutycycle * Tosc (25ns) * TMR2PR (1)
// 
// The maximum duty cycle is then 
//              dutycycle = (PR2 + 1 (125)) * 4 = 500
//
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void OpenBoth_PWM(UINT8 period) {
    CCP1CON=0b00001100;      // set capture/compare/pwm module 1 to PWM  mode
    T2CONbits.TMR2ON = 0;    // STOP TIMER2 registers to POR state
    PR2 = period;            // set period
    TRISCbits.TRISC2 = 0;    // configure pin 2 of PORTC as output

    ECCP1CON=0b00001100;     // set enhanced capture/compare/pwm module 1 to PWM mode

#if defined(__18F452)
    TRISCbits.TRISC1 = 0;    // configure pin 1 of PORTC as output
#endif
#if defined(__18F4585)
    TRISDbits.TRISD4 = 0;    // configure pin 4 of PORTD as output
#endif

    OpenTimer2( TIMER_INT_OFF & T2_PS_1_16 & T2_POST_1_1 );
    T2CONbits.TMR2ON = 1;    // Turn on PWM1  
}

void SetDutyCyclePWM1(UINT16 dutycycle) {
        union PWMDC DCycle;

    DCycle.lpwm = dutycycle << 6;       // Save the dutycycle value in the union
    CCPR1L = DCycle.bpwm[1];            // Write the high byte into CCPR1L
    CCP1CON = (CCP1CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);     // Write the low byte into CCP1CON5:4
}

void SetDutyCyclePWM2(UINT16 dutycycle) {
        union PWMDC DCycle;

    DCycle.lpwm = dutycycle << 6;       // Save the dutycycle value in the union

#if defined(__18F452)
    CCPR2L = DCycle.bpwm[1];            // Write the high byte into CCPR2L   
    CCP2CON = (CCP2CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);    // Write the low byte into CCP2CON5:4 
#endif
#if defined(__18F4585)
    ECCPR1L = DCycle.bpwm[1];           // Write the high byte into CCPR1L  
    ECCP1CON = (ECCP1CON & 0xCF) | ((DCycle.bpwm[0] >> 2) & 0x30);   // Write the low byte into CCP1CON5:4  
#endif
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// == ADC ==
// 
// ADC_init
//              Purpose:        configure ADC on AN0 (pin 2)
//
// ADC_Read(UINT8 channel)
//              Purpose:        read & return ADC result
//              Input:          Unless ADCON1<3:0> is changed this must be 0.
//              Output:         10bit result packed as 16bit uint.
//
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void ADC_init(void) {
    ADCON0 = 0;                         // select channel AN0
    ADCON1 = 0b00001110;        // use source voltage / AN0 as analog input
    ADCON2 = 0b10101010;        // (ADC_RIGHT_JUST | ADC_12_TAD | ADC_FOSC_32)
    ADCON0bits.ADON = 1;   // Enable the A/D subsystem
}

UINT16  ADC_Read(UINT8 channel) {
        union {
                UINT8   br[2];
                UINT16  lr;
        } i;

    ADCON0 = ((channel << 2) & 0b00111100) | 0b00000001;        // set channel
    ADCON0bits.GO = 1;                                                                          // go!
    while(ADCON0bits.GO != 0) {}                                                        // result is ready when go is cleared

    i.br[0] = ADRESL;                           // Read ADRESL into the lower byte
    i.br[1] = ADRESH;                           // Read ADRESH into the high byte
    return (i.lr);                              // Return the 16-bit integer
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// == init ==
//
// init
//              Purpose:        call set up of subsystems and configure pins for IO
//
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void init(void) {
    OpenBoth_PWM( 124 );                // Configure 2 PWM hardware subsystems for driving motors.
//      SetDCPWM2(0);        // 18f452
        SetDutyCyclePWM1(0);            // Stop the wattbot from running away
    SetDutyCyclePWM2(0);
    TRISBbits.TRISB4 = 0;               // Set pins used to control direction of motors as outputs
    TRISBbits.TRISB5 = 0;
    srand(143);                                 // initialise random number generator
    ADC_init();                                 // Initialise A/D input channel
}
