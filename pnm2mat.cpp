#include <iostream> 
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include "common/pnm.h"

using namespace std;
using namespace cv;

Mat *pnm2mat(pnm_t *pnm) {
    switch (pnm->format) {
        case PNM_FORMAT_GRAY:
            // TODO
            break;
        case PNM_FORMAT_RGB:
            // TODO
            break;
    }
}

int main() {
    pnm_t *pnm = pnm_create_from_file("../test-images/austin.pnm");
    cout << "Test image has bufflen " << pnm->buflen << endl;
    pnm_destroy(pnm);
    return 0;
}
