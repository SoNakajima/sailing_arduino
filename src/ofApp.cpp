#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
//    	ard.connect("/dev/cu.usbmodem1421", 9600);
    
    ard.connect("/dev/cu.usbmodem1421", 57600);
//    ofAddListener(ard.EInitialized, this, ofApp::setupArduino);

    ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
    bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
    
    setupLight();
    
    material.setShininess( 120 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    
    
    
    iMainCamera = 0;
    bCamParent = false;
    
    
    cameras[0] = &cam;
    
    
    // front
    camFront.scale = 20;
    cameras[1] = &camFront;
    
    // top
    camTop.scale = 20;
    camTop.tilt(-90);
    cameras[2] = &camTop;
    
    // left
    camLeft.scale = 20;
    camLeft.pan(-90);
    cameras[3] = &camLeft;
    
    //
    //--
    
    
    
    //--
    // Define viewports
    
    float xOffset = ofGetWidth() / 3;
    float yOffset = ofGetHeight() / N_CAMERAS;
    
    viewMain.x = xOffset;
    viewMain.y = 0;
    viewMain.width = xOffset * 2;
    viewMain.height = ofGetHeight();
    
    for(int i = 0; i < N_CAMERAS; i++){
        
        viewGrid[i].x = 0;
        viewGrid[i].y = yOffset * i;
        viewGrid[i].width = xOffset;
        viewGrid[i].height = yOffset;
    }
    
    
}


void ofApp::setupLight(){
    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    
    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));
    
    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );
    
    ofEnableLighting();
    pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();

}

void ofApp::setupArduino(const int & version) {
	ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    
    ard.sendAnalogPinReporting(1, ARD_ANALOG);
    ard.sendAnalogPinReporting(3, ARD_ANALOG);
    ard.sendAnalogPinReporting(5, ARD_ANALOG);
    
    bSetupArduino = true;
    
}

void ofApp::updateArduino(){
    
    // update the arduino, get any data or messages.
    // the call to ard.update() is required
    ard.update();
    
    
    
}


//--------------------------------------------------------------
void ofApp::update(){

    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
    

    updateArduino();
    int x,y,z;
    x = y = z =0;
    if (bSetupArduino) {
            x = ard.getAnalog(1);
            y = ard.getAnalog(3);
            z = ard.getAnalog(5);

        
        rotateX = (x-275)/2.58 - 90;
        rotateY = (y-267)/2.59 - 90;
        rotateZ = (z-275)/2.58 - 90;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(64), ofColor(0));
    
    
    
    ofDisableDepthTest();
    ofPushStyle();
    ofSetColor(100, 100, 100);
    ofDrawRectangle(viewGrid[iMainCamera]);
    ofPopStyle();
    ofEnableDepthTest();
    
    
    //--
    // Draw all viewports
    
    // draw main viewport
    cameras[iMainCamera]->begin(viewMain);
    drawScene(iMainCamera);
    
    
    cameras[iMainCamera]->end();
    
    // draw side viewports
    for(int i = 0; i < N_CAMERAS; i++){
        cameras[i]->begin(viewGrid[i]);
        drawScene(i);
        cameras[i]->end();
    }
    
    
    
    
    cam.begin();
    
    ofPushMatrix();

    ofRotateZ(rotateX);
    ofRotateX(rotateY);
    material.begin();
    box.draw();
    material.end();
    
    ofPopMatrix();
    
    cam.end();
    
    
    
    
    
    // draw outlines on views
    ofSetLineWidth(5);
    ofNoFill();
    ofSetColor(255, 255, 255);
    //
    for(int i = 0; i < N_CAMERAS; i++){
        ofDrawRectangle(viewGrid[i]);
    }
    //
    ofDrawRectangle(viewMain);
    
    // restore the GL depth function
    glDepthFunc(GL_LESS);
    ofPopStyle();

    


}

void ofApp::drawScene(int iCameraDraw){
    
 //   nodeGrid.draw();
    
    if(iCameraDraw != 0){
        
        ofPushStyle();
        ofPushMatrix();
        ofRectangle boundsToUse;
        if(iMainCamera == 0){
            boundsToUse = viewMain;
        }
        else{
            boundsToUse = viewGrid[0];
        }
        
        // Now lets get the inverse ViewProjection
        //  for the camera
        ofMatrix4x4 inverseCameraMatrix;
        inverseCameraMatrix.makeInvertOf(cam.getModelViewProjectionMatrix(boundsToUse));
        // By default, we can say
        //	'we are drawing in world space'
        //
        // The camera matrix performs
        //	world->camera
        //
        // The inverse camera matrix performs
        //	camera->world
        //
        // Our box is in camera space, if we
        //	want to draw that into world space
        //	we have to apply the camera->world
        //	transformation.
        //
        
        // This syntax is a little messy.
        // What it's saying is, send the data
        //  from the inverseCameraMatrix object
        //  to OpenGL, and apply that matrix to
        //  the current OpenGL transform
        ofMultMatrix( inverseCameraMatrix );
        ofNoFill();
        // i.e. a box -1, -1, -1 to +1, +1, +1
        ofDrawBox(0, 0, 0, 2.0f);
        //
        //--
        
        ofPopStyle();
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
