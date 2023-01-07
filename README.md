# Theia
 ...is a tiny vj app, designed to be lightweight.
 It's written, to fit my personal needs, so you might or might not find it useful.
  
 Features:
 Video playback and effects,
 Shader livecoding,
 Lua livecoding,
 Midi, OSC and sound input.

 
 # Building
 Theia is currently developed against the nightly openFrameworks releases, so grab a copy from the bottom of https://openframeworks.cc/download/ and build it if nescessary.
 
 Also you will need to clone the following repositories into the addons folder:
 #### [ofxBeat](https://github.com/darrenmothersele/ofxBeat) 
 #### [ofxHapPlayer](https://github.com/bangnoise/ofxHapPlayer)
 #### [ofxImGui](https://github.com/jvcleave/ofxImGui)
 #### [ofxLuaaa](https://github.com/arnkov/ofxLuaaa)
 #### [ofxMidi](https://github.com/danomatika/ofxMidi)
 #### [ofxPostGlitch](https://github.com/arnkov/ofxPostGlitch)
 
 Clone this repository into apps/myApps and run the project generator.
 
 Under Linux, just navigate to ...apps/myApps/Theia and type "make".
 
 Visual Studio users have to open the .sln, navigate to ofxLuaaa/libs/lua-5.4.4, select the .c files, right click, properties, c/c++, advanced, compile as C.
 
 Windows users can also download prebuilt binaries from [here](https://arnkov.itch.io/theia).
