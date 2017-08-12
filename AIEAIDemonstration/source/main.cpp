#include <iostream>
#include <ctime>

#include "Application2D.h"

const char APP_NAME[] = "AI Demonstration";
const int SCREEN_X = 640;
const int SCREEN_Y = 480;

//entry of the program
int main()
{
	srand((unsigned int)time(NULL));

	//create the application
	Application2D* app2D = new Application2D();

	//run the application
	app2D->run(APP_NAME, SCREEN_X, SCREEN_Y, false);

	return 0;
}