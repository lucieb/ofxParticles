#pragma once

#include "ofxParticleEmitter.h"
#include "ofxParticleMeshEmitter.h"

inline ofVec3f ofRandVec3f(){
    return ofVec3f(ofRandomf(),ofRandomf(),ofRandomf()).normalize().scale(ofRandomf());
}

class ofxParticleSystem {
    public:
        ofxParticleSystem(){
            numParticles = 0;
            totalParticlesEmitted = 0;
        }
        ~ofxParticleSystem(){}
            
        void addParticles(ofxParticleEmitter & src){
            for(int i=0;i<src.numPars;i++){
                ofVec3f pos = src.positionStart;
                ofVec3f vel = src.velocityStart;
                if(src.positionEnd != src.positionStart || src.velocityStart != src.velocityEnd){
                    float rf = ofRandomuf();
                    pos = src.positionStart.interpolated(src.positionEnd, rf);
                    vel = src.velocityStart.interpolated(src.velocityEnd, rf);
                }
                ofVec3f p = pos+ofRandVec3f()*src.posSpread;
                ofVec3f v = vel+ofRandVec3f()*src.velSpread;
                float s = src.size+ofRandomf()*src.sizeSpread;
                float l = src.life+ofRandomf()*src.lifeSpread;
                ofxParticle * par = new ofxParticle(p,v,s,l);
                par->rotation = src.rotation+ofRandVec3f()*src.rotSpread;
                par->rotationalVelocity = src.rotVel+ofRandVec3f()*src.rotVelSpread;
                par->particleID = totalParticlesEmitted+i;
                ofColor pColor = src.color;
                if(src.colorSpread != ofColor(0,0,0,0)){
                    pColor.r = ofClamp(pColor.r + ofRandomf()*src.colorSpread.r,0,255);
                    pColor.g = ofClamp(pColor.g + ofRandomf()*src.colorSpread.g,0,255);
                    pColor.b = ofClamp(pColor.b + ofRandomf()*src.colorSpread.b,0,255);
                    pColor.a = ofClamp(pColor.a + ofRandomf()*src.colorSpread.a,0,255);
                }
                par->color = pColor;
                particles.push_back(par);
            }
            numParticles+=src.numPars;
            totalParticlesEmitted+=src.numPars;
        }

		void addParticles(ofxParticleMeshEmitter & src, ofMesh& mesh){

			int numPars = src.maxParticles-numParticles;
			int numVertices = mesh.getNumVertices();

			if (mesh.getNumVertices() == 0) return;

            for(int n=0 ; n<numPars ; n++){

				int i = ofRandom(numVertices-1);

				ofVec3f pos = mesh.getVertex(i);
                ofVec3f vel = src.velocityStart;
                if(src.positionEnd != src.positionStart || src.velocityStart != src.velocityEnd){
                    float rf = ofRandomuf();
                    pos = src.positionStart.interpolated(src.positionEnd, rf);
                    vel = src.velocityStart.interpolated(src.velocityEnd, rf);
                }
                ofVec3f p = pos+ofRandVec3f()*src.posSpread;
                ofVec3f v = vel+ofRandVec3f()*src.velSpread;
                float s = src.size+ofRandomf()*src.sizeSpread;
                float l = src.life+ofRandomf()*src.lifeSpread;
                ofxParticle * par = new ofxParticle(p,v,s,l);
                par->rotation = src.rotation+ofRandVec3f()*src.rotSpread;
                par->rotationalVelocity = src.rotVel+ofRandVec3f()*src.rotVelSpread;
                par->particleID = totalParticlesEmitted+i;
                ofColor pColor = src.color;
                if(src.colorSpread != ofColor(0,0,0,0)){
                    pColor.r = ofClamp(pColor.r + ofRandomf()*src.colorSpread.r,0,255);
                    pColor.g = ofClamp(pColor.g + ofRandomf()*src.colorSpread.g,0,255);
                    pColor.b = ofClamp(pColor.b + ofRandomf()*src.colorSpread.b,0,255);
                    pColor.a = ofClamp(pColor.a + ofRandomf()*src.colorSpread.a,0,255);
                }
                par->color = pColor;
                particles.push_back(par);
            }
            numParticles+=numPars;
            totalParticlesEmitted+=numPars;
        }
            
        void attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle){
            for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
                (**it).attractTo(p, accel, minDist, consumeParticle);
            }
        }
            
        void gravitateTo(ofPoint p, const float gravity, const float mass, const float minDist, const float consumeParticle){
            for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
                (**it).gravitateTo(p, gravity, mass, minDist, consumeParticle);
            }
        }
            
        void rotateAround(ofPoint p, const float accel, const float minDist, const float consumeParticle) {
            for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
                (**it).rotateAround(p, accel, minDist, consumeParticle);
            }
        }
            
        void applyVectorField(float * field, int fieldWidth, int fieldHeight, int numComponents, ofRectangle areaOfInfluence, float force) {
            for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
                ofxParticle & p = (**it);
                ofVec2f pos(p.position.x,p.position.y);
                if(areaOfInfluence.inside(pos)){
                    int x = (int)ofMap(pos.x, areaOfInfluence.getMinX(), areaOfInfluence.getMaxX(), 0, fieldWidth-1);
                    int y = (int)ofMap(pos.y, areaOfInfluence.getMinY(), areaOfInfluence.getMaxY(), 0, fieldHeight-1);
                    int index = (x+y*fieldWidth)*numComponents;
                    ofVec2f dir(field[index],field[index+1]);
                    dir.scale(force);
//                                    cout << "(" << dir.x << "," << dir.y << ")\n";
                    p.applyForce(dir);
                }
            }
        }
            
        int update(float timeStep, float drag){
            int particlesRemoved = 0;
            for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
                if((**it).isAlive()){
                    (**it).update(timeStep, drag);
                }
                else{
                    ofxParticle * p = (*it);
                    it = particles.erase(it);
                    delete p;
                    particlesRemoved++;
                }
            }
            numParticles-=particlesRemoved;
            return particlesRemoved;
        }
            
        void draw(){
            for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
                (**it).draw();
            }
        }

        void draw(ofTexture &tex){
            //                particles.sort();
            for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
                (**it).draw(tex);
            }
        }
            
        void draw(ofTexture &tex, ofTexture &tex2){
            //                particles.sort();
            for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
                int index = (**it).particleID;
                if(index%2==0)
                    (**it).draw(tex);
                else
                    (**it).draw(tex2);
            }
        }
            
        int getNumParticles(){return numParticles;}
            
    private:
        list<ofxParticle*> particles;
        int numParticles;
        int totalParticlesEmitted;
};