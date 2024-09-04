#ifndef __SSD1306_TEST_H__
#define __SSD1306_TEST_H__

#include <_ansi.h>
#include <main.h>

_BEGIN_STD_C

void ssd1306_TestBorder(void);
void ssd1306_TestFonts1(void);
void ssd1306_TestFonts2(void);

void ssd1306_TestMenu(char*, char*, Menu_Option*);
void ssd1306_TestIntervalSetting(int*, unsigned int*);
void ssd1306_TestDurationSetting(int*, unsigned int*);
void ssd1306_TurnOff(void);

void ssd1306_TestFPS(void);
void ssd1306_TestAll(void);
void ssd1306_TestLine(void);
void ssd1306_TestRectangle(void);
void ssd1306_TestRectangleFill(void);
void ssd1306_TestRectangleInvert(void);
void ssd1306_TestCircle(void);
void ssd1306_TestArc(void);
void ssd1306_TestPolyline(void);
void ssd1306_TestDrawBitmap(void);

_END_STD_C

/* USER CODE BEGIN Private defines */

//// DISPLAY SIZE IS 128x64 PIXELS ////

typedef enum {
	HOURS = 0,
	MINUTES = 1,
	SET_INTERVAL = 2
} Time_Inerval_Section;

typedef enum {
	SECONDS = 0,
	SET_DURATION = 1
} Time_Duration_Section;

extern Time_Inerval_Section intervalSection;
extern Time_Duration_Section durationSection;

extern short int render_blinking_span;

/* USER CODE END Private defines */

#endif // __SSD1306_TEST_H__
