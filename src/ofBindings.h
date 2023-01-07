#pragma once

#include "ofMain.h"
#include "ofxLuaaa.h"

using namespace luaaa;

class luaDraw {

public:
	luaDraw() {};
	virtual ~luaDraw() {};

	void noFill() {
		ofNoFill();
	}
	void fill() {
		ofFill();
	}
	void setCircleResolution(int res) {
		ofSetCircleResolution(res);
	}
	void setSphereResolution(int res) {
		ofSetSphereResolution(res);
	}
	void setLineWidth(float x) {
		ofSetLineWidth(x);
	}
	void drawCircle(float x, float y, float radius) {
		ofDrawCircle(x, y, radius);
	}
	void drawBox(float x, float y, float z, float size) {
		ofDrawBox(x, y, z, size);
	}
	void drawSphere(float x, float y, float z, float size) {
		ofDrawSphere(x, y, z, size);
	}
	void drawIcoSphere(float x, float y, float z, float radius) {
		ofDrawIcoSphere(x, y, z, radius);
	}
	void drawRectanle(float x, float y, float w, float h) {
		ofDrawRectangle(x, y, w, h);
	}
	void drawPlane(float x, float y, float z, float width, float height) {
		ofDrawPlane(x, y, z, width, height);
	}
	void drawCylinder(float x, float y, float z, float radius, float height) {
		ofDrawCylinder(x, y, z, radius, height);
	}
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) {
		ofDrawLine(x1, y1, z1, x2, y2, z2);
	}

	void bindToLua(lua_State* state) {
		LuaClass<luaDraw> draw(state, "draw");
		draw.ctor();
		draw.fun("noFill", &luaDraw::noFill);
		draw.fun("fill", &luaDraw::fill);
		draw.fun("setCircleResolution", &luaDraw::setCircleResolution);
		draw.fun("setSphereResolution", &luaDraw::setSphereResolution);
		draw.fun("setLineWidth", &luaDraw::setLineWidth);
		draw.fun("drawCircle", &luaDraw::drawCircle);
		draw.fun("drawSphere", &luaDraw::drawSphere);
		draw.fun("drawIcoSphere", &luaDraw::drawIcoSphere);
		draw.fun("drawBox", &luaDraw::drawBox);
		draw.fun("drawRectangle", &luaDraw::drawRectanle);
		draw.fun("drawPlane", &luaDraw::drawPlane);
		draw.fun("drawCylinder", &luaDraw::drawCylinder);
		draw.fun("drawLine", &luaDraw::drawLine);
	}

};

class luaColor {
public:
	luaColor() {};
	virtual ~luaColor() {};

	void setColor(int r, int g, int b, int a) {
		ofSetColor(r, g, b, a);
	}
	void setFloatColor(float r, float g, float b, float a) {
		ofSetColor(ofFloatColor(r, g, b, a));
	}
	void setBackgroundColor(int r, int g, int b) {
		ofSetBackgroundColor(r, g, b);
	}

	void clear(float r, float g, float b, float a) {
		ofClear(r, g, b, a);
	}

	void bindToLua(lua_State *state) {
		LuaClass<luaColor> color(state, "color");
		color.ctor();
		color.fun("setColor", &luaColor::setColor);
		color.fun("setFloatColor", &luaColor::setFloatColor);
		color.fun("setBackgroundColor", &luaColor::setBackgroundColor);
		color.fun("clear", &luaColor::clear);
	}

};

class luaFbo : ofFbo {
public:
	luaFbo() {};
	virtual ~luaFbo() {};

	void allocateFbo(int x, int y) {
		ofFbo::allocate(x, y);
	}

	void clearFbo() {
		ofFbo::clear();
	}

	void beginFbo() {
		ofFbo::begin();
	}

	void endFbo() {
		ofFbo::end();
	}

	void drawFbo(int x, int y) {
		ofFbo::draw(x, y);
	}

	void bindToLua(lua_State* state) {
		LuaClass<luaFbo> fbo(state, "fbo");
		fbo.ctor();
		fbo.fun("allocateFbo", &luaFbo::allocateFbo);
		fbo.fun("clearFbo", &luaFbo::clearFbo);
		fbo.fun("beginFbo", &luaFbo::beginFbo);
		fbo.fun("endFbo", &luaFbo::endFbo);
		fbo.fun("drawFbo", &luaFbo::drawFbo);
	}
};

struct luaCam : ofCamera {
public:
	luaCam() {};
	virtual ~luaCam() {};

	void beginCam() {
		ofCamera::begin();
	}
	void endCam() {
		ofCamera::end();
	}
	void lookAt(float x, float y, float z) {
		ofCamera::lookAt(glm::vec3(x, y, z));
	}
	void setPosition(float x, float y, float z) {
		ofCamera::setPosition(x, y, z);
	}

	void bindToLua(lua_State* state) {
		LuaClass<luaCam> cam(state, "cam");
		cam.ctor();
		cam.fun("beginCam", &luaCam::beginCam);
		cam.fun("endCam", &luaCam::endCam);
		cam.fun("lookAt", &luaCam::lookAt);
		cam.fun("setPosition", &luaCam::setPosition);
	}
};

class luaUtils {
public:
	luaUtils() {};
	virtual ~luaUtils() {};

	int getWidth() {
		return ofGetWidth();
	}

	int getHeight() {
		return ofGetHeight();
	}

	float getElapsedTime() {
		return ofGetElapsedTimef();
	}

	void bindToLua(lua_State* state) {
		LuaClass<luaUtils> utils(state, "utils");
		utils.ctor();
		utils.fun("getWidth", &luaUtils::getWidth);
		utils.fun("getHeight", &luaUtils::getHeight);
		utils.fun("getElapsedTime", &luaUtils::getElapsedTime);
	}
};

class luaMath {
public:
	luaMath() {};
	virtual ~luaMath() {};

	float noise(float x, float y) {
		return ofNoise(glm::vec2(x, y));
	}

	float signedNoise(float x, float y) {
		return ofSignedNoise(glm::vec2(x, y));
	}

	float random(float x) {
		return ofRandom(x);
	}

	float random2(float x, float y) {
		return ofRandom(x, y);
	}

	float lerp(float start, float stop, float amt) {
		return ofLerp(start, stop, amt);
	}

	void bindToLua(lua_State* state) {
		LuaClass<luaMath> math(state, "math");
		math.ctor();
		math.fun("noise", &luaMath::noise);
		math.fun("signedNoise", &luaMath::signedNoise);
		math.fun("random", &luaMath::random);
		math.fun("random2", &luaMath::random2);
		math.fun("lerp", &luaMath::lerp);
	}
};

class luaMaterial : ofMaterial {
public:
	luaMaterial() {};
	virtual ~luaMaterial() {};

	void setPBR(bool x) {
		ofMaterial::setPBR(x);
	}

	void beginMaterial() {
		ofMaterial::begin();
	}

	void endMaterial(){
		ofMaterial::end();
	}

	void setDiffuse(float r, float g, float b) {
		ofMaterial::setDiffuseColor(ofFloatColor(r, g, b));
	}

	void setRoughness(float x) {
		ofMaterial::setRoughness(x);
	}

	void setShininess(float x) {
		ofMaterial::setShininess(x);
	}

	void bindToLua(lua_State* state) {
		LuaClass<luaMaterial> material(state, "material");
		material.ctor();
		material.fun("setPBR", &luaMaterial::setPBR);
		material.fun("beginMaterial", &luaMaterial::beginMaterial);
		material.fun("endMaterial", &luaMaterial::endMaterial);
		material.fun("setDiffuse", &luaMaterial::setDiffuse);
		material.fun("setRoughness", &luaMaterial::setRoughness);
		material.fun("setShininess", &luaMaterial::setShininess);
	}
};

class luaLight : ofLight {
public:
	luaLight() {};
	virtual ~luaLight() {};

	void enableLighting() {
		ofEnableLighting();
	}

	void disableLighting() {
		ofDisableLighting();
	}

	void setup() {
		ofLight::setup();
	}

	void setPosition(float x, float y, float z) {
		ofLight::setPosition(glm::vec3(x, y, z));
	}

	void enable() {
		ofLight::enable();
	}

	void disable() {
		ofLight::disable();
	}

	void bindToLua(lua_State* state) {
		LuaClass<luaLight> light(state, "light");
		light.ctor();
		light.fun("enableLighting", &luaLight::enableLighting);
		light.fun("disableLighting", &luaLight::disableLighting);
		light.fun("setup", &luaLight::setup);
		light.fun("setPosition", &luaLight::setPosition);
		light.fun("enable", &luaLight::enable);
		light.fun("disable", &luaLight::disable);
	}

};

class luaGL {
public:
	luaGL() {};
	virtual ~luaGL() {};

	void enableDepthTest() {
		ofEnableDepthTest();
	}
	void disableDepthTest() {
		ofDisableDepthTest();
	}

	void pushMatrix() {
		ofPushMatrix();
	}

	void popMatrix() {
		ofPopMatrix();
	}

	void translate(float x, float y, float z) {
		ofTranslate(glm::vec3(x, y, z));
	}

	void rotateX(float x) {
		ofRotateXDeg(x);
	}

	void rotateY(float y) {
		ofRotateYDeg(y);
	}

	void rotateZ(float z) {
		ofRotateZDeg(z);
	}

	void bindToLua(lua_State* state) {
		LuaClass<luaGL> gl(state, "gl");
		gl.ctor();
		gl.fun("enableDepthTest", &luaGL::enableDepthTest);
		gl.fun("disableDepthTest", &luaGL::disableDepthTest);
		gl.fun("pushMatrix", &luaGL::pushMatrix);
		gl.fun("popMatrix", &luaGL::popMatrix);
		gl.fun("translate", &luaGL::translate);
		gl.fun("rotateX", &luaGL::rotateX);
		gl.fun("rotateY", &luaGL::rotateY);
		gl.fun("rotateZ", &luaGL::rotateZ);
	}
};
