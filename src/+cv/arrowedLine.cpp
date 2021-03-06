/**
 * @file arrowedLine.cpp
 * @brief mex interface for cv::arrowedLine
 * @ingroup imgproc
 * @author Amro
 * @date 2015
 */
#include "mexopencv.hpp"
using namespace std;
using namespace cv;

/**
 * Main entry called from Matlab
 * @param nlhs number of left-hand-side arguments
 * @param plhs pointers to mxArrays in the left-hand-side
 * @param nrhs number of right-hand-side arguments
 * @param prhs pointers to mxArrays in the right-hand-side
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // Check the number of arguments
    nargchk(nrhs>=3 && (nrhs%2)==1 && nlhs<=1);

    // Argument vector
    vector<MxArray> rhs(prhs, prhs+nrhs);

    // Option processing
    Scalar color;
    int thickness = 1;
    int lineType = cv::LINE_8;
    int shift = 0;
    double tipLength = 0.1;
    for (int i=3; i<nrhs; i+=2) {
        string key(rhs[i].toString());
        if (key == "Color")
            color = (rhs[i+1].isChar()) ?
                ColorType[rhs[i+1].toString()] : rhs[i+1].toScalar();
        else if (key == "Thickness")
            thickness = (rhs[i+1].isChar()) ?
                ThicknessType[rhs[i+1].toString()] : rhs[i+1].toInt();
        else if (key == "LineType")
            lineType = (rhs[i+1].isChar()) ?
                LineType[rhs[i+1].toString()] : rhs[i+1].toInt();
        else if (key == "Shift")
            shift = rhs[i+1].toInt();
        else if (key == "TipLength")
            tipLength = rhs[i+1].toDouble();
        else
            mexErrMsgIdAndTxt("mexopencv:error",
                "Unrecognized option %s", key.c_str());
    }

    // Process
    Mat img(rhs[0].toMat());
    if (rhs[1].isNumeric() && rhs[1].numel() == 2) {
        Point pt1(rhs[1].toPoint()),
              pt2(rhs[2].toPoint());
        arrowedLine(img, pt1, pt2, color, thickness, lineType, shift, tipLength);
    }
    else {
        vector<Point> pt1(rhs[1].toVector<Point>()),
                      pt2(rhs[2].toVector<Point>());
        if (pt1.size() != pt2.size())
            mexErrMsgIdAndTxt("mexopencv:error", "Length mismatch");
        for (size_t i = 0; i < pt1.size(); ++i)
            arrowedLine(img, pt1[i], pt2[i],
                color, thickness, lineType, shift, tipLength);
    }
    plhs[0] = MxArray(img);
}
