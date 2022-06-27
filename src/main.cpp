
// Optional: Show CPU usage and FPS count
// #define LV_USE_PERF_MONITOR 1

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <string.h>


TFT_eSPI tft = TFT_eSPI();


static uint32_t screenWidth;
static uint32_t screenHeight;
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
void setup()
{
   Serial.begin(115200);
   Serial.println("ESP32 started");

   // Init Display
   tft.begin();
   tft.setRotation(1);
   tft.fillScreen( TFT_BLACK );

#ifdef GFX_BL
   pinMode(GFX_BL, OUTPUT);
   digitalWrite(GFX_BL, HIGH);
#endif

   lv_init();

   screenWidth = tft.width();
   screenHeight = tft.height();
   
   disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 10, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

   if (!disp_draw_buf)
   {
      Serial.println("LVGL disp_draw_buf allocate failed!");
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
   
      /* Initialize the (dummy) input device driver */
      static lv_indev_drv_t indev_drv;
      lv_indev_drv_init(&indev_drv);
      indev_drv.type = LV_INDEV_TYPE_POINTER;
      lv_indev_drv_register(&indev_drv);

      /* Create simple label */
      label = lv_label_create(lv_scr_act());
      lv_label_set_text(label, "LVGL label");
      lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

      Serial.println("Setup done");
   }
}

void loop()
{
   static int timeLast = millis();
   static int sec, min, hr = 0;
   int time = millis();
   if (time >= timeLast + 1000) {
      timeLast = time;
      sec++;
      if (sec == 60) {
         sec = 0;
         min++;
      }
      if (min == 60) {
         min = 0;
         hr++;
      }
      String str = "Uptime: " + String( hr ) + ":" + String( min ) + ":" + String( sec );
      lv_label_set_text(label, str.c_str());
   }
   lv_timer_handler(); /* let the GUI do its work */
   delay(5);
}

