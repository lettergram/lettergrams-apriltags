/* (C) 2013-2014, The Regents of The University of Michigan
   (C) 2015, Alterations made by Austin Walters
 All rights reserved.
 
 This software may be available under alternative licensing
 terms. Contact Edwin Olson, ebolson@umich.edu, for more information.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 */


#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <unistd.h>
#include <string>

#include "apriltag.h"
#include "common/image_u8.h"
#include "tag36h11.h"
#include "tag36h10.h"
#include "tag36artoolkit.h"
#include "tag25h9.h"
#include "tag25h7.h"

#include "common/zarray.h"
#include "common/getopt.h"

// Our extensions for chromatags
#include "pnm2mat.hpp" // functions to convert pnm to and from mat
#define ESC char(0x1B)


int main(){

  VideoCapture cap(0); // open the default camera
  if(!cap.isOpened())  // check if camera opened
    return -1;
  
  Mat frame;
  
  /* From apriltag_demo.c */
  
  int maxiters = 5;
  const int hamm_hist_max = 10;
  int quiet = 0;
  
  apriltag_family_t *tf = tag36h11_create();                // Apriltag family 36h11, can change
  tf->black_border = 1;                                     // Set tag family border size
  
  apriltag_detector_t *td = apriltag_detector_create();     // Apriltag detector
  apriltag_detector_add_family(td, tf);                     // Add apriltag family
  
  td->quad_decimate = 1.0;                                  // Decimate input image by factor
  td->quad_sigma = 0.0;                                     // No blur (I think)
  td->nthreads = 4;                                         // 4 treads provided
  td->debug = 0;                                            // No debuging output
  td->refine_decode = 0;                                    // Don't refine decode
  td->refine_pose = 0;                                      // Don't refine pose
  
  // Output variables
  char imgSize[20];
  char renderTime[20];
  char detect[50];
  char displayString[50];
  
  /* End of apriltag_demo.c */
  
  while(1){
    
    clock_t t;
    t = clock();
    
    cap >> frame;                                             // Get a new frame from camera

    pnm_t *pnm = mat2pnm(&frame);                             // Convert Mat fram to pnm
    image_u8_t *im = pnm_to_image_u8(pnm);                    // Convert pnm to gray image_u8
    if (im == NULL) {                                         // Error - no image created from pnm
      std::cout << "Error, not a proper pnm" << std::endl;
      return -1;
    }
    
    /*** Start from origional Apriltags from apriltag_demo.c ***/

    int hamm_hist[hamm_hist_max];
    memset(hamm_hist, 0, sizeof(hamm_hist));
    zarray_t *detections = apriltag_detector_detect(td, im);
    
    for (int i = 0; i < zarray_size(detections); i++) {
      
      apriltag_detection_t *det;
      zarray_get(detections, i, &det);
      
      sprintf(detect, "detection %2d: id (%2dx%2d)-%-4d, hamming %d, goodness %5.3f, margin %5.3f\n",
               i+1, det->family->d*det->family->d, det->family->h, det->id, det->hamming, det->goodness, det->decision_margin);

      hamm_hist[det->hamming]++;
      
      apriltag_detection_destroy(det);
    }
    
    if(zarray_size(detections) < 1){
      sprintf(detect, "No tag detected");
    }
    
    zarray_destroy(detections);
    image_u8_destroy(im);

    t = clock() - t;
    double time_taken = ((double)t)/(CLOCKS_PER_SEC/1000);
    //printf("ms to render: %5.3f\n", time_taken);
    
    if (!quiet) {
      //timeprofile_display(td->tp);
      sprintf(displayString, "fps: %2.2f, nedges: %d, nsegments: %d, nquads: %d\n",1000.0/time_taken,td->nedges,td->nsegments,td->nquads);
      //std::cout << displayString;
    }
    
    //for (int i = 0; i < hamm_hist_max; i++)
      //printf("%5d", hamm_hist[i]);
    
    sprintf(renderTime, "render: %5.3fms", time_taken);
    sprintf(imgSize, "%dx%d", frame.cols, frame.rows);
    printf("%s %s %s\r", detect, renderTime, imgSize);
    
    if (quiet) {
      printf("%12.3f", timeprofile_total_utime(td->tp) / 1.0E3);
    }
    
    printf("\n");
    
    /*** End of origional Apriltags from apriltag_demo.c ***/
    
    putText(frame, displayString, cvPoint(30,30),
            FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
    imshow("Display Apriltags", frame);
    
    if(waitKey(30) >= 0) break;
  }
  
  /* deallocate apriltag constructs */
  apriltag_detector_destroy(td);
  tag36h11_destroy(tf);
  
  return 0;
}
