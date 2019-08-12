#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <ctime>
#include <iostream>
#include <tbb/parallel_for.h>
#include <tbb/tbb.h>
#include <vector>
using namespace cv;
using namespace std;
#define kernel_size 5
int main(int argc, char **argv) {
  Mat src;
  try {
    src = imread("../pic.jpg", 0);
  } catch (runtime_error &ex) {
    cout << "[ERROR] can not find the image\n";
    return 1;
  }
  Mat dst0 = src.clone(); // sequential one
  Mat dst1 = src.clone(); // parallel one
  clock_t begin = clock();
  for (int i = 0; i < src.rows; i += kernel_size)
    for (int j = 0; j < src.cols; j += kernel_size) {
      int width = min(i + kernel_size, src.rows);
      int height = min(j + kernel_size, src.cols);
      Mat tile = dst0(Range(i, width), Range(j, height));
      GaussianBlur(tile, tile, Size(kernel_size, kernel_size), 0, 0);
    }
  cout << "elapsed time for sequential method: "
       << double(clock() - begin) / CLOCKS_PER_SEC << "\n";
  begin = clock();
  auto sze = Size(kernel_size, kernel_size);
  tbb::parallel_for(0, src.rows / kernel_size, [&](int i_) {
    tbb::parallel_for(0, src.cols / kernel_size, [&](int j_) {
      int i = i_ * kernel_size, j = j_ * kernel_size;
      int width = min(i + kernel_size, src.rows);
      int height = min(j + kernel_size, src.cols);
      Mat tile = dst1(Range(i, width), Range(j, height));
      GaussianBlur(tile, tile, sze, 0, 0);
    });
  });

  // tbb::parallel_for(0,src.rows*src.cols/kernel_size/kernel_size, [&](int k){
  //   int i=k%(src.rows/kernel_size);int j=k/(src.rows/kernel_size);
  //   int width =min(i+kernel_size,src.rows);
  //   int height=min(j+kernel_size,src.cols);
  //   Mat tile=dst1(Range(i,width),Range(j,height));
  //   GaussianBlur(tile,tile,Size(kernel_size,kernel_size),0,0);});
  cout << "elapsed time for parallel method: "
       << double(clock() - begin) / CLOCKS_PER_SEC << "\n";
  imshow("real image", src);
  imshow("sequential blured image", dst0);
  imshow("parallel blured image", dst1);
  waitKey(0);
  vector<int> compression_params;
  compression_params.push_back(IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(9);
  try {
    imwrite("../grey_blurred.png", dst1, compression_params);
  } catch (runtime_error &ex) {
    fprintf(stderr, "Exception converting image to PNG format: %s\n",
            ex.what());
    return 1;
  }
  return 0;
}
int min(int a, int b) {
  if (a > b)
    return b;
  return a;
}
