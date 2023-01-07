#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "CustomTheme.h"
#include "RandomTheme.h"
#include "ofxBeat.h"
#include "ofxMidi.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

// listening port
#define PORT 8000



class GuiApp : public ofBaseApp, public ofxMidiListener {
public:
	void setup();
	void update();
	void draw();
	void exit();
	void keyPressed(int key);
	void audioReceived(float* input, int bufferSize, int nChannels);
	void newMidiMessage(ofxMidiMessage& eventArgs);
	void updateMidi();
	void updateOSC();
	void updateSound();
	void loadXml();


	//Sound
	ofxBeat beat;
	ofSoundStream soundStream;
	bool isStreaming;
	float smoothRMS;
	float scaledRMS;
	float threshold;
	bool bang;
	float kick;
	float snare;
	float hat;
	float magnitude;
	bool bKick;
	bool bSnare;
	bool bHat;

	//midi
	ofxMidiIn midiIn;
	std::vector<ofxMidiMessage> midiMessages;
	std::size_t maxMessages = 10;
	bool isListeningMIDI;
	std::string midiCtlText;
	std::string midiNoteText;

	//OSC
	ofxOscReceiver osc;
	int currentMsgString;
	string oscText;
	bool isListeningOSC;

	//will most likely be deprecated: I used this with a Custom Audio-Analyzer App, which sent messages accordingly...
	float lowBang;
	float midBang;
	float highBang;
	float lowRms;
	float midRms;
	float highRms;

	//Randomness
	float randomBeat;
	float timer;

	//Files --- the actual loading takes place in ofApp.cpp
	std::vector<std::string> luaFileNames;
	std::vector<ofFile> luaFiles;
	//Maybe use an ofParameter for indexing, to make acces through MIDI and OSC easier?
	int luaFileIndex;
	std::vector<std::string> shaderFileNames;
	std::vector<ofFile> shaderFiles;
	int shaderFileIndex;
	std::vector<std::string> movieFileNames;
	std::vector<ofFile> movieFiles;
	int video1Index;
	int video2Index;

	//the actual gui
	ofxImGui::Gui gui;

	ImVec4 backgroundColor;

	//guide
	bool bShowGuide;
	bool bShowInfo;
	ofBuffer textBuffer;

	//Main Controls
	ofParameter<bool> bFullscreen{ "fullscreen", false };
	ofParameter<bool> bSound{ "useSound", false };
	ofParameter<int> deviceNumber{ "deviceNumber", 0, 0, 10 };
	ofParameter<float> gain{ "gain", 1, 0.005, 5 };
	ofParameter<float> decayRate{ "onsetDecayRate", 0.05, 0, 0.5 };
	ofParameter<float> minimumThreshold{ "onsetMinThreshold", 10.0, 0.01, 20.0 };
	ofParameter<bool> bOSC{ "useOSC", false };
	ofParameter<bool> bMIDI{ "useMIDI", false };
	ofParameter<bool> bLoadXml{ "loadXML", false };
	ofParameter<int> portNumber{ "portNumber", 0, 0, 10 };
	ofParameter<int> drumChannel{ "drumChannel", 0, 0, 15 };
	ofParameter<bool> bShowCursor{ "showCursor", true };
	ofParameter<float> globalRandom{ "globalRandomness", 0, 0, 1 };
	ofParameterGroup controlParams{
		"ControlIn",
		bFullscreen,
		bSound,
		deviceNumber,
		decayRate,
		minimumThreshold,
		bOSC,
		bMIDI,
		portNumber,
		bShowCursor,
		globalRandom
	};

	//Video
	ofParameter<bool> bSpout{ "useSpout", false };
	ofParameter<bool> bSpoutSettings{ "openSpoutSettings", false };
	ofParameter<bool> bVideo{ "drawVideo", false };
	ofParameter<bool> bToggleLoop{ "palindrome", false };
	ofParameter<int> blending{ "blendMode", 0, 0, 4 };
	ofParameter<bool> bLoadVideo{ "(R) loadVideo", false };
	ofParameterGroup videoParams{
		"Video",
		bSpout,
		bSpoutSettings,
		bVideo,
		blending,
	};

	//Shaders
	ofParameter<float> shaderRandom{ "shaderRandomness", 0, 0, 1 };
	ofParameter<bool> bRandGlitch{ "randomGlitch", false };
	ofParameter<int> glitchNum{ "glitchNumber", 0, 0,  8};
	ofParameter<bool> bRandColGlitch{ "randColourGlitch", false };
	ofParameter<int> glitchCol{ "glitchColourNumber", 0, 0, 7 };
	ofParameter<bool> bRandFbo{ "randomFbo", false };
	ofParameter<bool> bClearFbo{ "clearFbo", true };
	ofParameter<bool> bFeedback{ "feeback", false };
	ofParameter<int> fbValue{ "feedbackValue", 0, 0, 20 };
	ofParameter<bool> bRandCol{ "useRandomColours", false };
	ofParameter<int> r1{ "R1", 255, 0, 255 };
	ofParameter<int> g1{ "G1", 255, 0, 255 };
	ofParameter<int> b1{ "B1", 255, 0, 255 };
	ofParameter<int> a1{ "A1", 255, 0, 255 };
	ofParameter<int> r2{ "R2", 255, 0, 255 };
	ofParameter<int> g2{ "G2", 255, 0, 255 };
	ofParameter<int> b2{ "B2", 255, 0, 255 };
	ofParameter<int> a2{ "A2", 255, 0, 255 };
	ofParameterGroup shaderParams{
		"Shaders&FBO's",
		shaderRandom,
		bRandGlitch,
		glitchNum,
		bRandColGlitch,
		glitchCol,
		bRandCol,
		bRandFbo,
		bClearFbo,
		bFeedback,
		fbValue,
		r1,
		g1,
		b1,
		a1,
		r2,
		g2,
		b2,
		a2
	};
	
	//Lua
	ofParameter<bool> bLua{ "useLua", false };
	ofParameter<bool> bReloadLua{ "(R) reloadLuaScript", false };
	ofParameterGroup luaParams{
		"luaParams",
		bLua,
		bReloadLua,
	};

	//Live Shaders
	ofParameter<bool> bSoloShader{ "soloShaders", false };
	ofParameter<float> rmsMultiplier{ "rmsMultiplier", 0.5, 0.0, 2.0 };
	ofParameter<float> shaderPosX{ "posX", 0, 0, 1000 };
	ofParameter<float> shaderPosY{ "posY", 0, 0, 1000 };
	ofParameter<float> shaderVal0{ "val0", 0.0, 0.0, 1.0 };
	ofParameter<float> shaderVal1{ "val1", 0.0, -1.0, 1.0 };
	ofParameter<bool> bShowCode{ "showCode", false };
	ofParameter<bool> bReloadSoloShader{ "(R) reloadShader", false };
	ImVec4 shaderTextCol;
	ofParameterGroup soloShaderParams{
		"soloShaders",
		bSoloShader,
		bReloadSoloShader,
		rmsMultiplier,
		bShowCode
	};

	//Post Processing
	ofParameter<bool> bFxaa{ "enableFxaa", false };
	ofParameter<bool> bBloom{ "enableBloom", false };
	ofParameter<int>  bloomSteps{"steps", 3, 0, 8};
	ofParameter<float> bloomSeparation{"separation", 4.0, 0.0, 10.0};
	ofParameter<float> bloomThreshold{"threshold", 0.6, 0.0, 1.0};
	ofParameter<float> bloomAmount{"amount", 0.6, 0.0, 1.0};
	ofParameterGroup postParams{
		"postProcessing",
		bBloom,
		bloomSteps,
		bloomSeparation,
		bloomThreshold,
		bloomAmount
	};

	//XML Settings
	ofxXmlSettings xml;

	ofKey reloadKey;

	int midiChannel;

	float knob01;
	float knob02;
	float knob03;
	float knob04;
	float knob05;
	float knob06;
	float knob07;
	float knob08;
	float knob09;
	float knob10;
	float knob11;
	float knob12;
	float knob13;
	float knob14;
	float knob15;
	float knob16;

	int button01;
	int button02;
	int button03;
	int button04;
	int button05;
	int button06;
	int button07;
	int button08;
	int button09;
	int button10;
	int button11;
	int button12;
	int button13;
	int button14;
	int button15;
	int button16;

	int note01;
	int note02;
	int note03;
	int note04;
	int note05;
	int note06;
	int note07;
	int note08;
	int note09;
	int note10;
	int note11;
	int note12;
	int note13;
	int note14;
	int note15;
	int note16;
};
