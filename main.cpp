#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include<stdio.h>

// Basic movement, with double jumping and friction.

int h;
int w;

//Uitility namespace
namespace util{

	// Return the sign of the input
	int sgn(float f){
		if(f < 0) return -1;
		if(f > 0) return 1;
		return 0;
	}
}

class d : public olc::PixelGameEngine
{
public:
	d()
	{
		sAppName = "2d Movement";
	}

public:
	bool OnUserCreate() override
	{
		// Some window parameters
    h = ScreenHeight();
    w = ScreenWidth();
		return true;
	}

private:
	// Simulation parameters, can be adjusted.

	float g = 150.0f;
	float speedcap = 90.0f;

	// friction coefficient
	float mu = 1.5f;

	// The impulse, or deltaV imparted, both horizonatally and verrtically
	float impulse = 80.0f;
	float vimpulse = 100.0f;

	// For double jumping
	bool canJump = false;

	// Bat state variables
	float ay = 1.0f;
	float ax = 0.0f;
	float fBatX = 100.0f;
	float fBatY = 100.0f;
	float fBatVX = 0.0f;
	float fBatVY = 0.0f;

public:

	bool OnUserUpdate(float fElapsedTime) override
	{
    Clear(olc::DARK_BLUE);

		// Keyboard input, with velocity caps built in.
    if(GetKey(olc::Key::RIGHT).bPressed){
      fBatVX += impulse;
      if(fBatVX > speedcap){
        fBatVX = speedcap;
      }
    }

    if(GetKey(olc::Key::LEFT).bPressed){
      fBatVX -= impulse;
      if(fBatVX < -1 * speedcap){
        fBatVX = -1 * speedcap;
      }
    }

    if(GetKey(olc::Key::UP).bPressed){
			if(canJump){
				// Allows double jumping
				if(fBatY > h - 31.0f){
					 ax = 0;
		       fBatY -= 4.0;
		       fBatVY -= vimpulse;
	     	}
				else{
					ax = 0;
					fBatY -= 4.0;
					fBatVY -= vimpulse - 20.0f;
					canJump = false;
					fBatVX = 0.1 * util::sgn(fBatVX);
				}
			}
		}

		// Drawing all objects, including any walls, players etc.
    DrawLine(10, h - 10, w-10, h-10);
		DrawLine(10, 0, 10, h-10);
		DrawLine(w - 10, 0, h-10, w-10);
    DrawRect(int(fBatX), int(fBatY), 20, 20, olc::GREEN);

		// The double jumping breaks if you dont have this. Eek.
		if(!canJump){
			ax = 0;
		}

		// Checking collitions with floor
    if(fBatY >= h - 30.0f){
			// If the floor is in contact
      ay = 0;
      fBatVY = 0;
      fBatY = h - 30.0f;
			canJump = true;

			// Friction mechanic, makes very slow movenent impossible as well. (Very slow movement looks glitchy)
      if(fBatVX < -0.1 || fBatVX > 0.1){
        ax = -1 * mu * fBatVX;
      }
      else{
        ax = 0;
        fBatVX = 0;
      }
    }
    else{
			// Imposes speedcap in air
      ay = g;
      ax = 0;
      if(fBatVX > 120 || fBatVY < -120){
        fBatVX = util::sgn(fBatVX) * 120;
      }
    }

		// Wall collisions, causing bouncing
		if(fBatX < 10){
			fBatX = 10;
			fBatVX *= -1;
		}
		if(fBatX > w - 30){
			fBatX = w-30;
			fBatVX *= -1;
		}

		// Updating position and velocity
    fBatX += fBatVX * fElapsedTime;
    fBatY += fBatVY * fElapsedTime;

    fBatVX += ax * fElapsedTime;
    fBatVY += ay * fElapsedTime;

    return true;
    }

  };

// Boilerplate
int main()
{
	d demo;
	if (demo.Construct(500, 500, 1, 1))
		demo.Start();

	return 0;
}
