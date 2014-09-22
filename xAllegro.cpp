#include "xAllegro.h"
// A function which does nothing
void nothing(){}
END_OF_FUNCTION(nothing)

XAllegro::XAllegro(int w, int h, int speed) :
	_fpsDelta(0), _tick_counter(0), _fps(speed), _game_speed(speed), _width(w), _height(h),
	tick_counter(_tick_counter), fps(_fps), game_speed(_game_speed), width(_width), height(_height), isPaused(false), frameTime(0.f), frameCalc(0.f)
{
// Begin Constructor
    TRACE("Allegro Initializing\n");
    if (allegro_init() != 0){
        TRACE("Allegro Error\n");
        TRACE(allegro_error);
        throw 1;
    }
    TRACE("AllegroGL Initializing\n");
    if (install_allegro_gl() != 0){
        TRACE("AllegroGL Error\n");
        TRACE(allegro_gl_error);
        throw 2;
    }

    TRACE("Installing Timer\n");
    if (install_timer() != 0){
        TRACE("Allegro Timer Install Error\n");
        TRACE(allegro_error);
        throw 3;
    }
    LOCK_VARIABLE(tick_counter);
    LOCK_FUNCTION(_increment_tick_counter);
    TRACE("Installing Tick Timer\n");
    if (install_param_int_ex(_increment_tick_counter, this, BPS_TO_TIMER(speed)) != 0){
        TRACE("Allegro Tick Timer Install Error\n");
        TRACE(allegro_error);
        throw 4;
    }

    LOCK_VARIABLE(fps);
    LOCK_VARIABLE(fpsDelta);
    LOCK_FUNCTION(_delta_fps);
    // install_int_ex(delta_fps, BPS_TO_TIMER(1));
    TRACE("Installing FPS Counter Timer\n");
    if (install_param_int_ex(_delta_fps, this, BPS_TO_TIMER(1)) != 0){
        TRACE("Allegro FPS Counter Timer Install Error\n");
        TRACE(allegro_error);
        throw 5;
    }

    TRACE("Installing Keyboard\n");
	if (install_keyboard() != 0){
        TRACE("Allegro Keyboard Install Error\n");
        TRACE(allegro_error);
        throw 6;
    }
		
    TRACE("Installing Mouse\n");
    if (install_mouse() < 0){
        TRACE("Allegro Mouse Install Error\n");
        TRACE(allegro_error);
        throw 7;
    }
    show_os_cursor(MOUSE_CURSOR_ARROW);

    LOCK_VARIABLE(key[KEY_ESC]);
    LOCK_FUNCTION(_close_button);
    set_close_button_callback(_close_button);
    set_window_title("Game of Life");

    TRACE("Initializing Graphics Mode\n");

    allegro_gl_clear_settings();
	allegro_gl_set(AGL_COLOR_DEPTH, 32);
  	allegro_gl_set(AGL_Z_DEPTH, 24);
	allegro_gl_set(AGL_DOUBLEBUFFER, 1);
	allegro_gl_set(AGL_WINDOWED, TRUE);
	allegro_gl_set(AGL_RENDERMETHOD, 1);
    allegro_gl_set(AGL_REQUIRE, AGL_COLOR_DEPTH | AGL_DOUBLEBUFFER | AGL_RENDERMETHOD | AGL_WINDOWED);

	set_color_depth(32);
    if(set_gfx_mode(GFX_OPENGL,w, h, 0, 0) != 0){
        allegro_message ("Error setting OpenGL graphics mode:\n%s\nAllegro GL error : %s\n", allegro_error, allegro_gl_error);
        throw 1;
    }else{
        TRACE("OpenGL Version: %f\n",allegro_gl_opengl_version());
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    TRACE("Converting Font\n");
    agl_font = allegro_gl_convert_allegro_font(font, AGL_FONT_TYPE_TEXTURED, -1);

    TRACE("Setting switch modes\n");
    set_display_switch_mode(SWITCH_BACKGROUND);
    LOCK_FUNCTION(nothing);
    set_display_switch_callback(SWITCH_OUT,nothing);
    set_display_switch_callback(SWITCH_IN, nothing);
// End Constructor
}

XAllegro::~XAllegro(){
    allegro_exit();
}

// Timer functions for fps normalization
void XAllegro::decrease_counter(){
	--_tick_counter;
}
void XAllegro::increment_counter(){
	if (!isPaused)
		++_tick_counter;
}
// Used to calculate average FPS
void XAllegro::increaseDeltaFps(){
	++_fpsDelta;
}
// Calculate frame time, 1/frametime = instantanious fps
void XAllegro::calc_fps(){
    static clock_t clDelta = clock();
	float newFrameTime = ((float)(clock() - clDelta) / CLOCKS_PER_SEC);
    if (!std::isfinite(newFrameTime) || newFrameTime == 0){
        newFrameTime = 0.033f;
    }
    frameTime = (newFrameTime + (frameTime*game_speed))/(game_speed+1);

    frameCalc = 1 / frameTime;
    clDelta = clock();
}

// static due to how timers work:
// key is a global array that holds the keyboard state
void XAllegro::_close_button(){
    key[KEY_ESC] = true;
}
END_OF_FUNCTION(XAllegro::_close_button)
// Increment the tick counter of the current allegro instance
void XAllegro::_increment_tick_counter(void *v_this){
	XAllegro *_this = static_cast<XAllegro*>(v_this);
    _this->increment_counter();
}
END_OF_FUNCTION(XAllegro::_increment_tick_counter)
// Calculate the delta of the fps (frames displayed inthe last second)
void XAllegro::_delta_fps(void *v_this){
	XAllegro *_this = static_cast<XAllegro*>(v_this);
    _this->_fps = _this->_fpsDelta;
    _this->_fpsDelta = 0;
}
END_OF_FUNCTION(XAllegro::_delta_fps)

