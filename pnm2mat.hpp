#include <iostream> 
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "common/pnm.h"

using namespace std;
using namespace cv;

Mat pnm2mat(const pnm_t *pnm) {
    Mat mat;
    switch (pnm->format) {
        case PNM_FORMAT_GRAY:
            mat = Mat(pnm->height, pnm->width, CV_8UC1, pnm->buf);
            break;
        case PNM_FORMAT_RGB:
            mat = Mat(pnm->height, pnm->width, CV_8UC3, pnm->buf);
            cvtColor(mat, mat, CV_RGB2BGR);
            break;
    }

    return mat;
}

pnm_t *mat2pnm(Mat *mat) {
   pnm_t *pnm;

   cvtColor(*mat, *mat, CV_BGR2RGB);

   int width = mat->cols;
   int height = mat->rows;
   int format = PNM_FORMAT_RGB;
   int buflen = (mat->cols*mat->channels())*mat->rows;

   uchar *orig_data = (uchar *) mat->data;

   pnm = (pnm_t*) malloc(sizeof(pnm_t));
   if (!pnm)
       return 0;

   uchar *buf = (uchar *) malloc(buflen);
   if (!buf)
       return 0;
   memcpy(buf, orig_data, buflen);

   pnm->width = width;
   pnm->height = height;
   pnm->buf = buf;
   pnm->buflen = buflen;
   pnm->format = format;
   return pnm;
}

void showpnm() {
    VideoCapture cap(0);
    if (!cap.isOpened())
      return;
    
    Mat frame;
    cap >> frame;
    pnm_t *pnm = mat2pnm(&frame);
    Mat res = pnm2mat(pnm);

    namedWindow("display", WINDOW_AUTOSIZE);
    imshow("display", res);
    waitKey(5000);
    destroyWindow("display");
}
