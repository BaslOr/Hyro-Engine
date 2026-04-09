#include <iostream>
#include "Application.h"



Hyro::Application* CreateApplication();

int main(int argc, char** argv) {
	Hyro::Application* app = CreateApplication();
	app->Run();
	delete app;
}