#ifndef _LIFE_H
#define _LIFE_H 1

#include <fstream>
#include <iostream>

// To fix missing std in apmatrix
using namespace std;
#include "APMATRIX.H"

// If not previously defined, then setu pdefault sizes and scale
#ifndef GRID_WIDTH
#define GRID_WIDTH 50
#endif
#ifndef GRID_HEIGHT
#define GRID_HEIGHT 20
#endif
#ifndef GRID_MULT
#define GRID_MULT 16
#endif

// Some quick macros
#define min(a,b) ((a<b) ? a : b)
#define max(a,b) ((a>b) ? a : b)


/*
	Contains an apmatrix which represents the cells
*/
class LifeGrid{
    // private:
	    apmatrix<bool> cell;
    public:
        LifeGrid();
		void load(const char* fname);
		void save(const char* fname);
		void random(int density);
		
        void update();
        int count9(int x, int y);
        bool get(int x, int y);
        void set(int x, int y, bool state);
};


#endif
