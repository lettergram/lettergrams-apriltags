#include <iostream> 
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include "common/pnm.h"

using namespace std;
using namespace cv;

Mat pnm2mat(pnm_t *pnm) {
    Mat mat;
    switch (pnm->format) {
        case PNM_FORMAT_GRAY:
            cout << "converting grayscale pnm to cv mat" << endl;
            mat = Mat(pnm->height, pnm->width, CV_8UC1, pnm->buf);
            break;
        case PNM_FORMAT_RGB:
            cout << "converting color pnm to cv mat" << endl;
            mat = Mat(pnm->height, pnm->width, CV_8UC3, pnm->buf);
            cvtColor(mat, mat, CV_RGB2BGR);
            break;
    }

    return mat;
}

int main() {
    pnm_t *pnm = pnm_create_from_file("../test-images/austin.pnm");
    Mat mat = pnm2mat(pnm);
    namedWindow("display", WINDOW_AUTOSIZE);
    imshow("display", mat);
    waitKey(5000);
    pnm_destroy(pnm);
    return 0;
}
