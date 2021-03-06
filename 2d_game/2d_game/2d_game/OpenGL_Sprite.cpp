//Ragul S
//CS426 Assignment 3
//17250319

#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>    // Required for sound only (consider OpenAL), 
#include <mmsystem.h>   // Again only required for sound only
#include <freeglut.h>   // GLUT OpenGL Utility Toolkit
#include "Sprite.h"     // Prototype Sprite class

Sprite player;
Sprite projectile;
Sprite alien;
Sprite alienProjectile;
bool alien_move = true;
Sprite background;

// Score data and method
int score = 0;
char score_text[20];


// Callback function called each time screen is to be redrawn (called by OS not you directly)
static void display(void)
{															// Keep game logic out of display()...
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and the Depth Buffer

	background.drawSprite();							// Draw background at X,Y
	player.drawSprite();								// Draw player	
	alien.drawSprite();
	alienProjectile.drawSprite();
	projectile.drawSprite();

	sprintf_s(score_text, "Scr: %d", score);              // Display text 
	Sprite::text(5,20, score_text);

	glutSwapBuffers();  // Display buffer just drawn and then switch to draw on the other buffer
}


// Callback function to allow dynamic resizing of runtime display
static void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	Sprite::spriteModeOn(0, 0);  // (0,0) Causes orginal screen size to be reused
}

// Callback to manage standard keys on the keyboard
// In this case 'q' or 'Q' causes program to terminate
static void qwerty_keys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'Q':
	case 'q': glutLeaveMainLoop(); break;  // (Stop!)
	default: break;
	}
}

// Callback for cursor keys on the keyboard
// <- and -> cursors keys cause player to move left and right
static void function_keys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		player.sprite_x_position -= 10;
		break;


	case GLUT_KEY_RIGHT:
		player.sprite_x_position += 10;
		break;

	case GLUT_KEY_UP:
		if (projectile.sprite_y_position > 999) {
			projectile.sprite_x_position = player.sprite_x_position +60;
			projectile.sprite_y_position = player.sprite_y_position +120;
			
		} 
		
	default: break;
	}
}

// Game loop
// static void idle()    // if using  glutIdleFunc(idle); this line should replace the one below


static void idle(int v)  // Runs between frame updates in doublebuffer mode, game logic goes here
{
	
	projectile.sprite_y_position += 15;
	int random = rand() % 500 + 5;
	

	if (alien_move) {
		alien.sprite_y_position -= 10;
		if (alien.sprite_y_position < 10) {
			score = 0;
			alien.load((char*)"aliensheet.bmp", 9, 1, random, 1000, 150, 150, false);
			alien_move = false;
		}
	}
	else {
		alien.sprite_y_position -= 10;
		if (alien.sprite_y_position <10) {
			alien.load((char*)"aliensheet.bmp", 9, 1, random, 1000, 150, 150, false);
			score = 0;
			alien_move = true;
		}
	}

	

	if (alien.collision(projectile)) {
		
		alien.load((char*)"aliensheet.bmp", 9, 1, random, 1000, 150, 150, false);
		score += 1;
		projectile.sprite_y_position = 1500;
	}
	alien.advance_frame(0, 8);

	glutPostRedisplay();      // Send message to ask for screen to be redrawn
	glutTimerFunc(100, idle, 0); // Restart the timer (remove line if using the more common glutIdleFunc(idle) approach)
}

/* Program entry point - starts here */
int main(int argc, char *argv[])
{
	Sprite::spriteModeOn(640, 1024);		// Create a screen 640x480 to contain game

											// Start playing background music (uses Windows calls, there are other ways incluing OpenAL)
	PlaySound(L"music.wav", NULL, SND_LOOP | SND_ASYNC);

	// load sprite files (24bit BMP), Frames (X,Y), position (X,Y), size (Width,Height), direction
	// Use sprites with Width and Height divisible by 4, ideally powers of two e.g. 256x128

	player.load((char*)"spaceship.bmp", 1, 1, 320, 10, 150, 150, false);
	background.load((char*)"background.bmp", 1, 1, 0, 0, 640, 1024, false);
	alien.load((char*)"aliensheet.bmp", 9, 1, rand()%550, 1000, 150, 150, false);
	projectile.load((char*) "fire.bmp", 1, 1, 1000, 1000, 40, 40, false);
	

	glutReshapeFunc(resize);			// Called when window is resized (not required)
	glutDisplayFunc(display);			// Callback function to redraw, an event created by glutPostRedisplay()
	glutKeyboardFunc(qwerty_keys);		// QWERTY Keys
	glutSpecialFunc(function_keys);		// Cursor keys etc 

										//glutIdleFunc(idle);			    // Called during idle time 
	glutTimerFunc(10, idle, 0);           // Use timer callback for more consistent timing

	glutMainLoop();						// Start main loop running (go!)

	return EXIT_SUCCESS;
}

