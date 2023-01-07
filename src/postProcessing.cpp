#include "postProcessing.h"

//based on https://github.com/maxillacult/ofxPostGlitch

void postProcessing::setup(ofFbo* buffer_) {
	targetBuffer = buffer_;
	buffer_size.set(buffer_->getWidth(), buffer_->getHeight());
	ShadingBuffer.allocate(buffer_size.x, buffer_size.y);
}


void postProcessing::setPost(postType type_, bool enabled) {
	bShading[type_] = enabled;
}

void postProcessing::togglePost(postType type_) {
	bShading[type_] ^= true;
}

bool postProcessing::getPost(postType type_) {
	return bShading[type_];
}

void postProcessing::setBloomParams(int size, float separation, float threshold, float amount) {
	sizeParam = size;
	sepaParam = separation;
	threshParam = threshold;
	amountParam = amount;
}

void postProcessing::generatePost() {
	if (targetBuffer == NULL) {
		ofLog(OF_LOG_WARNING, "fxaa --- Fbo is not allocated.");
		return;
	}

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofSetColor(255);
	glClearColor(0, 0, 0, 0.0);

	for (int i = 0; i < POST_NUM; i++) {
		if (bShading[i]) {
			shader[i].begin();
			shader[i].setUniformTexture("tex0", *targetBuffer, 0);
			shader[i].setUniform2f("resolution", ofGetWidth(), ofGetHeight());
			shader[i].setUniform1i("size", sizeParam);
			shader[i].setUniform1f("separation", sepaParam);
			shader[i].setUniform1f("threshold", threshParam);
			shader[i].setUniform1f("amount", amountParam);

			ShadingBuffer.begin();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			ofRect(0, 0, buffer_size.x, buffer_size.y);
			ShadingBuffer.end();
			shader[i].end();

			targetBuffer->begin();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			ShadingBuffer.draw(0, 0, buffer_size.x, buffer_size.y);
			targetBuffer->end();
		}
	}
}

