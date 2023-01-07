#pragma once
#include "ofMain.h"
#include "guiApp.h"
#include "ofxLuaaa.h"
#include "ofxPostGlitch.h"
#include "ofxHapPlayer.h"
#include "ofBindings.h"
#include "postProcessing.h"


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void updateGlitch();
	void updateLuaParams();
	void updateUniforms();
	void loadVideo();
	void updatePost();
	void setupLua();
	void loadLuaScript();

	//GuiApp
	shared_ptr<GuiApp> gui;

	//Lua
	ofxLuaaa lua;
	luaDraw ld;
	luaColor cl;
	luaFbo fb;
	luaCam ca;
	luaMath ma;
	luaUtils ut;
	luaMaterial mat;
	luaLight li;
	luaGL gl;

	lua_State* ls = NULL;

	//hapVideo
	ofxHapPlayer player[2];
	const int playerSize = 1;
	ofBlendMode blendMode[5];

	//shaders
	ofxPostGlitch glitch;
	int glitchSize;
	int glitchColSize;

	postProcessing post;

	ofShader soloShader;

	//Fbos
	//drawing stuff inside
	ofFbo fbo[2];
	//only for fake feedback/trails
	ofFbo fbFbo;

	//textBuffers and fonts
	ofBuffer textBuffer;
	ofTrueTypeFont textFont;
	ofTrueTypeFont debugFont;
};
