

#include "ofApp.h"
#include <math.h>
#include <vtkXMLImageDataReader.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>

#include <vtkMarchingCubes.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>


//--------------------------------------------------------------
void ofApp::setup(){
  //some visual setup
  ofSetVerticalSync(true);
  ofEnableSmoothing();


  //do the data reading, note that the filename is hardcoded
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
  ///reader->SetFileName(ofToDataPath("foo.vti").c_str());
  reader->Update();

  image = reader->GetOutput();

  int dims[3];
  image->GetDimensions(dims);
    width=dims[0];
    height=dims[1];
    depth=dims[2];
  std::cout << "Image dimensions: " << dims[0] << "x" << dims[1] << "x" << dims[2] << std::endl;
    scalars = image->GetPointData()->GetScalars();
  //we use bbox_pos to orient the surface
  bbox_pos = ofVec3f(0,0,0);
  bbox_size = ofVec3f(dims[0]-1,dims[1]-1,dims[2]-1);
  //recenter the bbox
  bbox_pos = 0.5*(bbox_size-bbox_pos);
  //center the camera around the bbox
  cam.setAutoDistance(false);
  cam.setTarget(bbox_pos);

  double range[2];
  image->GetPointData()->GetScalars()->GetRange(range);
  std::cout << "Scalar value range: [" << range[0] << "," << range[1] << "]" << std::endl;
    data = new float[dims[0]*dims[1]*dims[2]];
    int i=0;
    int i2=0;
    for (int z=0; z<dims[2]; z++) {
        for (int y=0; y<dims[1]; y++) {
            for(int x=0; x<dims[0]; x++) {
                    data[i2] = scalars->GetTuple1(i);
                    i2++;
                    i++;
            }
        }
    }
    
  //now that we've read the data, set up the gui
  //we'll make the slider have a callback to re-run MC
  isovalueSlider.addListener(this, &ofApp::run_mc);

  guiPanel.setup();
  guiPanel.add(isovalueSlider.setup("isovalue", 0.5*(range[1]-range[0]) + range[0], range[0], range[1]));
  guiPanel.add(bboxToggle.setup("Draw Bounding Box", true));
}

int ofApp::indexTo1d(int a,int b,int c){
    return(c*width*height+b*width+a);
}
int ofApp::interpolate(){
    float a,b,value;
    return (min(a,b)-value)/(max(a,b)-value);


}

void ofApp::run_mc(float& isovalue) {
  mesh.clear();
    ofVec3f vertlist[12];
    int i=0;
    int index=0;
    for (int z=0; z<depth-1; z++) {
        for (int y=0; y<height-1; y++) {
            for(int x=0; x<width-1; x++) {
                
                index=0;
                i=0;
                v0=data[indexTo1d(x,y,z)];
                if(v0>isovalue)
                index=index+ pow (2.0,i);
                i++;
                v1=data[indexTo1d(x+1,y,z)];
                if(v1>isovalue)
                index=index+ pow (2.0,i);
                i++;
                v2=data[indexTo1d(x+1,y,z+1)];
                if(v2>isovalue)
                index=index+ pow (2.0,i);
                i++;
                v3=data[indexTo1d(x,y,z+1)];
                if(v3>isovalue)
                index=index+ pow (2.0,i);
                i++;
                v4=data[indexTo1d(x,y+1,z)];
                if(v4>isovalue)
                index=index+ pow (2.0,i);
                i++;
                v5=data[indexTo1d(x+1,y+1,z)];
                if(v5>isovalue)
                index=index+ pow (2.0,i);
                i++;
                v6=data[indexTo1d(x+1,y+1,z+1)];
                if(v6>isovalue)
                index=index+ pow (2.0,i);
                i++;
                v7=data[indexTo1d(x,y+1,z+1)];
                if(v7>isovalue)
                index=index+ pow (2.0,i);

                /* Find the vertices where the surface intersects the cube */
                /* Find the vertices where the surface intersects the cube */
                if (edgeTable[index] & 1)
                vertlist[0] = VertexInterp(isovalue,x,y,z,x+1,y,z,v0,v1);
                if (edgeTable[index] & 2)
                vertlist[1] = VertexInterp(isovalue,x+1,y,z,x+1,y,z+1,v1,v2);
                if (edgeTable[index] & 4)
                vertlist[2] = VertexInterp(isovalue,x+1,y,z+1,x,y,z+1,v2,v3);
                if (edgeTable[index] & 8)
                vertlist[3] = VertexInterp(isovalue,x,y,z+1,x,y,z,v3,v0);
                if (edgeTable[index] & 16)
                vertlist[4] = VertexInterp(isovalue,x,y+1,z,x+1,y+1,z,v4,v5);
                if (edgeTable[index] & 32)
                vertlist[5] = VertexInterp(isovalue,x+1,y+1,z,x+1,y+1,z+1,v5,v6);
                if (edgeTable[index] & 64)
                vertlist[6] = VertexInterp(isovalue,x+1,y+1,z+1,x,y+1,z+1,v6,v7);
                if (edgeTable[index] & 128)
                vertlist[7] = VertexInterp(isovalue,x,y+1,z+1,x,y+1,z,v7,v4);
                if (edgeTable[index] & 256)
                vertlist[8] = VertexInterp(isovalue,x,y,z,x,y+1,z,v0,v4);
                if (edgeTable[index] & 512)
                vertlist[9] = VertexInterp(isovalue,x+1,y,z,x+1,y+1,z,v1,v5);
                if (edgeTable[index] & 1024)
                vertlist[10] = VertexInterp(isovalue,x+1,y,z+1,x+1,y+1,z+1,v2,v6);
                if (edgeTable[index] & 2048)
                vertlist[11] = VertexInterp(isovalue,x,y,z+1,x,y+1,z+1,v3,v7);
                
                for (i=0;triTable[index][i]!=-1;i+=3) {
                    ofColor color(240, 59,32);
                    ofVec3f v(vertlist[triTable[index][i]].x,vertlist[triTable[index][i]].y,vertlist[triTable[index][i]].z);
                    ofVec3f v2(vertlist[triTable[index][i+1]].x,vertlist[triTable[index][i+1]].y,vertlist[triTable[index][i+1]].z);
                    ofVec3f v3(vertlist[triTable[index][i+2]].x,vertlist[triTable[index][i+2]].y,vertlist[triTable[index][i+2]].z);
                    mesh.addVertex(v);
                    mesh.addVertex(v2);
                    mesh.addVertex(v3);
//                    mesh.addColor(color);
//                    mesh.addColor(color);
//                    mesh.addColor(color);
                }

            }
        }
    }
    
  vbo_mesh = mesh;

}
    ofVec3f ofApp::VertexInterp(float isovalue,int x,int y,int z,int x2,int y2,int z2,float valp1,float valp2){
        double mu;
        ofVec3f p1(x, y, z);
        ofVec3f p2(x2, y2, z2);
        
        if (ABS(isovalue-valp1) < 0.00001)
            return(p1);
        if (ABS(isovalue-valp2) < 0.00001)
            return(p2);
        if (ABS(valp1-valp2) < 0.00001)
            return(p1);
        mu = (isovalue - valp1) / (valp2 - valp1);
        ofVec3f p(x + mu * (x2 - x), y + mu * (y2 - y), z + mu * (z2 - z));
        return(p);
    
    
    
    }

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
  //First draw all the 3d geometry
  ofPushMatrix();
  ofPushStyle();
  ofEnableDepthTest();
  cam.begin();

  ofSetColor(0);
  ofNoFill();
  if (bboxToggle) {
    ofDrawBox(bbox_pos,bbox_size[0],bbox_size[1],bbox_size[2]);
  }
  
  //This is OpenGL code to make sure the wireframe mesh and the solid
  //triangular mesh are subtlely offset and draw nicer
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1,-1);
  
  ofSetColor(0);
  vbo_mesh.drawWireframe();

  ofSetColor(240, 59,32);
  vbo_mesh.draw();

  cam.end();
  ofDisableDepthTest();
  ofPopStyle();
  ofPopMatrix();

  //finally, draw the GUI
  guiPanel.draw();
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
