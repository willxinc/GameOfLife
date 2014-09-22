#if __GNUC__ == 4
#include "main.h"
int main(int argc, char** argv){
    // Random Seed
    std::srand(std::time(0));
    LifeGrid lifegrid;
    char filename[256];
    
    XAllegro *xallegro;
    try{
        xallegro = new XAllegro(GRID_WIDTH*GRID_MULT, GRID_HEIGHT*GRID_MULT, 60);
    }catch (int e){
        return e;
    }
	// Black Background
    BITMAP *background = create_video_bitmap(xallegro->width, xallegro->width);
    drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
    rectfill(
        background,
        0,
        0,
        xallegro->width,
		xallegro->height,
        C_BLACK
    );
    
    // Setup game of life control variables
	unsigned long generation = 0;
	bool autorun = false;
	bool justToggledAR = false;
	int nextToggleCounter;
	int stepSpeed = 4;
	int stepTicks = 1;
	
    TRACE("Game Loop Beginning\n");
    TRACE("--------\n");

    while (!key[KEY_ESC]){
        if (xallegro->tick_counter){
            while (xallegro->tick_counter){
                // CALCULATIONS HERE
                // Keyboard commands:
                // - L: Load
                // - S: Save
                // - C: Clear
                // - R: Random
                // - 1-9: Speed
                // - A / P / 0 / Space: Pause
                // - Enter: Manual step when paused

                if (key[KEY_L]){
				    // Pause allegro while loading due to the async nature of the timers
				    xallegro->isPaused = true;

				    strcpy(filename, "");
					loadDialog(filename);
					// filename will be '\0' if nothing is entered
					if (filename[0] != '\0'){
					    try{
					    	lifegrid.load(filename);
					    }catch (int e){
					        return e;
					    }
						generation = 0;
					}
				    xallegro->isPaused = false;
                }
                if (key[KEY_S]){
				    // Pause allegro while loading due to the async nature of the timers
				    xallegro->isPaused = true;
				    
				    strcpy(filename, "");
					saveDialog(filename);
					// filename will be '\0' if nothing is entered
					if (filename[0] != '\0'){
					    try{
					    	lifegrid.save(filename);
					    }catch (int e){
					        return e;
					    }
					}
				    xallegro->isPaused = false;
                }
                if (key[KEY_C]){
				    for (int y = 0; y < GRID_HEIGHT; ++y){
				        for (int x = 0; x < GRID_WIDTH; ++x){
				        	lifegrid.set(x, y, false);
				        }
				    }
					generation = 0;
                }
				if (key[KEY_R]){
					// 33 / 100 filled
					lifegrid.random(33);
					generation = 0;
				}
                for (int kc = KEY_1; kc <= KEY_9; ++kc){
                	if(key[kc]){
                		// 2, 4, 6 ... 16, 18
                		stepSpeed = (kc - KEY_0)*2;
						autorun = true;
                	}
                }
                if ((key[KEY_A] || key[KEY_P] || key[KEY_0] || key[KEY_SPACE]) && !justToggledAR){
					autorun = !autorun;
					justToggledAR = true;
                }
                if (!key[KEY_A] && !key[KEY_P] && !key[KEY_0] && !key[KEY_SPACE]){
					justToggledAR = false;
            	}
            	
            	// Next step if paused and you hit enter
            	// Counter in order to provide stepping as opposed to executing once every frame
            	if (!autorun ){
	            	if ((key[KEY_ENTER]) && nextToggleCounter == 0){
	            		stepTicks = 0;
	            		nextToggleCounter = 30;
	            	}
	            }
	            // Reset should be able to occur at any time
            	if ((!key[KEY_ENTER])){
            		nextToggleCounter = 0;
            	}
            	if (nextToggleCounter > 0){
					--nextToggleCounter;
            	}
            	
            	// Mouse
            	// - Left: set block
            	// - Right: Remove block
            	if (mouse_on_screen()){
	            	if (mouse_b & 1){
						lifegrid.set(mouse_x / GRID_MULT, mouse_y / GRID_MULT, true);
	            	}else if (mouse_b & 2){
						lifegrid.set(mouse_x / GRID_MULT, mouse_y / GRID_MULT, false);
	            	}
	            }
				// Update step
                if (stepTicks % (60/stepSpeed) == 0){
	                lifegrid.update();
	                ++generation;
                	stepTicks = 1;
                }else if (autorun){
	                ++stepTicks;
	            }
                xallegro->decrease_counter();
            }
            allegro_gl_set_allegro_mode();  // Setup opengl projection for Allegros
            // Allegro Draw Functions Start Here
                set_trans_blender(0, 0, 0, 255);
                draw_sprite(screen, background, 0, 0);
                
                // Draw Grid
			    for (int y = 0; y < GRID_HEIGHT; ++y){
			        for (int x = 0; x < GRID_WIDTH; ++x){
			        	if (lifegrid.get(x, y)){
						    rectfill(
						        screen,
						        x*GRID_MULT,
						        y*GRID_MULT,
						        x*GRID_MULT + GRID_MULT,
								y*GRID_MULT + GRID_MULT,
						        C_WHITE
						    );
						}
			        }
			    }
			    
			    // Colour in the selected block
            	if (mouse_on_screen()){
				    rectfill(
				        screen,
				        (mouse_x / GRID_MULT)*GRID_MULT,
				        (mouse_y / GRID_MULT)*GRID_MULT,
				        (mouse_x / GRID_MULT)*GRID_MULT + GRID_MULT,
						(mouse_y / GRID_MULT)*GRID_MULT + GRID_MULT,
				        C_BLUE
				    );
            	}

            allegro_gl_unset_allegro_mode();  // End opengl projection for Allegros

            allegro_gl_set_projection();    // Begin regular OpenGL projection
            // GL Draw Functions Start Here
            	// Fix alpha corruption to allegro_gl functions after calling Allegro Draw functions
	        	glEnable(GL_TEXTURE_2D);
	        	glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                
                // GL Accelerated text draw functions
                allegro_gl_printf(xallegro->agl_font, 32, 8, 0, C_RED, "FPS: %-3d / %d (%-3.1f = %-3.1fms)", xallegro->fps, xallegro->game_speed, xallegro->frameCalc, xallegro->frameTime*1000);

                allegro_gl_printf(xallegro->agl_font, GRID_WIDTH*GRID_MULT / 2 - 12*8, GRID_HEIGHT*GRID_MULT - 2*8, 0, C_RED, "Generation: %lu", generation);
                
                allegro_gl_printf(xallegro->agl_font, GRID_WIDTH*GRID_MULT - 32*8, GRID_HEIGHT*GRID_MULT - 8*8, 0, C_RED, "Commands:");
                allegro_gl_printf(xallegro->agl_font, GRID_WIDTH*GRID_MULT - 32*8, GRID_HEIGHT*GRID_MULT - 7*8, 0, C_RED, " (L)oad pattern");
                allegro_gl_printf(xallegro->agl_font, GRID_WIDTH*GRID_MULT - 32*8, GRID_HEIGHT*GRID_MULT - 6*8, 0, C_RED, " (S)ave Pattern");
                allegro_gl_printf(xallegro->agl_font, GRID_WIDTH*GRID_MULT - 32*8, GRID_HEIGHT*GRID_MULT - 5*8, 0, C_RED, " (C)lear Pattern");
                allegro_gl_printf(xallegro->agl_font, GRID_WIDTH*GRID_MULT - 32*8, GRID_HEIGHT*GRID_MULT - 4*8, 0, C_RED, " (R)andom Pattern");
                allegro_gl_printf(xallegro->agl_font, GRID_WIDTH*GRID_MULT - 32*8, GRID_HEIGHT*GRID_MULT - 3*8, 0, C_RED, " (0/P/A/Space)Toggle Autorun");
                allegro_gl_printf(xallegro->agl_font, GRID_WIDTH*GRID_MULT - 32*8, GRID_HEIGHT*GRID_MULT - 1*8, 0, C_RED, " Left/Right Click to set/unset");

                if (autorun){
                	allegro_gl_printf(xallegro->agl_font, 32, 8*5, 0, C_RED, "Speed: %d steps per second", stepSpeed);
                	allegro_gl_printf(xallegro->agl_font, GRID_WIDTH*GRID_MULT - 32*8, GRID_HEIGHT*GRID_MULT - 2*8, 0, C_RED, " (1-9)x2 Speed");
                }else{
                	allegro_gl_printf(xallegro->agl_font, 32, 8*5, 0, C_RED, "Speed: Autorun Disabled", stepSpeed);
                	allegro_gl_printf(xallegro->agl_font, GRID_WIDTH*GRID_MULT - 32*8, GRID_HEIGHT*GRID_MULT - 2*8, 0, C_RED, " (Enter)Proceed Step");
                }
                // Reset alpha fix (glEnable stuff is reset by allegro_mode)
                glBlendFunc(GL_ONE, GL_ZERO);
            allegro_gl_unset_projection();  //End Projection
            
            // Wait for vsync, then flip the buffer to the fully drawn one
            vsync();
            allegro_gl_flip();
            
            // Clear backbuffer and calculate FPS
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            xallegro->calc_fps();
            xallegro->increaseDeltaFps();
        }else{
        	// If at max fps, then wait 1ms, else yield to other process (don't wait)
        	if (xallegro->fps == xallegro->game_speed){
	            rest(1);
	        }else{
				rest(0);
	        }
        }
    }
	return 0;
}
END_OF_MAIN();
#else
    #error GCC VERSION is not 4!
#endif
