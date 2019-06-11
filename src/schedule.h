/***
** Created by Aleksey Volkov on 2019-04-11.
***/

#ifndef HV_CC_LED_DRIVER_SCHEDULE_H
#define HV_CC_LED_DRIVER_SCHEDULE_H

#include "settings.h"
#include "Ticker.h"

typedef struct {
  uint8_t current_duty;
  uint8_t target_duty;
  uint32_t steps_left;
} led_schedule_t;

class ScheduleClass {

  public:
    void init();
    void refresh();
    char * getCurrentTimeString();
    void loop();
    uint8_t getChannelDuty(uint8_t channel);
    void setChannelDuty(uint8_t duty, uint8_t channel);

    /* Find Next Light Schedule Point */
//    uint8_t getNearestPoint();

  private:
    int minutesLeft(time_t t, unsigned char schedule_hour, unsigned char schedule_minute);
    void updateLed();
    void transition();
    bool process_schedule = true;

    uint32_t toPWM(uint8_t percent);
    uint8_t fromPWM(uint32_t pwm);

    led_schedule_t _leds[MAX_LED_CHANNELS];


};

extern ScheduleClass SCHEDULE;

#endif //HV_CC_LED_DRIVER_SCHEDULE_H