/*********************************************************************
 * @file       ImageEditorDriverTwo.cpp
 * @brief      
 *
 * @details    
 *
 * @author     Joseph Lan
 * @date       18 October 2021
 *
 * CSS 487
 * Program 2
 * Prof. Clark Olson
 *********************************************************************/

#include <iostream>
#include <fstream>

#include "Image.h"

/**
 * @brief checks if a file is in the directory or not
 *
 * @pre n/a
 * @post no change to objects
 *
 * @param filename string of filename to check for
 * @return true if file exists in the directory
 */
bool fileIsInDirectory(const string& filename) {
  ifstream test_file(filename.c_str());
  return test_file.good();
}

void smoothImage(Image& img, const Image& kernel, int cent_row, int cent_col) {



}

/**
 * @brief Changes images pixel value from RGBG values to float
 * 
 * @param img input image to change to float values
 */
void convertImageToFloat(Image& img) {
  for (int row = 0; row < img.getRows(); ++row) {

    // For each column of the image
    for (int col = 0; col < img.getCols(); ++col) {

      // Set img[row][col] or img[y][x] pixel float value from corresponding
      // grey value of the same pixel position
      img.setFloat(row, col, (float)img.getPixel(row, col).grey);
    }
  }
}

Image createSxKernel() {
  Image Sx_kernel(1, 3);
  Sx_kernel.setFloat(0, 0, 0.25);
  Sx_kernel.setFloat(0, 1, 0.5);
  Sx_kernel.setFloat(0, 2, 0.25);
  return Sx_kernel;
}

Image createSyKernel() {
  Image Sy_kernel(3, 1);
  Sy_kernel.setFloat(0, 0, 0.25);
  Sy_kernel.setFloat(1, 0, 0.5);
  Sy_kernel.setFloat(2, 0, 0.25);
  return Sy_kernel;
}

Image createXGradientKernel() {
  Image x_grad_kernel(1, 3);
  x_grad_kernel.setInt(0, 0, -1);
  x_grad_kernel.setInt(0, 1, 0);
  x_grad_kernel.setInt(0, 2, 1);
  return x_grad_kernel;
}

Image createYGradientKernel() {
  Image y_grad_kernel(3, 1);
  y_grad_kernel.setInt(0, 0, -1);
  y_grad_kernel.setInt(1, 0, 0);
  y_grad_kernel.setInt(2, 0, 1);
  return y_grad_kernel;
}


int main(int argc, char* argv[]) {

  // Ends program is test1.gif is not found in the directory
  if (!fileIsInDirectory("test2.gif")) {
    std::cout << "File test1.gif was not found" << endl;
    return -1;
  }

  // Initialize input image in local directory within program
  Image img("test2.gif");

  // Convert image pixel RGB to float
  convertImageToFloat(img);

  // Test image for after image converted to float
  img.writeFloatImage("After_convert_to_float.gif");

  // Create hardcoded kernels for Gaussian representative smoothing
  // Uses 1/4, 1/2, 1/4 for values, respectively.
  Image Sx_kernel = createSxKernel();
  Image Sy_kernel = createSyKernel();

  // Smooth img n times with Sx (x kernel)


  // Smooth img n times with Sy (y kernel)





  // Output image back to bytes and create output file
  
  return 0;
}