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

struct center {
  center(int in_row, int in_col) {
    row = in_row;
    col = in_col;
  }

  int row;
  int col;
};

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

Image smoothImage(const Image& img, const Image& knl, center knl_cnt) {

  // Result image to return
  Image result(img.getRows(), img.getCols());

  // Traverse over every pixel in the input image and convolute with kernel
  for (int row = 0; row < img.getRows(); ++row) {
    for (int col = 0; col < img.getCols(); ++col) {

      int pix_knl_sum = 0;

      // Row offset to correct for center
      int row_offset = knl_cnt.row;

      // For each pixel in kernel, apply the weights and find the sum
      for (int knl_row = 0; knl_row < knl.getRows(); ++knl_row) {

        // Column offset to correct for center
        int col_offset = knl_cnt.col;

        for (int knl_col = 0; knl_col < knl.getCols(); ++knl_col) {

          // pix_knl_sum = img(corrected pix value) * knl weight
          // Check to see if pixel will be outside of img
          if (!pixelInImage(img, row - row_offset, col - col_offset)) {


          } else {

            // If within image, img(corrected pix) * knl weight
            pix_knl_sum += img.getFloat(row - row_offset, col - col_offset) *
              knl.getFloat(knl_row, knl_col);
          }

          // Decrement offset to subtract one less
          --col_offset;
        }
        // Decrement offset to subtract one less
        --row_offset;
      }
    }
  }

  return result;
}

/**
 * @brief Returns whether the given row and column are within the img dimensions
 * 
 * @pre n/a
 * @post No change to objects
 * 
 * @param img image to test against
 * @param row row of px to test for
 * @param col col of px to test for
 * @return true if (row,col) is within img dimensions
 */
bool pixelInImage(const Image& img, int row, int col) {
  return !(row < 0 ||
          col < 0 ||
          row >= img.getRows() ||
          col >= img.getCols());
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

  // Create struct variables holding center of the kernels
  // This is applicable for both x knls and both y knls (smooth & grad)
  center x_knl_cnt(0, 1);
  center y_knl_cnt(1, 0);

  // Smooth img n times with Sx (x kernel)
  int smooth_iterations;
  for (int i = 0; 0 < smooth_iterations; ++i) {
    img = smoothImage(img, Sx_kernel, x_knl_cnt);
  }

  // Smooth img n times with Sy (y kernel)
  for (int i = 0; 0 < smooth_iterations; ++i) {
    img = smoothImage(img, Sy_kernel, y_knl_cnt);
  }

  // Output image back to bytes and create output file
  
  return 0;
}