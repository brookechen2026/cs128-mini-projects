#include "seam_carver.hpp"
#include <iostream>


// implement the rest of SeamCarver's functions here
const ImagePPM& SeamCarver::GetImage() const {
  return image_;
}
int SeamCarver::GetHeight() const {
  return height_;
}
int SeamCarver::GetWidth() const {
  return width_;
}

int SeamCarver::GetEnergy(int row, int col) const {
    // Calculate the horizontal and vertical gradients for the current pixel
    int red_x = GetXGradient(row, col, [](const Pixel& p) { return p.GetRed(); });
    int green_x = GetXGradient(row, col, [](const Pixel& p) { return p.GetGreen(); });
    int blue_x = GetXGradient(row, col, [](const Pixel& p) { return p.GetBlue(); });

    int red_y = GetYGradient(row, col, [](const Pixel& p) { return p.GetRed(); });
    int green_y = GetYGradient(row, col, [](const Pixel& p) { return p.GetGreen(); });
    int blue_y = GetYGradient(row, col, [](const Pixel& p) { return p.GetBlue(); });

    // Calculate the energy using the gradients
    int energy_x = red_x * red_x + green_x * green_x + blue_x * blue_x;
    int energy_y = red_y * red_y + green_y * green_y + blue_y * blue_y;

    return energy_x + energy_y;
}

int SeamCarver::GetXGradient(int row, int col, int (*get_channel)(const Pixel&)) const {
    int left = get_channel(image_.GetPixel(row, (col + width_ - 1) % width_));
    int right = get_channel(image_.GetPixel(row, (col + 1) % width_));
    return right - left;
}

int SeamCarver::GetYGradient(int row, int col, int (*get_channel)(const Pixel&)) const {
    int top = get_channel(image_.GetPixel((row + height_ - 1) % height_, col));
    int bottom = get_channel(image_.GetPixel((row + 1) % height_, col));
    return bottom - top;
}


int Min (int one, int two, int three) {
  if (one < two && one < three) {
    return one;
  }
  if (two < three) {
    return two;
  }
  return three;
}

int Min (int one, int two) {
  if (one < two) {
    return one;
  }
  return two;
}

int* SeamCarver::HorizHelper(int start, int** values) const {
  int* ret = new int[width_];
  ret[0] = start;
  int row = start;
  for (int i = 1; i < width_; i++) {
    int straight = values[row][i];
    if (row == 0) {
      int plus = values[row + 1][i];

      if (straight <= plus) {
        ret[i] = row;
      } else {
        ret[i] = row + 1;
        row++;
      }
    } else if (row == height_ - 1) {
      int minus = values[row - 1][i];
      if (straight <= minus) {
        ret[i] = row;
      } else {
        ret[i] = row - 1;
        row--;
      }
    } else {
      int plus = values[row + 1][i];
      int minus = values[row - 1][i];
      if (straight <= plus && straight <= minus) {
        ret[i] = row;
      } else if (minus <= plus) {
        ret[i] = row - 1;
        row--;
      }else {
        ret[i] = row + 1;
        row++;
      }
    }
  }
  return ret;
}

int* SeamCarver::GetHorizontalSeam() const { 
    int** dp = new int*[height_];
    for (int i = 0; i < height_; ++i) {
        dp[i] = new int[width_];
    }
    for (int row = 0; row < height_; ++row) {
        dp[row][width_ - 1] = GetEnergy(row, width_ - 1);
    }
    for (int col = width_ - 2; col >= 0; --col) {
        for (int row = 0; row < height_; ++row) {
            int best = 0;
            if (row == 0) {
                best = Min(dp[row][col + 1], dp[row + 1][col + 1]);
            } else if (row == height_ - 1) {
                best = Min(dp[row][col + 1], dp[row - 1][col + 1]);
            } else {
                best = Min(dp[row][col + 1], dp[row + 1][col + 1], dp[row - 1][col + 1]);
            }
            dp[row][col] = best + GetEnergy(row, col);
        }
    }
    int min_energy = kMax;
    int min_row = 0;
    for (int row = 0; row < height_; ++row) {
        if (dp[row][0] < min_energy) {
            min_energy = dp[row][0];
            min_row = row;
        }
    }
    int* seam = HorizHelper(min_row, dp);
    for (int i = 0; i < height_; ++i) {
        delete[] dp[i];
    }
    delete[] dp;
    return seam;
}


int* SeamCarver::VertHelper(int start, int** values) const {
  int* ret = new int[height_];
  ret[0] = start;
  int col = start;
  for (int i = 1; i < height_; i++) {
    int straight = values[i][col];
    if (col == 0) {
      int plus = values[i][col + 1];
      if (straight <= plus) {
        ret[i] = col;
      } else {
        ret[i] = col + 1;
        col++;
      }
    } else if (col == width_ - 1) {
      int minus = values[i][col - 1];
      if (straight <= minus) {
        ret[i] = col;
      } else {
        ret[i] = col - 1;
        col--;
      }
    } else {
      int plus = values[i][col + 1];
      int minus = values[i][col - 1];
      if (straight <= plus && straight <= minus) {
        ret[i] = col;
      } else if (minus <= plus && minus <= straight) {
        ret[i] = col - 1;
        col--;
      }else {
        ret[i] = col + 1;
        col++;
      }
    }
  }
  return ret;
}

int* SeamCarver::GetVerticalSeam() const {
    int** dp = new int*[height_];
    for (int i = 0; i < height_; ++i) {
        dp[i] = new int[width_];
    }
    for (int col = 0; col < width_; ++col) {
        dp[height_ - 1][col] = GetEnergy(height_ - 1, col);
    }
    for (int row = height_ - 2; row >= 0; --row) {
        for (int col = 0; col < width_; ++col) {
            int best = 0;
            if (col == 0) {
                best = Min(dp[row + 1][col + 1], dp[row + 1][col]);
            } else if (col == width_ - 1) {
                best = Min(dp[row + 1][col], dp[row + 1][col - 1]);
            } else {
                best = Min(dp[row + 1][col + 1], dp[row + 1][col], dp[row + 1][col - 1]);
            }
            dp[row][col] = best + GetEnergy(row, col);
        }
    }
    int min_energy = kMax;
    int min_col = 0;
    for (int col = 0; col < width_; ++col) {
        if (dp[0][col] < min_energy) {
            min_energy = dp[0][col];
            min_col = col;
        }
    }
    int* seam = VertHelper(min_col, dp);
    for (int i = 0; i < height_; ++i) {
        delete[] dp[i];
    }
    delete[] dp;
    return seam;
}


void SeamCarver::RemoveHorizontalSeam() {
  int* seam = GetHorizontalSeam();
  auto** hold = new Pixel*[height_ - 1];
  for (int i = 0; i < height_ - 1; i++) {
    hold[i] = new Pixel[width_];
  }
  int c = -1;
  for (int col = 0; col < width_; col++) {
    c++;
    int r = -1;
    for (int row = 0; row < height_; row++) {
      r++;
        if (seam[col] == row) {
          r--;
        } else {
          hold[r][c] = image_.GetPixel(row,col);
        }
    }
  }
  image_.Overwrite(hold, false);
  delete[] seam;
  height_--;
}


void SeamCarver::RemoveVerticalSeam() { 
  int* seam = GetVerticalSeam();
  auto** hold = new Pixel*[height_];
  for (int i = 0; i < height_; i++) {
    hold[i] = new Pixel[width_ - 1];
  }
  int r = -1;
  for (int row = 0; row < height_; row++) {
    r++;
    int c = -1;
    for (int col = 0; col < width_; col++) {
      c++;
        if (seam[row] == col) {
          c--;
        } else {
          hold[r][c] = image_.GetPixel(row,col);
        }
    }
  }
  image_.Overwrite(hold, true);
  delete[] seam;
  width_--;
}



// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}
