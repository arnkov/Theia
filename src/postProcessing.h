#pragma once

// Based on https://github.com/maxillacult/ofxPostGlitch

#include "ofMain.h"

#define POST_NUM 2

enum postType {
	FXAA,
	BLOOM
};

	class postProcessing {
	public:

		postProcessing() {

			targetBuffer = NULL;

			if (ofIsGLProgrammableRenderer()) {
				shader[0].load("shaders/post/gl3/fxaa");
				shader[1].load("shaders/post/gl3/bloom");
			}
			else {
				shader[0].load("shaders/post/gl2/fxaa");
				shader[1].load("shaders/post/gl2/bloom");
			}
		}

		/* Initialize & set target Fbo */
		void setup(ofFbo* buffer_);

		/* Switch each effects on/off */
		void setPost(postType type_, bool enabled);

		/* Toggle each effects on/off */
		void togglePost(postType type_);

		/* Return current effect's enabled info*/
		bool getPost(postType type_);

		/* Apply enable effects to target Fbo */
		void generatePost();

		void setBloomParams(int size, float separation, float threshold, float amount);

	protected:
		bool bShading[POST_NUM];
		ofShader shader[POST_NUM];
		ofFbo* targetBuffer;
		ofFbo ShadingBuffer;
		ofPoint	buffer_size;
		int sizeParam = 0;
		float sepaParam = 0.0;
		float threshParam = 0.0;
		float amountParam = 0.0;
	};


