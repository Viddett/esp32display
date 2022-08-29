
// Optional: Show CPU usage and FPS count
// #define LV_USE_PERF_MONITOR 1

#include <TFT_eSPI.h> // Display parallel driver
#include <lvgl.h> // Graphics driver
#include <TouchManager.hpp> // Touch
#include <string.h>
#include <defines.h>
#include <WiFiManager.hpp> // WiFi
#include <Log.hpp>

TFT_eSPI tft = TFT_eSPI();
TouchManager _touchManager;
lv_indev_drv_t _lvInDevDrv;

WiFiManager _wifiManager;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    uint32_t wh = w*h;

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    while (wh--) tft.pushColor(color_p++->full);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

lv_obj_t *label;
lv_obj_t *_buttonTest;
lv_obj_t *_buttonTest2;

static void button_event(lv_event_t * event) {
   switch (event->code)
   {
   case LV_EVENT_CLICKED:
      Log() << "Button Clicked";
      break;
      case LV_EVENT_SHORT_CLICKED:
      Log() << "Button Clicked Short";
      break;
      case LV_EVENT_PRESSING:
      //Log() << "Button Pressing";
      break;
      case LV_EVENT_PRESS_LOST:
      Log() << "Button Press Lost";
      break;
      case LV_EVENT_LONG_PRESSED:
      Log() << "Button Long Press";
      break;
      case LV_EVENT_RELEASED:
      Log() << "Button Released";
      break;
   default:
      //Log() << "Unknown button event: " << event->code;
      break;
   }
}

void touchCb(lv_indev_drv_t * drv, lv_indev_data_t * data) {
   _touchManager.poll(drv, data);
}

void setup()
{
   // Init serial
   Serial.begin(115200);
   Log() << "ESP32 started";

   // Connect wifi
   _wifiManager.setup();

   // Init TFT_eSPI driver
   tft.begin();
   tft.setRotation(ROTATION);
   tft.fillScreen(TFT_BLACK);

#ifdef GFX_BL
   pinMode(GFX_BL, OUTPUT);
   digitalWrite(GFX_BL, HIGH);
#endif

   // Init graphics
   lv_init();

   uint32_t screenWidth = DISPLAY_WIDTH;
   uint32_t screenHeight = DISPLAY_HEIGHT;
   
   disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 10, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

   if (!disp_draw_buf)
   {
      Log() << "LVGL disp_draw_buf allocate failed!";
   }
   else
   {
      lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * 10);

      /* Initialize the display */
      lv_disp_drv_init(&disp_drv);
      /* Change the following line to your display resolution */
      disp_drv.hor_res = screenWidth;
      disp_drv.ver_res = screenHeight;
      disp_drv.flush_cb = my_disp_flush;
      disp_drv.draw_buf = &draw_buf;
      lv_disp_drv_register(&disp_drv);
   
      // Init input device driver
      _touchManager.setup(ROTATION);
      lv_indev_drv_init(&_lvInDevDrv);
      _lvInDevDrv.type = LV_INDEV_TYPE_POINTER;
      _lvInDevDrv.read_cb = touchCb;
      lv_indev_drv_register(&_lvInDevDrv);

      /* Create simple label */
      label = lv_label_create(lv_scr_act());
      lv_label_set_text(label, "LVGL label");
      lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

      /* Add test button */
      _buttonTest = lv_btn_create(lv_scr_act());
      lv_obj_align(_buttonTest, LV_ALIGN_TOP_LEFT, 20, 20);
      lv_obj_add_event_cb(_buttonTest, button_event, LV_EVENT_ALL, NULL);
      lv_obj_set_content_height(_buttonTest, 40);
      lv_obj_set_content_width(_buttonTest, 40);

      _buttonTest2 = lv_btn_create(lv_scr_act());
      lv_obj_set_content_height(_buttonTest2, 40);
      lv_obj_set_content_width(_buttonTest2, 40);
      lv_obj_align(_buttonTest2, LV_ALIGN_BOTTOM_RIGHT, 20 - 40, 20 - 40);
      lv_obj_add_event_cb(_buttonTest2, button_event, LV_EVENT_ALL, NULL);

      Log() << "Setup done";
   }
}

String _currentTime;
void loop()
{
   _wifiManager.poll();
   String newTime = _wifiManager.getCurrentTime();
   if (!_currentTime.equals(newTime)) {
      _currentTime = newTime;
      lv_label_set_text(label,_currentTime.c_str());
   }


   lv_timer_handler(); /* let the GUI do its work */
   delay(5);
}

