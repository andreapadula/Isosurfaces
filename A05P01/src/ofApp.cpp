#include "ofApp.h"
#include <vtkXMLImageDataReader.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>


//--------------------------------------------------------------
void ofApp::setup(){


    vtkSmartPointer<vtkXMLImageDataReader> reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
    string path;
    ofFileDialogResult result = ofSystemLoadDialog("Load file");
    if(result.bSuccess) {
        path = result.getPath();
         reader->SetFileName((path).c_str());
    }
    else{
        return;
    }
   
    reader->Update();
    
    vtkSmartPointer<vtkImageData> image = reader->GetOutput();
    int dims[3];
    image->GetDimensions(dims);
    std::cout << "Image dimensions: " << dims[0] << "x" << dims[1] << "x" << dims[2] << std::endl;
    width=dims[0];
    depth=dims[2];
    height=dims[1];

    scalars = image->GetPointData()->GetScalars();
    

    double range[2];
    scalars->GetRange(range);
    min=range[0];
    max=range[1];
    std::cout << "Scalar value range: [" << range[0] << "," << range[1] << "]" << std::endl;

    xSlicer.addListener(this, &ofApp::slicer);
    
    ySlicer.addListener(this, &ofApp::slicer2);
    
    zSlicer.addListener(this, &ofApp::slicer3);

    sx.addListener(this, &ofApp::setX);
    sy.addListener(this, &ofApp::setY);
    sz.addListener(this, &ofApp::setZ);
    
    guiPanel.setup();
    //guiPanel.add(toggle);
    guiPanel.add(sx.setup("ACTIVE"));
    ofColor red;
    red.r=255;
    red.g=0;
    red.b=0;
    sx.setBackgroundColor(red);
    guiPanel.add(xSlicer.setup("SLICE X:", dims[0]/2, 0, dims[0]));
    guiPanel.add(sy.setup("NOT ACTIVE"));
    guiPanel.add(ySlicer.setup("SLICE Y:", dims[1]/2, 0, dims[1]));
    guiPanel.add(sz.setup("NOT ACTIVE"));
    guiPanel.add(zSlicer.setup("SLICE Z:", dims[2]/2, 0, dims[2]));
//
    //ofSetHexColor(0xffffff);

    
}



void ofApp::slicer(int& slice) {
    if (sx2==false) {
        return;
    }
    float *data = new float[3*height*depth];
    
    //int value=slice;
    int i=0,i2=0;
    for (int z=0; z<depth; z++) {
        for (int y=0; y<height; y++) {
            for(int x=0; x<width; x++) {
                int current=x;
                if (current==slice) {
                    ofColor r= setColor(scalars->GetTuple1(i));
                    //cout<<r.r<<endl;
                    data[3*i2+0] = (float(r.r)) / (255);
                    data[3*i2+1] = (float(r.g)) / (255);
                    data[3*i2+2] = (float(r.b)) / (255);
                    i2++;
                }
                i++;
            }
        }
    }

    
    ////Finally, store the data in the ofFloatImage
    test_image.setFromPixels(data, height, depth, OF_IMAGE_COLOR);
     test_image.resize(512, 512);
    
    
}
void ofApp::slicer2(int& slice) {
    if (sy2==false) {
        return;
    }
    float *data = new float[3*height*depth];
    
    //int value=slice;
    int i=0,i2=0;
    for (int z=0; z<depth; z++) {
        for (int y=0; y<height; y++) {
            for(int x=0; x<width; x++) {
                int current;
                current=y;
                if (current==slice) {
                    ofColor r= setColor(scalars->GetTuple1(i));
                    //cout<<r.r<<endl;
                    data[3*i2+0] = (float(r.r)) / (255);
                    data[3*i2+1] = (float(r.g)) / (255);
                    data[3*i2+2] = (float(r.b)) / (255);
                    i2++;
                }
                i++;
            }
        }
    }
    
    
    ////Finally, store the data in the ofFloatImage
    test_image.setFromPixels(data, height, depth, OF_IMAGE_COLOR);
     test_image.resize(512, 512);
    
    
}
void ofApp::slicer3(int& slice) {
    if (sz2==false) {
        return;
    }
    float *data = new float[3*height*depth];
    
    //int value=slice;
    int i=0,i2=0;
    for (int z=0; z<depth; z++) {
        for (int y=0; y<height; y++) {
            for(int x=0; x<width; x++) {
                int current;
                current=z;
                if (current==slice) {
                    ofColor r= setColor(scalars->GetTuple1(i));
                    //cout<<r.r<<endl;
                    data[3*i2+0] = (float(r.r)) / (255);
                    data[3*i2+1] = (float(r.g)) / (255);
                    data[3*i2+2] = (float(r.b)) / (255);
                    i2++;
                }
                i++;
            }
        }
    }
    
    
    ////Finally, store the data in the ofFloatImage
    test_image.setFromPixels(data, height, depth, OF_IMAGE_COLOR);
    test_image.resize(512, 512);
    
}
ofColor ofApp::setColor(float num){
    if(map1){
    if (num>max/2 && num<=max) {
        ofColor color;
        color.r=ofMap(num, min, max, 247, 202);
        color.g=ofMap(num, min, max, 247, 0);
        color.b=ofMap(num, min, max, 247, 32);
        return color;
    } else {
        ofColor color;
        color.r=ofMap(num, min, max, 5, 247);
        color.g=ofMap(num, min, max, 113, 247);
        color.b=ofMap(num, min, max, 176, 247);
        return color;
    }
    }
    if(map2){
        ofColor color;
        color.r=ofMap(num, min, max, 227, 254);
        color.g=ofMap(num, min, max, 74, 232);
        color.b=ofMap(num, min, max, 51, 200);
        return color;
    }
    if(map3){
        ofColor color;
        color.r=ofMap(num, min, max, 37, 247);
        color.g=ofMap(num, min, max, 37, 247);
        color.b=ofMap(num, min, max, 37, 247);
        return color;
    }
    if(map4){
        if (num>max/2 && num<=max) {
            ofColor color;
            color.r=ofMap(num, min, max, 247, 123);
            color.g=ofMap(num, min, max, 247, 50);
            color.b=ofMap(num, min, max, 247, 148);
            return color;
        } else {
            ofColor color;
            color.r=ofMap(num, min, max, 0, 247);
            color.g=ofMap(num, min, max, 136, 247);
            color.b=ofMap(num, min, max, 55, 247);
            return color;
        }
    }

}
void ofApp::setX(){
    sx.setName("ACTIVE");
    sy.setName("NOT ACTIVE");
    sz.setName("NOT ACTIVE");
    ofColor red;
    red.r=255;
    red.g=0;
    red.b=0;
    ofColor black;
    black.r=0;
    black.g=0;
    black.b=0;
    sx.setBackgroundColor(red);
    sy.setBackgroundColor(black);
    sz.setBackgroundColor(black);
    sx2=true;
    sy2=false;
    sz2=false;
}
void ofApp::setY(){
    sx.setName("NOT ACTIVE");
    sy.setName("ACTIVE");
    sz.setName("NOT ACTIVE");
    ofColor red;
    red.r=255;
    red.g=0;
    red.b=0;
    ofColor black;
    black.r=0;
    black.g=0;
    black.b=0;
    sx.setBackgroundColor(black);
    sy.setBackgroundColor(red);
    sz.setBackgroundColor(black);
    sx2=false;
    sy2=true;
    sz2=false;
}
void ofApp::setZ(){
    sz.setName("ACTIVE");
    sx.setName("NOT ACTIVE");
    sy.setName("NOT ACTIVE");
    ofColor red;
    red.r=255;
    red.g=0;
    red.b=0;
    ofColor black;
    black.r=0;
    black.g=0;
    black.b=0;
    sx.setBackgroundColor(black);
    sy.setBackgroundColor(black);
    sz.setBackgroundColor(red);
    sx2=false;
    sy2=false;
    sz2=true;
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
    
    ofSetColor(255,255,255);
    test_image.draw(400,100);
    guiPanel.draw();
    ofFill();
    legend();

}

void ofApp::legend(){
    for (int color=0; color<=255; color+=15){
        //ofSetColor(color);
        ofColor c;
        if (color <255/2){
            
            c.r=ofMap(color, 0, 255/2, 5, 247);
            c.g=ofMap(color, 0, 255/2, 113, 247);
            c.b=ofMap(color, 0, 255/2, 176, 247);
        }
        else{
            c.r=ofMap(color, 255/2, 255, 247, 202);
            c.g=ofMap(color, 255/2, 255, 247, 0);
            c.b=ofMap(color, 255/2, 255, 247, 32);
        }
        ofSetColor(c);
        ofRect(20+color,500,20,20);
    }
    ofSetColor(0);
    int  nearest = min;
    int  nearest2 = max;
    ofDrawBitmapString(std::to_string(nearest), 10, 530);
    ofDrawBitmapString(std::to_string(nearest2), 280, 530);
    ///////////////////////////////////////////color 2
    for (int color=0; color<=255; color+=15){
        ofColor c;
        c.r=ofMap(color,min, max, 227, 254);
        c.g=ofMap(color,min, max, 74, 232);
        c.b=ofMap(color, min, max, 51, 200);
        ofSetColor(c);
        ofRect(20+color,550,20,20);
    }
    ofSetColor(0);
    nearest = min;
    nearest2 = max;
    ofDrawBitmapString(std::to_string(nearest), 10, 580);
    ofDrawBitmapString(std::to_string(nearest2), 280, 580);
    ///////////////////////////////////////////color 3
    for (int color=0; color<=255; color+=15){
        ofColor c;
        c.r=ofMap(color,min, max, 37, 247);
        c.g=ofMap(color,min, max, 37, 247);
        c.b=ofMap(color, min, max, 37, 247);
        ofSetColor(c);
        ofRect(20+color,450,20,20);
    }
    ofSetColor(0);
    nearest = min;
    nearest2 = max;
    ofDrawBitmapString(std::to_string(nearest), 10, 480);
    ofDrawBitmapString(std::to_string(nearest2), 280, 480);
    ///////////////////////////////////////////color 4
    for (int color=0; color<=255; color+=15){
        //ofSetColor(color);
        ofColor c;
        if (color <255/2){
            
            c.r=ofMap(color, 0, 255/2, 0, 247);
            c.g=ofMap(color, 0, 255/2, 136, 247);
            c.b=ofMap(color, 0, 255/2, 55, 247);
        }
        else{
            c.r=ofMap(color, 255/2, 255, 247, 123);
            c.g=ofMap(color, 255/2, 255, 247, 50);
            c.b=ofMap(color, 255/2, 255, 247, 148);
        }
        ofSetColor(c);
        ofRect(20+color,600,20,20);
    }
    ofSetColor(0);
    nearest = min;
    nearest2 = max;
    ofDrawBitmapString(std::to_string(nearest), 10, 630);
    ofDrawBitmapString(std::to_string(nearest2), 280, 630);

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
    if(ofInRange(x,20,275)&& ofInRange(y,500,520)){
        map1=true;
        map2=false;
        map3=false;
        map4=false;
        return;
    }
    if(ofInRange(x,20,275)&& ofInRange(y,550,570)){
        map1=false;
        map2=true;
        map3=false;
        map4=false;
        return;
    }
    if(ofInRange(x,20,275)&& ofInRange(y,450,470)){
        map1=false;
        map2=false;
        map3=true;
        map4=false;
        return;
    }
    if(ofInRange(x,20,275)&& ofInRange(y,600,620)){
        map1=false;
        map2=false;
        map3=false;
        map4=true;
        return;
    }
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
