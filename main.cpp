#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(){

	CvFont font;
	double hScale=0.7;
	double vScale=0.7;
	int    lineWidth=1;
	cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);

	char path[30];

	cout<<"Digite o caminho da imagem: ";

	cin>>path;

	IplImage* img =  cvLoadImage(path);

	int width, height, nchannels, step, offset;

	width = img->width;
	height = img->height;
	nchannels = img->nChannels;
	step = img->widthStep;

	for(int i = 0 ; i < height ; i++){
		uchar* data = (uchar*)(img->imageData + i*step);
		for(int  j = 0 ; j < width ; j++){

			if(i==0){
				offset = j * nchannels;
				data[offset] = 255;
				data[offset+1] = 255;
				data[offset+2] = 255;
			}

			if(j==0){
				offset = j * nchannels;
				data[offset] = 255;
				data[offset+1] = 255;
				data[offset+2] = 255;
			}

			if(i==height-1){
				offset = j * nchannels;
				data[offset] = 255;
				data[offset+1] = 255;
				data[offset+2] = 255;
			}

			if(j==width-1){
				offset = j * nchannels;
				data[offset] = 255;
				data[offset+1] = 255;
				data[offset+2] = 255;
			}
		}
	}

	cvNamedWindow("Raw");
	cvShowImage("Raw",img);

	IplImage* imgGrayScale = cvCreateImage(cvGetSize(img), 8, 1);
	cvCvtColor(img,imgGrayScale,CV_BGR2GRAY);

	int threshold;
	cout<<"Valor threshold: ";
	cin>>threshold;
	cvThreshold(imgGrayScale,imgGrayScale,threshold,255,1); //folha = 234, of = 100, gv = 150, c5 = 200, c4 = 55, c3 = 100

	cvDilate(imgGrayScale,imgGrayScale);
	cvErode(imgGrayScale,imgGrayScale);

	cvErode(imgGrayScale,imgGrayScale);
	cvDilate(imgGrayScale,imgGrayScale);

	cvNamedWindow("Raw2");
	cvShowImage("Raw2",imgGrayScale);

	CvSeq* contours;
	CvSeq* result;
	CvMemStorage *storage = cvCreateMemStorage(0);

	cvFindContours(imgGrayScale, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

	int cont = 0;
	char c[5];

	while(contours){

		result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);

		double area = cvContourArea(result);
		cout<<"Area do objeto "<<cont<<": "<<area<<endl;

		CvPoint *pt[result->total];
		for(int i=0;i<result->total;i++){
			pt[i] = (CvPoint*)cvGetSeqElem(result, i);
		}

		cvDrawContours(img, contours, Scalar(0, 0, 255), Scalar(0, 0, 255), 0, 2, 8);
		cont++;
		sprintf(c,"%i",cont);

		const char* obj = c;

		cvPutText (img,obj,*pt[result->total/2], &font, cvScalar(255,0,0));

		contours = contours->h_next;
	}

	cvNamedWindow("Tracked");
	cvShowImage("Tracked",img);

	cvWaitKey(0);

	cvDestroyAllWindows();
	cvReleaseMemStorage(&storage);
	cvReleaseImage(&img);
	cvReleaseImage(&imgGrayScale);

	return 0;
}
