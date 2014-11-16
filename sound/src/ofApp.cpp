#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255);
    
    int bufferSize		= 512;
    sampleRate 			= 44100;
    phase 				= 0;
    phaseAdder 			= 0.0f;
    phaseAdderTarget 	= 0.0f;
    volume				= 0.1f;
    bNoise 				= false;
    
    lAudio.assign(bufferSize, 0.0);
    rAudio.assign(bufferSize, 0.0);
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
    ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(100);
    ofDrawBitmapString("PRESSURE AND BEND", 50, 50);
    
    ofNoFill();
    
    // draw the left channel: ----------------------------------
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(50, 100, 0);
    
    ofSetColor(100);
    ofDrawBitmapString("Left Channel", 5, 20);
    
    ofSetColor(180);
    ofSetLineWidth(1);
    ofRect(0, 0, 400, 600);
    
    ofSetColor(66);
    ofSetLineWidth(1);
    
    ofBeginShape();
    for (unsigned int i = 0; i < lAudio.size(); i++){
        float x =  ofMap(i, 0, lAudio.size(), 0, 400, true);
        ofVertex(x, 300 -lAudio[i]*180.0f * 15);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
    // draw the right channel: ---------------------------------
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(500, 100, 0);
    
    ofSetColor(100);
    ofDrawBitmapString("Right Channel", 5, 20);
    
    ofSetColor(180);
    ofSetLineWidth(1);
    ofRect(0, 0, 400, 600);
    
    ofSetColor(66);
    ofSetLineWidth(1);
    
    ofBeginShape();
    for (unsigned int i = 0; i < rAudio.size(); i++){
        float x =  ofMap(i, 0, rAudio.size(), 0, 400, true);
        ofVertex(x, 300 -rAudio[i]*180.0f * 15);
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    if (key == '-' || key == '_' ){
        volume -= 0.05;
        volume = MAX(volume, 0);
    } else if (key == '+' || key == '=' ){
        volume += 0.05;
        volume = MIN(volume, 1);
    }
    
    if( key == 's' ){
        soundStream.start();
    }
    if( key == 'e' ){
        soundStream.stop();
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    int width = ofGetWidth();
    pan = (float)x / (float)width;
    float height = (float)ofGetHeight() / 2;
    float heightPct = ((height-y) / height);
    targetFrequency = 2000.0f * heightPct;
    phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
    
    ofDrawBitmapString(ofToString(heightPct), 200, 50);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bNoise = true;
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bNoise = false;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels){
    float leftScale = 1 - pan;
    float rightScale = pan;
    
    while (phase > TWO_PI){
        phase -= TWO_PI;
    }
    
    if ( bNoise == true){
        // ---------------------- noise --------------
        for (int i = 0; i < bufferSize; i++){
            lAudio[i] = output[i*nChannels    ] = ofRandom(0, 1) * volume * leftScale;
            rAudio[i] = output[i*nChannels + 1] = ofRandom(0, 1) * volume * rightScale;
        }
    } else {
        phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
        for (int i = 0; i < bufferSize; i++){
            phase += phaseAdder;
            float sample = sin(phase);
            lAudio[i] = output[i*nChannels    ] = sample * volume * leftScale;
            rAudio[i] = output[i*nChannels + 1] = sample * volume * rightScale;
        }
    }
}


//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
