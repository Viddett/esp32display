#ifndef TOUCHMANAGER_HPP
#define TOUCHMANAGER_HPP
#include <XPT2046_Touchscreen.h>
#include <lvgl.h>
#include <defines.h>
#include <convenience.hpp>
#include <Log.hpp>

#define TOUCH_CS_PIN 21
//#define TOUCH_IRQ_PIN 22 // Removed to free up pin

//inline XPT2046_Touchscreen ts(TOUCH_CS_PIN, TOUCH_IRQ_PIN); // Param 2 - Touch IRQ Pin - interrupt enabled polling
inline XPT2046_Touchscreen ts(TOUCH_CS_PIN); // Param 2 - Touch IRQ Pin - interrupt enabled polling

class TouchManager {

public:
    void setup(uint8_t rotation = 1);
    void poll(lv_indev_drv_t * indevDrv, lv_indev_data_t * data);

private:
    /* Manually calibrated AD limits: */
    static constexpr uint16_t AdXmin = 260;
    static constexpr uint16_t AdXmax = 3900;
    static constexpr uint16_t AdYmin = 280;
    static constexpr uint16_t AdYmax = 3840;
};

#endif