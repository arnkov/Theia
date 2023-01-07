#include "ofMain.h"
#include "ofApp.h"
#include "guiApp.h"

//========================================================================
int main( ){

	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(600, 600);
	settings.setPosition(glm::vec2(300, 20));
	settings.resizable = true;
	auto mainWindow = ofCreateWindow(settings);

	settings.setSize(300, 300);
	settings.setPosition(glm::vec2(20, 20));
	settings.resizable = true;
	settings.shareContextWith = mainWindow;
	auto guiWindow = ofCreateWindow(settings);

	shared_ptr<ofApp> mainApp(new ofApp);
	shared_ptr<GuiApp> guiApp(new GuiApp);
	mainApp->gui = guiApp;

	ofRunApp(guiWindow, guiApp);
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();

}
