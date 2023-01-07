#include "guiApp.h"
#include "ImHelpers.h"

void GuiApp::setup() {

    ofSetVerticalSync(false);
    ofSetLogLevel(OF_LOG_VERBOSE);

	//Load xml file for Midi Mapping
	loadXml();

    gui.setup();
    //ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::GetIO().MouseDrawCursor = false;

	shaderTextCol = ofColor(110);

	bShowGuide = false;
	bShowInfo = false;

    isStreaming = false;
	isListeningOSC = false;
	isListeningMIDI = false;

	bang = false;
	kick = 0.0;
	snare = 0.0;
	hat = 0.0;
	bKick = false;
	bSnare = false;
	bHat = false;
	magnitude = 0.0;

	threshold = minimumThreshold;
	lowBang = 0;
	randomBeat = 1.0;

	midiCtlText = "MidiCtl: none";
	midiNoteText = "MidiNote: none";
	oscText = "OscMessage: none";

	//List Lua scripts
	ofDirectory luaDirectory(ofToDataPath("scripts/", true));
	luaFiles = luaDirectory.getFiles();
	for (size_t i = 0; i < luaFiles.size(); i++)
	{
		luaFileNames.push_back(luaFiles[i].getFileName());
	}
	luaFileIndex = 0;

	//List shaders
	ofDirectory shaderDirectory(ofToDataPath("shaders/solo/", true));
	shaderFiles = shaderDirectory.getFiles();
	for (size_t i = 0; i < shaderFiles.size(); i++)
	{
		std::vector<string >name = ofSplitString(shaderFiles[i].getFileName(), ".");
		shaderFileNames.push_back(name[0]);
	}
	shaderFileIndex = 0;

	//List movies (Yeah, I know that i should collapse this into it's own function)
	ofDirectory movieDirectory(ofToDataPath("movies/", true));
	movieFiles = movieDirectory.getFiles();
	for (size_t i = 0; i < movieFiles.size(); i++)
	{
		movieFileNames.push_back(movieFiles[i].getFileName());
	}
	video1Index = 0;
	video2Index = 0;
}

void GuiApp::update() {

    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	updateSound();
	updateOSC();
	updateMidi();

	if (bLoadXml) {
		loadXml();
	}

	/*--------UpdateBeat&manageRandomness--------*/

	if (isStreaming) {

		beat.update(ofGetElapsedTimeMillis());
		//I believe isn't actually used anywhere but might be handy.
		//scaledRMS = ofMap(smoothRMS, 0.0, 0.17, 0.0, 1.0, true);
	}

	//Actually only existing for passing to ofApp.cpp. More clever way?
	kick = beat.kick();
	snare = beat.snare();
	hat = beat.hihat();
	bKick = beat.isKick();
	bSnare = beat.isSnare();
	bHat = beat.isHat();
	magnitude = beat.getMagnitude();

	/*Okay, so we want to trigger some events on onset. 
	Since ofxBeat's kick detection isn't very accurate, let's use the onset.
	Let's only use the kick thingie, if it's triggered by MIDI.
	You can find the MIDI controls under ::updateMidi.
	lowBang is triggered by OSC. Will be changed to also use kick soon.*/
	if (bang  || lowBang || (!bSound && kick)) {

		randomBeat = ofRandom(1);

		if (randomBeat < shaderRandom) {
			timer = ofGetElapsedTimef();
			if (bRandFbo) { bClearFbo = false; }
		}


		if (bRandCol && randomBeat < globalRandom) {
			r1 = ofRandom(255);
			g1 = ofRandom(255);
			b1 = ofRandom(255);
			r2 = ofRandom(255);
			g2 = ofRandom(255);
			b2 = ofRandom(255);
		}
	}
	//Reset shader and fbos after a certain time. Maybe expose time to gui? 
	else if (ofGetElapsedTimef() > timer + 0.5) {
		if (bRandFbo) { bClearFbo = true; }
		if (bRandGlitch) {
			glitchNum = 0;
		}
		if (bRandColGlitch) {
			glitchCol = 0;
		}
	}
}

void GuiApp::draw() {

    ofSetBackgroundColor(backgroundColor);

    auto mainSettings = ofxImGui::Settings();


    //required to call this at beginning
    this->gui.begin();
    
	static bool bCollapse = false;
    
	mainSettings.windowPos = ImVec2(ImVec2(0, 0));
    if (ofxImGui::BeginWindow("MAIN CONTROLS", mainSettings, ImGuiWindowFlags_NoCollapse, &bCollapse)) {

        ImGui::ColorEdit3("Background Color", (float*)&backgroundColor);

        if (ImGui::Button("DEFAULT THEME")){
            gui.setTheme(new ofxImGui::DefaultTheme());
        }ImGui::SameLine();

        if (ImGui::Button("GREY THEME")){
            gui.setTheme(new CustomTheme());
        }ImGui::SameLine();

        if (ImGui::Button("RANDOM THEME")){
            gui.setTheme(new RandomTheme());
        }

		if (ImGui::Button("QUICK GUIDE")){
			bShowGuide ^= 1;
		}
		if (ImGui::Button("INPUT INFO")){
			bShowInfo ^= 1;
		}

		if (ofxImGui::BeginTree(this->controlParams, mainSettings)){
			ImGuiIO& io = ImGui::GetIO();
			ImGui::CheckboxFlags("EnableKeyboard", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
			ofxImGui::AddParameter(this->bFullscreen);
			ofxImGui::AddParameter(this->bSound);
			ofxImGui::AddParameter(this->deviceNumber);
			ofxImGui::AddParameter(this->gain);
			ofxImGui::AddParameter(this->decayRate);
			ofxImGui::AddParameter(this->minimumThreshold);
			ofxImGui::AddParameter(this->bOSC);
			ofxImGui::AddParameter(this->bMIDI);
			ofxImGui::AddParameter(this->portNumber);
			ofxImGui::AddParameter(this->drumChannel);
			ofxImGui::AddParameter(this->bLoadXml);
			ofxImGui::AddParameter(this->bShowCursor);
			ofxImGui::AddParameter(this->globalRandom);
			ofxImGui::EndTree(mainSettings);
		}

		if (ofxImGui::BeginTree(this->videoParams, mainSettings)) {
			ofxImGui::AddParameter(this->bSpout);
			ofxImGui::AddParameter(this->bSpoutSettings);
			ofxImGui::AddParameter(this->bVideo);
			ofxImGui::AddParameter(this->bToggleLoop);
			ofxImGui::AddParameter(this->blending);
			if (ofxImGui::VectorCombo("Video1", &video1Index, movieFileNames))
			{
				ofLog() << "Video1 FILE PATH: " << movieFiles[video1Index].getAbsolutePath();
			}

			if (ofxImGui::VectorCombo("Video2", &video2Index, movieFileNames))
			{
				ofLog() << "Video2 FILE PATH: " << movieFiles[video2Index].getAbsolutePath();
			}
			ofxImGui::AddParameter(this->bLoadVideo);
			ofxImGui::EndTree(mainSettings);
		}

		if (ofxImGui::BeginTree(this->shaderParams, mainSettings)) {
			ofxImGui::AddParameter(this->shaderRandom);
			ofxImGui::AddParameter(this->bRandGlitch);
			ofxImGui::AddParameter(this->glitchNum);
			ofxImGui::AddParameter(this->bRandColGlitch);
			ofxImGui::AddParameter(this->glitchCol);
			ofxImGui::AddParameter(this->bRandCol);
			ofxImGui::AddParameter(this->bRandFbo);
			ofxImGui::AddParameter(this->bClearFbo);
			ofxImGui::AddParameter(this->bFeedback);
			ofxImGui::AddParameter(this->fbValue);
			ofxImGui::AddParameter(this->r1);
			ofxImGui::AddParameter(this->g1);
			ofxImGui::AddParameter(this->b1);
			ofxImGui::AddParameter(this->a1);
			ofxImGui::AddParameter(this->r2);
			ofxImGui::AddParameter(this->g2);
			ofxImGui::AddParameter(this->b2);
			ofxImGui::AddParameter(this->a2);
			ofxImGui::EndTree(mainSettings);
		}

		if (ofxImGui::BeginTree(soloShaderParams, mainSettings)) {
			ofxImGui::AddParameter(this->bSoloShader);
			ofxImGui::AddParameter(this->rmsMultiplier);
			ofxImGui::AddParameter(this->shaderPosX);
			ofxImGui::AddParameter(this->shaderPosY);
			ofxImGui::AddParameter(this->shaderVal0);
			ofxImGui::AddParameter(this->shaderVal1);
			ofxImGui::AddParameter(this->bShowCode);
			ImGui::ColorEdit3("textColour", (float*)&shaderTextCol);
			if (ofxImGui::VectorCombo("load Shader", &shaderFileIndex, shaderFileNames))
			{
				ofLog() << "SoloShader FILE PATH: " << shaderFiles[shaderFileIndex].getAbsolutePath();
			}
			ofxImGui::AddParameter(this->bReloadSoloShader);
			ofxImGui::EndTree(mainSettings);
		}

		if (ofxImGui::BeginTree(luaParams, mainSettings)) {
			ofxImGui::AddParameter(this->bLua);
			ofxImGui::AddParameter(this->bShowCode);
			ImGui::ColorEdit3("textColour", (float*)&shaderTextCol);
			if (!luaFileNames.empty())
			{
				if (ofxImGui::VectorCombo("load Lua script", &luaFileIndex, luaFileNames))
				{
					ofLog() << "LuaScript FILE PATH: " << luaFiles[luaFileIndex].getAbsolutePath();
				}
			}
			ofxImGui::AddParameter(this->bReloadLua);
			ofxImGui::EndTree(mainSettings);	
		}

		if (ofxImGui::BeginTree(postParams, mainSettings)) {
			ofxImGui::AddParameter(this->bFxaa);
			ofxImGui::AddParameter(this->bBloom);
			ofxImGui::AddParameter(this->bloomSteps);
			ofxImGui::AddParameter(this->bloomSeparation);
			ofxImGui::AddParameter(this->bloomThreshold);
			ofxImGui::AddParameter(this->bloomAmount);
			ofxImGui::EndTree(mainSettings);
		}

		ofxImGui::EndWindow(mainSettings);
	}


	if (bShowInfo) {
		ImGui::SetNextWindowSize(ofVec2f(200, 100), ImGuiCond_FirstUseEver);
		ImGui::Begin("Input Info", &bShowInfo);

			ImGui::Text("RMS:");
			ImGui::SameLine();
			ImGui::Text(ofToString(smoothRMS).c_str());
			ImGui::Text("Magnitude:");
			ImGui::SameLine();
			ImGui::Text(ofToString(magnitude).c_str());
			ImGui::Text("Onset:");
			ImGui::SameLine();
			ImGui::Text(ofToString(bang).c_str());
			ImGui::Text("Kick:");
			ImGui::SameLine();
			ImGui::Text(ofToString(kick).c_str());
			ImGui::Text("Snare:");
			ImGui::SameLine();
			ImGui::Text(ofToString(snare).c_str());
			ImGui::Text("Hat:");
			ImGui::SameLine();
			ImGui::Text(ofToString(hat).c_str());

			ImGui::Text(midiCtlText.c_str());
			ImGui::Text(midiNoteText.c_str());
			ImGui::Text(oscText.c_str());

			ImGui::End();
	}


	if (bShowGuide)	{

		ImGui::SetNextWindowSize(ofVec2f(200, 100), ImGuiCond_FirstUseEver);
		ImGui::Begin("Quick Guide", &bShowGuide);

		if (ImGui::Button("About")) {
			textBuffer = ofBufferFromFile("guide/about.txt");
		}ImGui::SameLine();
		if (ImGui::Button("ControlIn")) {
			textBuffer = ofBufferFromFile("guide/controlIn.txt");
		}ImGui::SameLine();
		if (ImGui::Button("Video")) {
			textBuffer = ofBufferFromFile("guide/video.txt");
		}ImGui::SameLine();
		if (ImGui::Button("Shaders&Fbos")) {
			textBuffer = ofBufferFromFile("guide/shadersFbos.txt");
		}ImGui::SameLine();
		if (ImGui::Button("SoloShaders")) {
			textBuffer = ofBufferFromFile("guide/soloShaders.txt");
		}
		if (ImGui::Button("LuaParams")) {
			textBuffer = ofBufferFromFile("guide/lua.txt");
		}ImGui::SameLine();
		if (ImGui::Button("postProcessing")) {
			textBuffer = ofBufferFromFile("guide/postProcessing.txt");
		}ImGui::SameLine();
		if (ImGui::Button("OSC")) {
			textBuffer = ofBufferFromFile("guide/osc.txt");
		}ImGui::SameLine();
		if (ImGui::Button("MIDI")) {
			textBuffer = ofBufferFromFile("guide/midi.txt");
		}ImGui::SameLine();
		if (ImGui::Button("Lua cheatsheet")) {
			textBuffer = ofBufferFromFile("guide/luaSheet.txt");
		}

		string infoText = textBuffer.getText();
		ImGui::Text(infoText.c_str());
		ImGui::End();
	}

    //required to call this at end
    this->gui.end();
}

void GuiApp::exit() {

	soundStream.stop();
	soundStream.close();
	midiIn.closePort();
	midiIn.removeListener(this);
	osc.stop();
}

void GuiApp::keyPressed(int key) {

	if (key == 'r') {
		if (bSoloShader) bReloadSoloShader = true;
		if (bLua) bReloadLua = true;
		if (bVideo) bLoadVideo = true;
	}
}

void GuiApp::updateSound() {

    if (bSound && !isStreaming) {

        soundStream.printDeviceList();
        ofSoundStreamSettings s;
        auto devices = soundStream.getMatchingDevices("default");
        if (!devices.empty()) {
            s.setInDevice(devices[deviceNumber]);
        }
        s.setInListener(this);
        s.sampleRate = 44100;
        s.numOutputChannels = 0;
        s.numInputChannels = 2;
        s.bufferSize = beat.getBufferSize();

        soundStream.setup(s);
        isStreaming = true;
    }
    else if (isStreaming && !bSound) {
        soundStream.stop();
        soundStream.close();
        ofLog() << "closed soundStream";
        isStreaming = false;
    }

}

void GuiApp::audioReceived(float* input, int bufferSize, int nChannels) {

	if (isStreaming == true) {
		beat.audioReceived(input, bufferSize, nChannels);
		// modified from audioInputExample
		float rms = 0.0;
		int numCounted = 0;

		for (int i = 0; i < beat.getBufferSize(); i++) {
			float sample = (input[i]);

			rms += (sample * sample) * gain;
			numCounted += 2;
		}

		rms /= (float)numCounted;
		rms = sqrt(rms);
		// rms is now calculated
		smoothRMS *= 0.93;
		smoothRMS += 0.07 * rms;

		threshold = ofLerp(threshold, minimumThreshold, decayRate);

		/*
		//use smoothRMS instead?
		if(rms > threshold) {
			// onset detected!
			threshold = rms;
			bang = true;
		} else bang = false;
		*/

		if (beat.getMagnitude() > threshold) {
			threshold = beat.getMagnitude();
			bang = true;
		}
		else bang = false;
	}
}

//-------------------------
void GuiApp::newMidiMessage(ofxMidiMessage& message) {

	// cout << message.pitch << endl;;
	// add the latest message to the message queue
	midiMessages.push_back(message);

	// remove any old messages if we have too many
	while (midiMessages.size() > maxMessages) {
		midiMessages.erase(midiMessages.begin());
	}
}

//-----------------------------------------
void GuiApp::updateMidi() {

	if (bMIDI && !isListeningMIDI) {
		midiIn.listInPorts();
		midiIn.openPort(portNumber);
		midiIn.ignoreTypes(false, false, false);
		midiIn.addListener(this);
		midiIn.setVerbose(true);
		isListeningMIDI = true;
	}
	else if (isListeningMIDI && !bMIDI) {
		midiIn.closePort();
		midiIn.removeListener(this);
		isListeningMIDI = false;
	}


	//Move to ::newMidiMessage?
	/*TODO: Add way to dynamically map controls to buttons, knobs or anything.*/
	for (unsigned int i = 0; i < midiMessages.size(); ++i) {

		ofxMidiMessage& message = midiMessages[i];

		if (isListeningMIDI) {

			//control message stuff
			if (message.status == MIDI_CONTROL_CHANGE) {
				if (message.control == button01) {
						bVideo = true; bLua = false; bSoloShader = false;
				}
				else if (message.control == button02) {
					bVideo = false; bLua = true; bSoloShader = false;
				}
				else if (message.control == button03) {
					bVideo = false; bLua = false; bSoloShader = true;
				}
			}

			//Notes
			if (message.status == MIDI_NOTE_ON && message.channel == drumChannel && !bSound) {
				if (message.pitch == note01) {
					kick = ofMap(message.velocity, 0, 127, 0.0f, 1.0f);
				}
				if (message.pitch == note02) {
					snare = ofMap(message.velocity, 0, 127, 0.0f, 1.0f);
				}
				if (message.pitch == note03) {
					hat = ofMap(message.velocity, 0, 127, 0.0f, 1.0f);
				}
			}
			
			if (bShowInfo) {
				if (message.status == MIDI_CONTROL_CHANGE) {
					midiCtlText = "midiCtl: " + ofToString(message.control) + " val: " + ofToString(message.value);
				}
				if (message.status == MIDI_NOTE_ON) {
					midiNoteText = "midiNote: " + ofToString(message.pitch) + " vel:" + ofToString(message.velocity);
				}
			}
		}
	}
}

//------------------------------------------------------------------
void GuiApp::updateOSC() {

	//Toggles OSC listener
	if (bOSC && !isListeningOSC) {

		ofLog() << "listening for osc messages on port " << PORT;
		osc.setup(PORT);
		isListeningOSC = true;
	}
	else if (isListeningOSC && !bOSC) {
		osc.stop();
		ofLog() << "closed osc listener on port " << PORT;
		isListeningOSC = false;
	}

	if (isListeningOSC) {

		// check for waiting messages
		while (osc.hasWaitingMessages()) {

			// get the next message
			ofxOscMessage m;
			osc.getNextMessage(m);

			//This was initially setup for a separate custom app. You might still wanna use it, 
			//if you are analyzing the sound somewhere else and just want to send messages accordingly
			if (m.getAddress() == "/band1") {
				lowBang = m.getArgAsFloat(1);
				lowRms = ofMap(m.getArgAsFloat(0), 0, 0.5, 0, 1.0, true);
			}
			else if (m.getAddress() == "/band2") {
				midBang = m.getArgAsFloat(1);
				midRms = ofMap(m.getArgAsFloat(0), 0, 0.5, 0, 1.0, true);
			}
			else if (m.getAddress() == "/band3") {
				highBang = m.getArgAsFloat(1);
				highRms = ofMap(m.getArgAsFloat(0), 0, 0.5, 0, 1.0, true);
			}

			//ControlIn
			if (m.getAddress() == "/bSound") { bSound = m.getArgAsBool(0); }
			if (m.getAddress() == "/deviceNumber") { deviceNumber = m.getArgAsInt(0); }
			if (m.getAddress() == "/gain") { gain = m.getArgAsFloat(0); }
			if (m.getAddress() == "/decayRate") { decayRate = m.getArgAsFloat(0); }
			if (m.getAddress() == "/onsetThresh") { minimumThreshold = m.getArgAsFloat(0); }
			if (m.getAddress() == "/bMidi") { bMIDI = m.getArgAsBool(0); }
			if (m.getAddress() == "/portNumber") { portNumber = m.getArgAsInt(0); }
			if (m.getAddress() == "/bShowInfo") { bShowInfo = m.getArgAsBool(0); };
			if (m.getAddress() == "/globalRand") { globalRandom = m.getArgAsFloat(0); }
			//Video
			if (m.getAddress() == "/bSpout") { bSpout = m.getArgAsBool(0); }
			if (m.getAddress() == "/bSpoutSettings") { bSpoutSettings = m.getArgAsBool(0); }
			if (m.getAddress() == "/bVideo") { bVideo = m.getArgAsBool(0); }
			if (m.getAddress() == "/blending") { blending = m.getArgAsInt(0); }
			if (m.getAddress() == "/bLoadVideo") { bLoadVideo = m.getArgAsBool(0); }
			if (m.getAddress() == "/bToggleLoop") { bToggleLoop = m.getArgAsBool(0); }
			//Shaders & Fbos
			if (m.getAddress() == "/shaderRandom") { shaderRandom = m.getArgAsFloat(0); }
			if (m.getAddress() == "/bRandGlitch") { bRandGlitch = m.getArgAsBool(0); }
			if (m.getAddress() == "/glitchNum") { glitchNum = m.getArgAsInt(0); }
			if (m.getAddress() == "/bRandColGlitch") { bRandColGlitch = m.getArgAsBool(0); }
			if (m.getAddress() == "/glitchCol") { glitchCol = m.getArgAsInt(0); }
			if (m.getAddress() == "/bRandFbo") { bRandFbo = m.getArgAsBool(0); }
			if (m.getAddress() == "/bClearFbo") { bClearFbo = m.getArgAsBool(0); }
			if (m.getAddress() == "/bFeedback") { bFeedback = m.getArgAsBool(0); }
			if (m.getAddress() == "/fbValue") { fbValue = m.getArgAsFloat(0); }
			if (m.getAddress() == "/bRandCol") { bRandCol = m.getArgAsBool(0); }
			if (m.getAddress() == "/r1") { r1 = m.getArgAsInt(0); }
			if (m.getAddress() == "/g1") { g1 = m.getArgAsInt(0); }
			if (m.getAddress() == "/b1") { b1 = m.getArgAsInt(0); }
			if (m.getAddress() == "/a1") { a1 = m.getArgAsInt(0); }
			if (m.getAddress() == "/r2") { r2 = m.getArgAsInt(0); }
			if (m.getAddress() == "/g2") { g2 = m.getArgAsInt(0); }
			if (m.getAddress() == "/b2") { b2 = m.getArgAsInt(0); }
			if (m.getAddress() == "/a2") { a2 = m.getArgAsInt(0); }
			//Lua
			if (m.getAddress() == "/bLua") { bLua = m.getArgAsBool(0); }
			if (m.getAddress() == "/bReloadLua") { bReloadLua = m.getArgAsBool(0); }
			//SoloShaders
			if (m.getAddress() == "/bSoloShader") { bSoloShader = m.getArgAsBool(0); }
			if (m.getAddress() == "/rmsMultiplier") { rmsMultiplier = m.getArgAsFloat(0); }
			if (m.getAddress() == "/bShowCode") { bShowCode = m.getArgAsBool(0); }
			if (m.getAddress() == "/shaderVal0") { shaderVal0 = m.getArgAsFloat(0); }
			if (m.getAddress() == "/shaderVal1") { shaderVal1 = m.getArgAsFloat(0); }
			//PostProcessing
			if (m.getAddress() == "/bFxaa") { bFxaa = m.getArgAsBool(0); }
			if (m.getAddress() == "/bBloom") { bBloom = m.getArgAsBool(0); }
			if (m.getAddress() == "/bloomSteps") { bloomSteps = m.getArgAsInt(0); }
			if (m.getAddress() == "/bloomSeparation") { bloomSeparation = m.getArgAsFloat(0); }
			if (m.getAddress() == "/bloomThreshold") { bloomThreshold = m.getArgAsFloat(0); }
			if (m.getAddress() == "/bloomAmount") { bloomAmount = m.getArgAsFloat(0); }

			//Not in use for now due to redoing the whole shabang. Just here as a reminder.


			oscText = ofToString(m.getAddress());
			oscText += ":";
			for (size_t i = 0; i < m.getNumArgs(); i++) {

				oscText += ": ";

				// display the argument - make sure we get the right type
				if (m.getArgType(i) == OFXOSC_TYPE_INT32) {
					oscText += ofToString(m.getArgAsInt32(i));
				}
				else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT) {
					oscText += ofToString(m.getArgAsFloat(i));
				}
				else if (m.getArgType(i) == OFXOSC_TYPE_STRING) {
					oscText += m.getArgAsString(i);
				}
				else if (m.getArgType(i) == OFXOSC_TYPE_FALSE) {
					oscText += ofToString(m.getArgAsBool(i));
				}
				else if (m.getArgType(i) == OFXOSC_TYPE_TRUE) {
					oscText += ofToString(m.getArgAsBool(i));
				}
				else {
					oscText += "unhandled argument type " + m.getArgTypeName(i);
				}
			}
		}
	}
}

void GuiApp::loadXml() {

	/*These are used for Midi mapping.
	At the moment not all of them are used anymore,
	I'm still trying to figure out a more flexible but still intuitive way to use Midi.
	Also it should require as little as possible buttons and knobs.
	First version was made for Arturia Beatstep Pro,
	but I think it should be usable with less expensive equipment.*/

	xml.load("settings/settings.xml");

	knob01 = xml.getValue("midi:knob01", 0);
	knob02 = xml.getValue("midi:knob02", 0);
	knob03 = xml.getValue("midi:knob03", 0);
	knob04 = xml.getValue("midi:knob04", 0);
	knob05 = xml.getValue("midi:knob05", 0);
	knob06 = xml.getValue("midi:knob06", 0);
	knob07 = xml.getValue("midi:knob07", 0);
	knob08 = xml.getValue("midi:knob08", 0);
	knob09 = xml.getValue("midi:knob09", 0);
	knob10 = xml.getValue("midi:knob10", 0);
	knob11 = xml.getValue("midi:knob11", 0);
	knob12 = xml.getValue("midi:knob12", 0);
	knob13 = xml.getValue("midi:knob13", 0);
	knob14 = xml.getValue("midi:knob14", 0);
	knob15 = xml.getValue("midi:knob15", 0);
	knob16 = xml.getValue("midi:knob16", 0);

	button01 = xml.getValue("midi:button01", 0);
	button02 = xml.getValue("midi:button02", 0);
	button03 = xml.getValue("midi:button03", 0);
	button04 = xml.getValue("midi:button04", 0);
	button05 = xml.getValue("midi:button05", 0);
	button06 = xml.getValue("midi:button06", 0);
	button07 = xml.getValue("midi:button07", 0);
	button08 = xml.getValue("midi:button08", 0);
	button09 = xml.getValue("midi:button09", 0);
	button10 = xml.getValue("midi:button10", 0);
	button11 = xml.getValue("midi:button11", 0);
	button12 = xml.getValue("midi:button12", 0);
	button13 = xml.getValue("midi:button13", 0);
	button14 = xml.getValue("midi:button14", 0);
	button15 = xml.getValue("midi:button15", 0);
	button16 = xml.getValue("midi:button16", 0);

	note01 = xml.getValue("midi:note01", 0);
	note02 = xml.getValue("midi:note02", 0);
	note03 = xml.getValue("midi:note03", 0);
	note04 = xml.getValue("midi:note04", 0);
	note05 = xml.getValue("midi:note05", 0);
	note06 = xml.getValue("midi:note06", 0);
	note07 = xml.getValue("midi:note07", 0);
	note08 = xml.getValue("midi:note08", 0);
	note09 = xml.getValue("midi:note09", 0);
	note10 = xml.getValue("midi:note10", 0);
	note11 = xml.getValue("midi:note11", 0);
	note12 = xml.getValue("midi:note12", 0);
	note13 = xml.getValue("midi:note13", 0);
	note14 = xml.getValue("midi:note14", 0);
	note15 = xml.getValue("midi:note15", 0);
	note16 = xml.getValue("midi:note16", 0);

	bLoadXml = false;
}
