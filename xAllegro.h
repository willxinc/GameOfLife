#ifndef _XALLEGRO_H
#define _XALLEGRO_H 1

#include <allegro.h>
#include <alleggl.h>
#include <cmath>

// Callback fix
void nothing();

// Class Definition
class XAllegro{
	public:
		XAllegro(int w, int h, int s);
		~XAllegro();
		void decrease_counter();
		void increment_counter();
		void increaseDeltaFps();
		void calc_fps();

		const volatile long& tick_counter;
		const volatile int& fps;

		const int& game_speed;
		const int& width;
		const int& height;

		bool isPaused;
		float frameTime;
		float frameCalc;
		FONT* agl_font;
	private:
		static void _close_button();
		static void _increment_tick_counter(void *_this);
		static void _delta_fps(void *_this);

		volatile int _fpsDelta;

		volatile long _tick_counter;
		volatile int _fps;

		int _game_speed;
		int _width;
		int _height;
};

//Common Colours assumming 32bit
const int C_BLACK = makeacol_depth(32,0,0,0,255);
const int C_WHITE = makeacol_depth(32,255,255,255,255);
const int C_BLUE = makeacol_depth(32,0,0,255,255);
const int C_RED = makeacol_depth(32,255,0,0,255);

#endif
