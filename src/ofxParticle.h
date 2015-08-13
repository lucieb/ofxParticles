#pragma once

#include "ofMain.h"

class ofxParticle {
public:
    ofVec3f position;
    ofVec3f velocity; // pixels/sec
    ofVec3f acceleration; // pixels/sec/sec
    ofColor color;
    
    ofVec3f rotation;
    ofVec3f rotationalVelocity;
    
    float mass;
    float size;
    float life;
    float lifeStart;
    float dt;
    int particleID;
    
    
    bool operator < (const ofxParticle &b){
        return position.z < b.position.z;
        }
        
        ofxParticle(){
            position = velocity = acceleration = ofVec3f(0,0,0);
            color = ofColor(255,255,255,255);
            mass = 1.0;
            size = 1.0;
            lifeStart = life = 1.0;
            particleID = 0;
            dt = 1.0/60;
        }
        
        ofxParticle(ofVec3f pos, ofVec3f vel, float size_, float life_){
            position = pos;
            velocity = vel;
            acceleration = ofVec3f(0,0,0);
            color = ofColor(255,255,255,255);
            mass = 1.0;
            size = size_;
            lifeStart = life = life_;
            particleID = 0;
            dt = 1.0/60;
        }
        
        ofxParticle(const ofxParticle &mom){
            position = mom.position;
            velocity = mom.velocity;
            acceleration = mom.acceleration;
            rotation = mom.rotation;
            rotationalVelocity = mom.rotationalVelocity;
            color = mom.color;
            mass = mom.mass;
            size = mom.size;
            life = mom.life;
            lifeStart = mom.lifeStart;
            particleID = mom.particleID;
            dt = 1.0/60;
        }
        
        ~ofxParticle(){}
        
        ofxParticle & operator=(const ofxParticle &mom){
            if(&mom==this) return *this;
            position = mom.position;
            velocity = mom.velocity;
            acceleration = mom.acceleration;
            rotation = mom.rotation;
            rotationalVelocity = mom.rotationalVelocity;
            color = mom.color;
            mass = mom.mass;
            size = mom.size;
            life = mom.life;
            lifeStart = mom.lifeStart;
            particleID = mom.particleID;
            dt = 1.0/60;
            return *this;
        }
        
        void applyForce(ofVec3f force){
            acceleration += force * dt;
        }
        
        void attractTo(ofxParticle p, const float accel, const float minDist, const bool consumeParticle){ attractTo(p.position, accel, minDist, consumeParticle); }
        void attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle){
            ofVec3f dir = p-position;
            float dist = dir.length();
            if(dist < minDist){
                dist = minDist;
                if(consumeParticle)
                    life = 0;
            }

            dir.normalize().scale(accel*dt);
            acceleration += dir;
        }
        
        void gravitateTo(ofxParticle p, const float gravity, const float minDist, const bool consumeParticle){ gravitateTo(p.position, gravity, p.mass, minDist, consumeParticle); }
        void gravitateTo(ofPoint p, const float gravity, const float mass2, const float minDist, const bool consumeParticle){
            if(p==position) return;
            ofVec3f dir = p-position;
            float dist = dir.length();
            if(dist < minDist){
                dist = minDist;
                if(consumeParticle)
                    life = 0;
            }
            dir.normalize().scale( gravity * mass * mass2 * dt * (1.0/(dist)));
            acceleration += dir;
        }
        
        void rotateAround(ofxParticle p, const float accel, const float minDist, const bool consumeParticle) { rotateAround(p.position, accel, minDist, consumeParticle); }
        void rotateAround(ofPoint p, const float accel, const float minDist, const bool consumeParticle){
            ofVec3f toPoint = position - p;
            ofVec3f dir = ofVec3f(-toPoint.y, toPoint.x, toPoint.z);
            float dist = toPoint.length();
            if(dist < minDist){
                dist = minDist;
                if(consumeParticle)
                    life = 0;
            }
            dir.normalize().scale((accel/dist) *dt);
            
            acceleration += dir;
        }
        
        void update(const float timeStep, const float drag){
            dt = timeStep;
            velocity += acceleration * dt;
            velocity -= (velocity * dt * (1.0-drag));
            position += velocity * dt;
            acceleration -= acceleration * dt;
            rotation += rotationalVelocity * dt;
            
            life -= dt;
        }
        
        void draw(){
            ofColor c = color;
            c.a = life/lifeStart*color.a;
            ofSetColor(c);
			glLineWidth(3.0);
            ofLine(position, position-velocity*dt);
        }
        
        void draw(ofTexture &tex){
            float w = tex.getWidth();
            float h = tex.getHeight();
            if(w > h){
                h = h/w*size;
                w = size;
            }
            else{
                w = w/h*size;
                h = size;
            }
            ofSetColor(color);
            ofPushMatrix();
            ofTranslate(position);
            ofRotateX(rotation.x);
            ofRotateY(rotation.y);
            ofRotateZ(rotation.z);
            tex.draw(w*-0.5,h*-0.5,w,h);
            ofPopMatrix();
        }
        
        bool isAlive() {return life > 0.0;}
        
};