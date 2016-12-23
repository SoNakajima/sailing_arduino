#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "Grid.h"
#include "OrthoCamera.h"

#define N_CAMERAS 4

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    void setupLight();

    int rotateX, rotateY, rotateZ;
    ofEasyCam cam;

    
    ofBoxPrimitive box;
    ofConePrimitive cone;
    
    ofArduino ard;
    
    bool bSetupArduino;
    void setupArduino(const int & version);
    void updateArduino();
    
    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;

    ofMaterial material;
    
    
    
    void drawScene(int iCameraDraw);
    
    bool bCamParent;
    orthoCamera camFront;
    orthoCamera camTop;
    orthoCamera camLeft;
    ofCamera * cameras[N_CAMERAS];
    int iMainCamera;
    
    ofRectangle viewMain;
    ofRectangle viewGrid[N_CAMERAS];

    grid nodeGrid;
    
    bool drawGrid;
    
    ofImage image,image2;
    
    int count;
    bool photoChange;
    
};
