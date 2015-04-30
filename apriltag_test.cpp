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
#include "pnm2mat.hpp" // functions to convert pnm to and from mat



int main(){

  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if camera opened
    return -1;
  
  Mat frame;
  
  while(1){
    
    clock_t t;
    t = clock();
    
    cap >> frame; // get a new frame from camera
    pnm_t *pnm = mat2pnm(&frame);
    frame = pnm2mat(pnm);
    
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    std::cout << "time to convert pnm: " << time_taken << ", ";
  
    
    imshow("Display Apriltags", frame);
    
    if(waitKey(30) >= 0) break;
    
    std::cout << endl;
  }
  
  return 0;
}
