#ifndef HotStepper_h
#define HotStepper_h
#include "Arduino.h"


#define FORWARD 1
#define BACKWARD 0

#define TIMER1INT 1
#define TIMER2INT 2

#ifndef HOTSTEPPER_TIMER1
  #define HOTSTEPPER_TIMER2
#endif


class HotStepper {
  public:
    HotStepper(volatile uint8_t* port, byte offset);
    static void setup();
    static void setup(char timer);
    void instanceSetup();
    void turn(long steps, byte direction);
    void continuousTurn(byte direction);
    boolean ready();
    long remaining();
    void release();
    static void triggerTop();
    void pause();
    void resume();
    void stop();
    long currentPos();
    void returnToZero();
    void trigger();
  private:
    static HotStepper *firstInstance;
    HotStepper *nextInstance;
    void addNext(HotStepper *ref);
    boolean _paused;
    volatile long _currentPos;
    volatile uint8_t* _port;
    byte _pinmask;
    volatile long _remaining;
    byte _dir;
    byte _continuousTurn;
    byte nextStep();
    void setStep(byte);
    void setNextStep();
    byte pad(byte, byte);
    byte unpad(byte, byte);
};

#ifndef FROM_LIB
#ifdef HOTSTEPPER_TIMER1
ISR(TIMER1_COMPA_vect)
{
  HotStepper::triggerTop();
}
#endif

#ifdef HOTSTEPPER_TIMER2
ISR(TIMER2_COMPA_vect)
{
  HotStepper::triggerTop();
}
#endif
#endif

#endif

