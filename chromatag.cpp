#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <unistd.h>

#include "apriltag.h"
#include "image_u8.h"
#include "tag36h11.h"
#include "tag36h10.h"
#include "tag36artoolkit.h"
#include "tag25h9.h"
#include "tag25h7.h"

#include "zarray.h"
#include "getopt.h"

// Our extensions for chromatags
#include "rgb2lab.hpp" // functions to convert to rgb to lab, and seperate color channels
#include "pnm2mat.hpp" // functions to convert pnm to and from mat

int main(){

  bool showGradient = false;
  
  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if camera opened
    return -1;
  
  Mat a, b, g, frame;
  
  while(1){
    
    clock_t t;
    t = clock();
    
    cap >> frame; // get a new frame from camera
    Mat imgLab =  RGB2LAB(frame);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    std::cout << "time to convert: " << time_taken << ", ";
    
    a = alphaLAB(imgLab);
    b = betaLAB(imgLab);
    
    t = clock();
    
    if(showGradient){
      g = gradientEdges(a);
    }else{
      g = a;
    }
    
    pnm_t *pnm = mat2pnm(&g);
    g = pnm2mat(pnm);

    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC;
    std::cout << "time to find gradient: " << time_taken << ", ";
    
    imshow("Display Edges", g);
    
    if(waitKey(30) >= 0){ break; }

    std::cout << std::endl;
    
  }

  return 0;
}
