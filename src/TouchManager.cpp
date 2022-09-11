#include <TouchManager.hpp>
//#define DEBUG_TOUCH

void TouchManager::setup(uint8_t rotation)
{
    ts.begin();
    ts.setRotation(rotation);
}

void TouchManager::poll(lv_indev_drv_t *indevDrv, lv_indev_data_t *data)
{
    bool isTouched = /*ts.tirqTouched() &&*/ ts.touched(); // TODO: remove irq? (raplace with BL pin?)

    data->state = isTouched ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;

    if (isTouched)
    {
        TS_Point p = ts.getPoint();
        int x = (1 - (p.x - AdXmin) / (float)(AdXmax - AdXmin)) * DISPLAY_WIDTH;
        int y = (1 - (p.y - AdYmin) / (float)(AdYmax - AdYmin)) * DISPLAY_HEIGHT;
        x = rangeLim(x, 0, DISPLAY_WIDTH);
        y = rangeLim(y, 0, DISPLAY_HEIGHT);

        data->point.x = x;
        data->point.y = y;

        // delay(30);

#ifdef DEBUG_TOUCH
        Log() << "Pressure:" << p.z << "x =" << p.x << "y =" << p.y << "xc =" << x << "yc =" << y;
#endif
    }
}