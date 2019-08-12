#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
// #include <opencv2/core/matx.hpp>
#include <vector>
#include <iostream>
#define Threshold 10
using namespace cv;
int main(){
  Mat pic0=imread("../pic0.png");
  Mat pic1=imread("../pic1.png");
  if(pic0.empty() || pic1.empty()){
    std::cout << "\033[1;31m no such file!! \033[0m\n" << '\n';
    return 1;}
  if(pic0.rows!=pic1.rows || pic1.cols!=pic0.cols){
    std::cout << "\033[1;31m two pictures r not same size\033[0m\n" << '\n';
    return 1;}
  if(pic0.channels()!=pic1.channels()){
    std::cout << "\033[1;31m channels count are not same\033[0m\n" << '\n';
  }
  Mat pic2=pic0.clone();
  imshow("pic0",pic0);
  imshow("pic1",pic1);
  Vec3b pic0_pixel,pic1_pixel,white={255,255,255};
  float white_pixel_count=0;
  for(int i=0;i<pic0.rows;i++)
    for(int j=0;j<pic0.cols;j++){
      // std::cout<<i<<","<<j<<"\n";
      pic1_pixel = pic1.at<Vec3b>(i, j);
      pic0_pixel = pic0.at<Vec3b>(i, j);
      Vec3b &pic2_pixel = pic2.at<Vec3b>(i, j);
      bool is_black=true;
      for(int k = 0; k < pic0.channels(); k++){
        int t=abs((int)pic0_pixel[k]-(int)pic1_pixel[k]);
        if(t<=Threshold) is_black=false;
        pic2_pixel[k]=t;}
      if(!is_black){
        pic2_pixel=white;
        white_pixel_count++;
      /*std::cout << "white" << '\n';*/}
      else{
        std::cout <<"- ("<<i<<","<<j<<")\n";
      }
      // std::cout << pic2_pixel << '\n';
    }
    std::cout<<"white pixel percentage is:"<<white_pixel_count/(pic0.rows*pic0.cols)*100<<"\n";
    imshow("res",pic2);
    waitKey(0);
    std::vector<int> compression_params;
    compression_params.push_back(IMWRITE_PNG_COMPRESSION);compression_params.push_back(9);
    try{
        imwrite("../RGB_processed.png", pic2, compression_params);}
    catch(std::runtime_error& ex){
         std::cout << "\033[1;31mException converting image to PNG format:"<<ex.what()<< "\033[0m\n";
        return 1;}
  return 0;}
int abs_minus(int a){
  if(a<0) return -1*a;
  return a;
}
