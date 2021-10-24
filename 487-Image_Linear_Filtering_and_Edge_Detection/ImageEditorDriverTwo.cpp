/*********************************************************************
 * @file      ImageEditorDriverTwo.cpp
 * @brief     Program takes an input image named "test2.gif" and outputs
 *              two images called "smooth.gif" and "edges.gif" which show the
 *              greyscale image smoothed using simple 1/4, 1/2, 1/4 gaussian
 *              linear filtering and edging using a threshhold of 10.
 * 
 * @details   main method drives program through 9 nine steps which take input
 *              image, smoothes the image, prints out the smoothed image
 *              creates edge detection gradient images in x and y direction 
 *              combine the gradient images, and creates an edge image based on 
 *              the created magnitude given threshold and maximus suppression.
 *
 * @author     Joseph Lan
 * @date       24 October 2021
 *
 * CSS 487
 * Program 2
 * Prof. Clark Olson
 *********************************************************************/

#include <iostream>
#include <fstream>

#include "Image.h"

/**
 * @brief struct center holds the center information of an image
 */
struct center {

  // Constructor initializes data members to in_row and in_col
  center(const Image& img) {
    row = img.getRows() / 2;
    col = img.getCols() / 2;
  }

  // Data members
  int row; // row of center pix
  int col; // col of center pix
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
           row > img.getRows() - 1 ||
           col > img.getCols() - 1);
}

/**
 * @brief applies linear filtering using passed in kernel
 * 
 * @param img input image to get original values from
 * @param knl knl to obtain weighted values
 * @param knl_cnt center of the kernel
 * @return new image that has been filtered
 */
Image convolveImage(const Image& img, const Image& knl, center knl_cnt) {

  // Result image to return
  Image result(img.getRows(), img.getCols());

  // Traverse over every pixel in the input image and convolute with kernel
  for (int row = 0; row < img.getRows(); ++row) {
    for (int col = 0; col < img.getCols(); ++col) {

      float pix_knl_sum = 0;

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
            
            // Correct for row out of bounds
            int row_correct = 0;

            // If negative row
            if (row - row_offset < 0) {
              row_correct = 0 - (row - row_offset);

              // If row is past max number of rows
            } else if (row - row_offset > img.getRows() - 1) {
              row_correct = -1 * (img.getRows() - 1 - row - row_offset);
            }

            // Correct for col out of bounds
            int col_correct = 0;

            if (col - col_offset < 0) {
              col_correct = 0 - (col - col_offset);

              // If col is past max number of rows
            } else if (col - col_offset > img.getCols() - 1) {
              col_correct = -1 * (img.getCols() - 1 - col - col_offset);
            }
            pix_knl_sum += (float)img.getFloat(row - row_offset + row_correct,
                                        col - col_offset + col_correct) *
              knl.getFloat(knl_row, knl_col);
            result.setFloat(row, col, pix_knl_sum);

          } else {

            // If within image, img(corrected pix) * knl weight
            pix_knl_sum += (float)img.getFloat(row - row_offset, col - col_offset) *
              knl.getFloat(knl_row, knl_col);
            result.setFloat(row, col, pix_knl_sum);
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
 * @brief Changes images pixel value from RGBG values to float
 * 
 * @param img input image to change to float values
 */
void convertImageToFloat(Image& img) {
  
  // For each row of the img
  for (int row = 0; row < img.getRows(); ++row) {

    // For each column of the img
    for (int col = 0; col < img.getCols(); ++col) {

      // Set img[row][col] or img[y][x] pixel float value from corresponding
      // grey value of the same pixel position
      img.setFloat(row, col, (float)img.getPixel(row, col).grey);
    }
  }
}

/**
 * @brief Returns a copy of the float image with <byte> grey image value
 * 
 * @pre img is not null
 * @post no change to arg
 * 
 * @param img passed in image to replicate and return
 * @return img as grey scale image instead of float
 */
Image createByteImage(const Image& img) {
  
  Image result(img);

  // For each row of the img
  for (int row = 0; row < img.getRows(); ++row) {

    // For each column of the image
    for (int col = 0; col < img.getCols(); ++col) {

      // Set img[row][col] or img[x][y] pixel float value from corresponding
      // float value of the same pixel position
      result.setGrey(row,
                     col,
                     static_cast<byte>(result.getPixel(row, col).floatVal));
    }
  }
  return result;
}

/**
 * @brief Hardcoded Sx Kernel | .25 | 0.5 | 0.25 |
 * 
 * @pre n/a
 * @post no object changes
 * 
 * @return hardcoded | .25 | .5 | 0.25 | kernel
 */
Image createSxKernel() {
  Image Sx_kernel(1, 3);
  Sx_kernel.setFloat(0, 0, 0.25);
  Sx_kernel.setFloat(0, 1, 0.5);
  Sx_kernel.setFloat(0, 2, 0.25);
  return Sx_kernel;
}

/**
 * @brief Hardcoded Sx Kernel | .25 | .5 | .25 |
 *
 * @pre n/a
 * @post no object changes
 *
 * @return hardcoded | .25 | .5 | .25 |
 */
Image createSyKernel() {
  Image Sy_kernel(3, 1);
  Sy_kernel.setFloat(0, 0, 0.25);
  Sy_kernel.setFloat(1, 0, 0.5);
  Sy_kernel.setFloat(2, 0, 0.25);
  return Sy_kernel;
}

/**
 * @brief Hardcoded x Gradient Kernel -1 | 0 | 1
 *
 * @pre n/a
 * @post no object changes
 *
 * @return hardcoded -1 | 0 | 1 kernel
 */
Image createXGradientKernel() {
  Image x_grad_kernel(1, 3);
  x_grad_kernel.setFloat(0, 0, -1.0);
  x_grad_kernel.setFloat(0, 1, 0.0);
  x_grad_kernel.setFloat(0, 2, 1.0);
  return x_grad_kernel;
}

/**
 * @brief Hardcoded Y Gradient Kernel -1 | 0 | 1
 *
 * @pre n/a
 * @post no object changes
 *
 * @return hardcoded -1 | 0 | 1 kernel
 */
Image createYGradientKernel() {
  Image y_grad_kernel(3, 1);
  y_grad_kernel.setFloat(0, 0, -1.0);
  y_grad_kernel.setFloat(1, 0, 0.0);
  y_grad_kernel.setFloat(2, 0, 1.0);
  return y_grad_kernel;
}

/**
 * @brief interpolates the given column and row position in the input image
 *          based on a floating point column and row to obtain the most
 *          accurate pixel.
 *
 * @pre input img is properly initialized
 * @post no change to objects, returns new pixel
 *
 * @param img input image to obtain pixel information from
 * @param d_col double value of column from input img
 * @param d_row double value of row from input img
 * @return new pixel with interpolated values from each surrounding pixel
 */
pixel interpolate(const Image& img, float d_col, float d_row) {

  // pixel result is the return pixel holding interpolated values
  pixel result;

  // initialize variables for math equations below to get interpolated data
  int c = (int)(floor(d_col));
  int r = (int)(floor(d_row));
  float alpha = d_row - r;
  float beta = d_col - c;

  int c_plus_one = c + 1;
  int r_plus_one = r + 1;

  if (c < 0) {
    c = 0;
  }

  if (c > img.getCols() - 1) {
    c = img.getCols() - 1;
  }

  if (c_plus_one < 0) {
    c_plus_one = 0;
  }

  if (c_plus_one > img.getCols() - 1) {
    c_plus_one = img.getCols() - 1;
  }

  if (r < 0) {
    r = 0;
  }

  if (r > img.getRows() - 1) {
    r = img.getRows() - 1;
  }

  if (r_plus_one < 0) {
    r_plus_one = 0;
  }

  if (r_plus_one > img.getRows() - 1) {
    r_plus_one = img.getRows() - 1;
  }

  // interpolate float value
  result.floatVal =
    ((1 - alpha) * (1 - beta) * (img.getFloat(r, c))) +
    (alpha * (1 - beta) * (img.getFloat(r, c_plus_one))) +
    ((1 - alpha) * beta * (img.getFloat(r_plus_one, c))) +
    (alpha * beta * (img.getFloat(r_plus_one, c)));

  return result; // return interpolated pixel
}


/**
 * @brief main method drives program through 9 nine steps which take an input
 *          image, smoothes the image, prints out the smoothed image, creates
 *          edge detection gradient images in x and y direction, combine the
 *          gradient images, and creates an edge image based on the created
 *          magnitude given threshold and maximus suppression.
 * 
 * @detail 
 *    STEP 1: INPUT IN
 *    STEP 2: Initialize test2.gif
 *    STEP 3: Create floating point image
 *    STEP 4: Smooth image argv[1] times
 *    STEP 5: Intermediary printing of smooth.gif
 *    STEP 6: Create gx and gy gradient images
 *    STEP 7: Combine gx and gy gradient images into gmag image
 *    STEP 8: Edge gmag image based on interpolated threshold
 *    STEP 9: Output image
 * 
 * @pre Program must run with input argv[1] which gives n number of smoothing
 *        "test2.gif" must be present in the directory
 * @post Prints out 2 images, smooth.gif and edges.gif
 * 
 * @param argc input counter
 * @param argv input array, [1] containing program name [2] containing n number
 *          of desired smoothing iterations
 * @return 0 if normal exit
 */
int main(int argc, char* argv[]) {

  // STEP 1 INPUT IN------------------------------------------------------------
  // Ends program is test1.gif is not found in the directory
  if (!fileIsInDirectory("test2.gif")) {
    std::cout << "File test2.gif was not found" << endl;
    return -1;
  }

  // If not enough argument provided
  if (argc < 2) {
    std::cout << "Did not provide number of iterations as an argument." << endl;
    return -1;
  }

  // STEP 2 test2.gif-----------------------------------------------------------
  // Initialize input image in local directory within program
  Image img("test2.gif");

  // STEP 3 FLOATING POINT IMG--------------------------------------------------
  // Convert image pixel RGB to float
  convertImageToFloat(img);

  // STARTING IMAGE AS FLOAT
  // Test image for after image converted to float
  img.writeFloatImage("before_smooth.gif");

  // STEP 4A CREATE HARD CODED GAUUSSIAN FOR SMOOTHING
  // Create hardcoded kernels for Gaussian representative smoothing
  // Uses 1/4, 1/2, 1/4 for values, respectively.
  Image Sx_kernel = createSxKernel();
  Image Sy_kernel = createSyKernel();

  // STEP 4B CREATE CENTER HOLDERS FOR KERNELS
  // Create struct variables holding center of the kernels
  // This is applicable for both x knls and both y knls (smooth & grad)
  center x_knl_cnt(Sx_kernel);
  center y_knl_cnt(Sy_kernel);

  // STEP 4C SMOOTH i times in X and Y directions
  // Smooth for iteration_num number of iterations to the output image
  int iteration_num = stoi(argv[1]);
  for (int i = 0; i < iteration_num; ++i) {
    img = convolveImage(img, Sx_kernel, x_knl_cnt);
  }

  // STEP 4C Continued
  for (int i = 0; i < iteration_num; ++i) {
    img = convolveImage(img, Sy_kernel, y_knl_cnt);
  }

  // STEP 5 Intermediary print, Convert to bytes and print smooth.gif-----------
  Image after_smoothing = createByteImage(img);
  after_smoothing.writeGreyImage("smooth.gif");

  // STEP 6 Calculate gradient in X and gradient in Y---------------------------

  // Create gradient_x and gradient_y kernels
  Image gradient_x = createXGradientKernel();
  Image gradient_y = createYGradientKernel();

  // Create gradient center struct
  center gx_knl_cnt(gradient_x);
  center gy_knl_cnt(gradient_y);

  // Convolve images to create gx and gy with gradient x and gradient y
  Image gx = convolveImage(img, gradient_x, gx_knl_cnt);
  Image gy = convolveImage(img, gradient_y, gy_knl_cnt);
  gx.writeFloatImage("gx.gif");
  gy.writeFloatImage("gy.gif");

  // Print after_gx image and after_gy image
  Image after_gx = createByteImage(gx);
  Image after_gy = createByteImage(gy);

  after_gx.writeGreyImage("after_gx.gif");
  after_gy.writeGreyImage("after_gy.gif");

  // STEP 7 Gradient Magnitude--------------------------------------------------

  // Image gmag combines information from gx and gy with threshold 10
  Image gmag(img.getRows(), img.getCols());

  // Set gmag to the calculation of gx and gy
  // sqrt( (gx)^2 + (gy)^2 )

  // for every row of gmag
  for (int row = 0; row < gmag.getRows(); ++row) {

    // for every column of gmag
    for (int col = 0; col < gmag.getCols(); ++col) {

      // declare variable for easy of reading
      float gx_squared = gx.getFloat(row, col) * gx.getFloat(row, col);
      float gy_squared = gy.getFloat(row, col) * gy.getFloat(row, col);

      // calculate sqrt of gx^2 + gy^2
      float sqrt_gx_gy = sqrt(gx_squared + gy_squared);

      // Set pixel
      gmag.setFloat(row, col, sqrt_gx_gy);
    }
  }
  gmag.writeFloatImage("gmag.gif");

  // Print after_gmag image
  Image after_gmag = createByteImage(gmag);
  after_gmag.writeGreyImage("after_gmag.gif");

  // Step 8 Edge Image----------------------------------------------------------

  // result_edge is the resultant edged image
  Image result_edge(gmag.getRows(), gmag.getCols());

  // For every point in result_edge
  // Check in gmag with maximum suppression with conditions:
  //  -Gmag >= 10
  //  -Gmag > Gr
  //  -Gmag > Gp
  // Given g is one pixel in the gradient direction given by:
  //  g = (Gx / Gmag, Gy/Gmag), and
  //  r is one pixel in the q direction and
  //  p is one direction in the opposite direction of q, where
  //    r = q + g   p = q - g
  // for every row of gmag
  for (int row = 0; row < gmag.getRows(); ++row) {

    // for every column of gmag
    for (int col = 0; col < gmag.getCols(); ++col) {

      // If magnitude of pixel in gmag at (row, col) is at least 10
      if (gmag.getFloat(row, col) >= 10) {

        // Calculate g, one pixel in the gradient direction
        float gx_over_gmag = gx.getFloat(row, col) / gmag.getFloat(row, col);
        float gy_over_gmag = gy.getFloat(row, col) / gmag.getFloat(row, col);

        float r_col = col + gx_over_gmag;
        if (r_col > gmag.getCols() - 1) {
          r_col = (float)(gmag.getCols() - 1);
        } else if (r_col < 0) {
          r_col = 0;
        }

        float r_row = row + gy_over_gmag;
        if (r_row > gmag.getRows() - 1) {
          r_row = (float)(gmag.getRows() - 1);
        } else if (r_row < 0) {
          r_row = 0;
        }

        float p_col = col - gx_over_gmag;
        if (p_col < 0) {
          p_col = 0;
        } else if (p_col > gmag.getCols() - 1) {
          p_col = (float)(gmag.getCols() - 1);
        }

        float p_row = row - gy_over_gmag;
        if (p_row < 0) {
          p_row = 0;
        } else if (p_row > gmag.getRows() - 1) {
          p_row = (float)(gmag.getRows() - 1);
        }

        float r_val = interpolate(gmag, r_col, r_row).floatVal;
        float p_val = interpolate(gmag, p_col, p_row).floatVal;
        
        if (gmag.getFloat(row, col) > r_val &&
            gmag.getFloat(row, col) > p_val) {
          result_edge.setFloat(row, col, 255);
        } else {
          result_edge.setFloat(row, col, 0);
        }

      } else {
        result_edge.setFloat(row, col, 0);
      }
    }
  }

  // Step 9 Print out Edge Image------------------------------------------------
  result_edge = createByteImage(result_edge);
  result_edge.writeGreyImage("edges.gif");
  
  return 0; // Normal exit code
}