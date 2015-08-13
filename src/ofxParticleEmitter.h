#pragma once

#include "ofxParticle.h"

class ofxParticleEmitter {
    public:
        ofxParticleEmitter() : 
			positionStart(), positionEnd(), posSpread(),
			velocityStart(), velocityEnd(), velSpread(),
			rotation(), rotSpread(), rotVel(), rotVelSpread(),
			size(1.0), sizeSpread(0.0),
			life(1.0), lifeSpread(0.0),
			numPars(1),
			color(255,255,255,255),colorSpread(0,0,0,0)
        {}

        ~ofxParticleEmitter(){}

        ofVec3f positionStart;
        ofVec3f positionEnd;
        ofVec3f posSpread;
        ofVec3f velocityStart;
        ofVec3f velocityEnd;
        ofVec3f velSpread;
        ofVec3f rotation;
        ofVec3f rotSpread;
        ofVec3f rotVel;
        ofVec3f rotVelSpread;
        float size;
        float sizeSpread;
        float life;
        float lifeSpread;
        int numPars;
        ofColor color;
        ofColor colorSpread;
        
		ofxParticleEmitter & setPosition(ofVec3f pos){
            positionStart = positionEnd = pos;
            return *this;
        }
        
		ofxParticleEmitter & setPosition(ofVec3f posStart, ofVec3f posEnd){
            positionStart = posStart;
            positionEnd = posEnd;
            return *this;
        }

        ofxParticleEmitter & setVelocity(ofVec3f vel){
            velocityStart = velocityEnd = vel;
            return *this;
        }

        ofxParticleEmitter & setVelocity(ofVec3f velStart, ofVec3f velEnd){
            velocityStart = velStart;
            velocityEnd = velEnd;
            return *this;
        }
};