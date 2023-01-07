#include "ofApp.h"
#include "ofAppRunner.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetWindowTitle("Theia 0.2");
	ofSetFrameRate(60);
	ofSetBackgroundColor(0);
	ofSetSmoothLighting(true);

	//Setup Lua and load bindings
	//TODO: Add error callbacks and something to check, if the params passed from cpp are actually existing.
	//		For now it's like driving without a seatbelt.
	setupLua();

	//load your script
	std::string currentScript = "scripts/" + gui->luaFileNames[gui->luaFileIndex];
	lua.runScript(ls, currentScript);
	//call a lua function
	lua.fnSetup(ls);

	//video ---- actually doesn't need to be an array anymore, but does it hurt?
	player[0].load("movies/" + gui->movieFileNames[0]);
	player[1].load("movies/" + gui->movieFileNames[0]);

	for (int i = 0; i <= playerSize; i++){
		player[i].setLoopState(OF_LOOP_NORMAL);
	}


	fbo[0].allocate(ofGetWidth(), ofGetHeight());
	fbo[1].allocate(ofGetWidth(), ofGetHeight());

	fbFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);

	//shaders2D
	glitch.setup(&fbo[0]);
	glitchSize = 9;
	glitchColSize = 8;


	//shaders2D
	glitch.setup(&fbo[0]);
	glitchSize = 9;
	glitchColSize = 8;
	post.setup(&fbo[0]);

	soloShader.load("shaders/solo/" + gui->shaderFileNames[gui->shaderFileIndex]);

	blendMode[0] = OF_BLENDMODE_DISABLED;
	blendMode[1] = OF_BLENDMODE_ADD;
	blendMode[2] = OF_BLENDMODE_MULTIPLY;
	blendMode[3] = OF_BLENDMODE_SUBTRACT;
	blendMode[4] = OF_BLENDMODE_ALPHA;

	textFont.load("fonts/ShareTechMono-Regular.ttf", 12, true);
}

//--------------------------------------------------------------
void ofApp::update(){

	if (gui->bFullscreen) {
		ofToggleFullscreen();
		gui->bFullscreen = false;
	}

	if (gui->bReloadSoloShader) {
		soloShader.load("shaders/solo/" + gui->shaderFileNames[gui->shaderFileIndex]);
		gui->bReloadSoloShader = false;
	}

	updateUniforms();
	updateGlitch();
	updatePost();
	loadVideo();

	//LUA
	if (gui->bLua) {
		updateLuaParams();
		lua.fnUpdate(ls);
		if (gui->bReloadLua) {
			loadLuaScript();
		}
	}

	//SPOUT
#ifdef TARGET_WIN32

#endif
}

//--------------------------------------------------------------
void ofApp::draw(){

	//LUA
	if (gui->bLua) {
		fbo[0].begin();
		if (gui->bClearFbo) {
			ofClear(0, 0, 0, 255);
		}
		lua.fnDraw(ls);
		if (gui->bFeedback) {
			ofFill();
			ofSetColor(0, 0, 0, gui->fbValue);
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		}
		if (gui->bShowCode) {
			textBuffer = ofBufferFromFile("scripts/" + gui->luaFileNames[gui->luaFileIndex]);
			string luaText = textBuffer.getText();
			ofSetColor(gui->shaderTextCol);
			textFont.drawString(luaText, 20, 20);
		}
		fbo[0].end();
		glitch.generateFx();
		post.generatePost();
		fbo[0].draw(0, 0);
	}
	
	//VIDEO
	if (gui->bVideo) {
		ofDisableLighting();
		fbo[0].begin();
		if (gui->bClearFbo) {
			ofClear(0, 0, 0, 255);
		}
		//Only draw the first video, if there's some blending going on!
		if (!blendMode[0]) {

			ofSetColor(gui->r1, gui->g1, gui->b1, gui->a1);
			//Scale the Video to the size of the Screen
			ofRectangle videoRect1(0, 0, player[0].getWidth(), player[0].getHeight());
			videoRect1.scaleTo(ofGetWindowRect());
			player[0].draw(videoRect1.x, videoRect1.y, videoRect1.width, videoRect1.height);
			if (player[0].isLoaded()) {
				player[0].play();
			}
		}

		//To be replaced by something with a bigger palette of blend modes
		ofEnableBlendMode(blendMode[gui->blending]);

		//draw the second video
		ofSetColor(gui->r2, gui->g2, gui->b2, gui->a2);
		if (player[1].isLoaded()) {
			ofRectangle videoRect2(0, 0, player[1].getWidth(), player[1].getHeight());
			videoRect2.scaleTo(ofGetWindowRect());
			player[1].draw(videoRect2.x, videoRect2.y, videoRect2.width, videoRect2.height);
			player[1].play();
		}
		if (gui->bFeedback) {
			ofFill();
			ofSetColor(0, 0, 0, gui->fbValue);
			ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		}
		ofDisableBlendMode();
		fbo[0].end();
		glitch.generateFx();
		fbo[0].draw(0, 0);
	}

	/*SPOUT?*/
	//Is going to be back soon. I'm having trouble with resizing textures.

	/*--------------SOLOSHADERS-------------------*/

	if (gui->bSoloShader && soloShader.isLoaded()) {

		soloShader.begin();
		updateUniforms();
		ofSetColor(255);
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
		soloShader.end();

		if (gui->bShowCode) {
			textBuffer = ofBufferFromFile("shaders/solo/" + gui->shaderFileNames[gui->shaderFileIndex] + ".frag");
			string shaderText = textBuffer.getText();
			ofSetColor(gui->shaderTextCol);
			textFont.drawString(shaderText, 20, 20);
		}
	}
}

//--------------------------------------------------------------
void ofApp::exit() {

	lua.fnExit(ls);
	lua.clearLua(ls);
	lua_close(ls);
}

//--------------------------------------------------------------
void ofApp::updateUniforms() {

	/*TODO: Find a nice way to use the same uniforms regardless of using Sound, Midi, or OSC as input*/

	if (gui->bSoloShader) {
		soloShader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
		soloShader.setUniform1f("u_time", ofGetElapsedTimef());
		soloShader.setUniform2f("u_pos", gui->shaderPosX, gui->shaderPosY);
		soloShader.setUniform1f("u_val0", gui->shaderVal0);
		soloShader.setUniform1f("u_val1", gui->shaderVal1);

		if (gui->bSound) {
			soloShader.setUniform3f("u_beat", gui->kick, gui->snare, gui->hat);
			soloShader.setUniform1f("u_rms", gui->smoothRMS * gui->rmsMultiplier);
			soloShader.setUniform1f("u_magnitude", gui->magnitude);
			float u_bang = 0.0;
			if (gui->bang) { u_bang = 1.0f; }
			else { u_bang = 0.0; }
			soloShader.setUniform1f("u_bang", u_bang);
		}
		else if (!gui->bSound && gui->bMIDI) {
			soloShader.setUniform3f("u_beat", gui->kick, gui->snare, gui->hat);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'f') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

	ofClear(0, 0, 0, 255);
	fbo[0].allocate(w, h);
	fbo[1].allocate(w, h);
	fbFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA32F_ARB);
	glitch.setup(&fbo[0]);
	post.setup(&fbo[0]);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::updateGlitch() {
	
	if (gui->bRandGlitch && gui->randomBeat < gui->shaderRandom && (gui->bKick || gui->bang || gui->lowBang > 0.0f)) {
		gui->glitchNum = ofRandom(glitchSize);
	}
	if (gui->bRandColGlitch && gui->randomBeat < gui->shaderRandom && (gui->bKick || gui->bang || gui->lowBang > 0.0f)) {
		gui->glitchCol = ofRandom(glitchColSize);
	}
	
	if (gui->glitchNum == 1) glitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
	else  glitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
	if (gui->glitchNum == 2) glitch.setFx(OFXPOSTGLITCH_SHAKER, true);
	else glitch.setFx(OFXPOSTGLITCH_SHAKER, false);
	if (gui->glitchNum == 3) glitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
	else glitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
	if (gui->glitchNum == 4) glitch.setFx(OFXPOSTGLITCH_TWIST, true);
	else glitch.setFx(OFXPOSTGLITCH_TWIST, false);
	if (gui->glitchNum == 5) glitch.setFx(OFXPOSTGLITCH_OUTLINE, true);
	else  glitch.setFx(OFXPOSTGLITCH_OUTLINE, false);
	if (gui->glitchNum == 6) glitch.setFx(OFXPOSTGLITCH_SLITSCAN, true);
	else glitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
	if (gui->glitchNum == 7) glitch.setFx(OFXPOSTGLITCH_SWELL, true);
	else glitch.setFx(OFXPOSTGLITCH_SWELL, false);
	if (gui->glitchNum == 8) glitch.setFx(OFXPOSTGLITCH_INVERT, true);
	else glitch.setFx(OFXPOSTGLITCH_INVERT, false);

	if (gui->glitchCol == 1) glitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
	else glitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
	if (gui->glitchCol == 2) glitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
	else glitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
	if (gui->glitchCol == 3) glitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
	else glitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
	if (gui->glitchCol == 4) glitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
	else glitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
	if (gui->glitchCol == 5) glitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
	else glitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
	if (gui->glitchCol == 6) glitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
	else glitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
	if (gui->glitchCol == 7) glitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);
	else glitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);
}

//----------------------------------------------------------------------
void ofApp::updatePost() {

	if (gui->bFxaa) { post.setPost(FXAA, true); }
	else { post.setPost(FXAA, false); }
	if (gui->bBloom) {
		post.setPost(BLOOM, true);
		post.setBloomParams(gui->bloomSteps, gui->bloomSeparation, gui->bloomThreshold, gui->bloomAmount);
	}
	else { post.setPost(BLOOM, false); }
}

//--------------------------------------------------------------
void ofApp::setupLua() {

	ls = luaL_newstate();
	luaopen_base(ls);
	ld.bindToLua(ls);
	cl.bindToLua(ls);
	fb.bindToLua(ls);
	ca.bindToLua(ls);
	ma.bindToLua(ls);
	ut.bindToLua(ls);
	mat.bindToLua(ls);
	li.bindToLua(ls);
	gl.bindToLua(ls);
	lua_settop(ls, 0);
}

//--------------------------------------------------------------
void ofApp::loadLuaScript() {

	lua.fnExit(ls);
	lua.clearLua(ls);
	lua_close(ls);
	setupLua();
	std::string currentScript = "scripts/" + gui->luaFileNames[gui->luaFileIndex];
	lua.runScript(ls, currentScript);
	lua.fnSetup(ls);
	gui->bReloadLua = false;
}

//----------------------------------------------------------------------
void ofApp::updateLuaParams() {

	lua.setBool(ls, "bang", gui->bang);
	lua.setNumber(ls, "magnitude", gui->magnitude);
	lua.setNumber(ls, "kick", gui->kick);
	lua.setNumber(ls, "snare", gui->snare);
	lua.setNumber(ls, "hat", gui->hat);
}

//----------------------------------------------------------------------
void ofApp::loadVideo() {

	if (gui->bLoadVideo) {

		player[0].load("movies/" + gui->movieFileNames[gui->video1Index]);
		player[1].load("movies/" + gui->movieFileNames[gui->video2Index]);

		if (!gui->bToggleLoop) {
			for (int i = 0; i <= playerSize; i++) {
				player[i].setLoopState(OF_LOOP_NORMAL);
			}
		}
		else {
			for (int i = 0; i <= playerSize; i++) {
				player[i].setLoopState(OF_LOOP_PALINDROME);
				std::cout << "Palindrome" << std::endl;
			}
		}
		gui->bLoadVideo = false;
	}
}

