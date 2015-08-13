#pragma once

#include "ofxParticle.h"
#include "ofxParticleEmitter.h"

class ofxParticleMeshEmitter : public ofxParticleEmitter {
    public:
        ofxParticleMeshEmitter() : ofxParticleEmitter(),
			maxParticles(1000) 
		{ }

        ~ofxParticleMeshEmitter() { }

		int maxParticles;

	private:

};