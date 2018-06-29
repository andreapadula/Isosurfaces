#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

class ofApp : public ofBaseApp {

  public:
    void setup();
    void slicer(int& value);
    void slicer2(int& value);
    void slicer3(int& value);
    void update();
    void draw();
    ofColor setColor(float num);
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
    vtkSmartPointer<vtkImageData> image;
    ofFloatImage test_image;
    vtkSmartPointer<vtkDataArray> scalars;
    int width,height,depth;
    double max ,min;
    ofxButton sx,sy,sz;
    bool sx2=true,sy2=false,sz2=false;
    void setX();
    void setY();
    void setZ();
    void legend();
    ofxPanel guiPanel;
    ofxIntSlider xSlicer;
    ofxIntSlider ySlicer;
    ofxIntSlider zSlicer;
    bool map1=true;
    bool map2;
    bool map3;
    bool map4;
    

};
