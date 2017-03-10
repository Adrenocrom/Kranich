#include "kranich.h"


QImage mat_to_qimage_ref(cv::Mat &mat, QImage::Format format) {
  return QImage(mat.data, mat.cols, mat.rows, mat.step, format);
}


QImage mat_to_qimage_cpy(cv::Mat const &mat, QImage::Format format){
    return QImage(mat.data, mat.cols, mat.rows, mat.step, format).copy();
}



cv::Mat qimage_to_mat_ref(QImage &img, int format) {
    return cv::Mat(img.height(), img.width(), format, img.bits(), img.bytesPerLine());
}

cv::Mat qimage_to_mat_cpy(QImage const &img, int format) {    
    return cv::Mat(img.height(), img.width(), format, const_cast<uchar*>(img.bits()), img.bytesPerLine()).clone();
}
