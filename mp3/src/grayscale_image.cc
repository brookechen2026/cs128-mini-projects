#include "grayscale_image.hpp"
#include <fstream>
#include <iostream>
#include <cmath>


GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset) : width_(dataset.Width()), height_(dataset.Height()){
    for (size_t i = 0; i < height_; i++) {
        std::vector<Color> temp;
        for (size_t j = 0; j < width_; j++) {
            int shade = static_cast<int>(std::round(
                static_cast<double>(dataset.DatumAt(i, j) - dataset.MinEle()) / (dataset.MaxEle() - dataset.MinEle()) *
                kMaxColorValue));

            temp.push_back(Color(shade, shade, shade));
        }
        image_.push_back(temp);
    }
    
}



GrayscaleImage::GrayscaleImage(const std::string& filename, size_t width, size_t height) : width_(width), height_(height){
    ElevationDataset dataset(filename, width, height);
    for (size_t i = 0; i < height_; i++) {
        std::vector<Color> temp;
        for (size_t j = 0; j < width_; j++) {
            int shade = static_cast<int>(std::round(
                static_cast<double>(dataset.DatumAt(i, j) - dataset.MinEle()) / (dataset.MaxEle() - dataset.MinEle()) *
                kMaxColorValue));

            temp.push_back(Color(shade, shade, shade));
        }
        image_.push_back(temp);
    }
}
    
    
    


size_t GrayscaleImage::Width() const{
    return width_;
}

size_t GrayscaleImage::Height() const{
    return height_;
}

unsigned int GrayscaleImage::MaxColorValue() const{
    return kMaxColorValue;
}

const Color& GrayscaleImage::ColorAt(int row, int col) const{
    return image_[row][col];
}

const std::vector<std::vector<Color> >& GrayscaleImage::GetImage() const{
    return image_;
}

void GrayscaleImage::ToPpm(const std::string& name) const {
    std::ofstream ofs(name);
    ofs << "P3\n" << width_ << " " << height_ << "\n" << kMaxColorValue << "\n";
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            ofs << image_[i][j].Red() << " " << image_[i][j].Green() << " " << image_[i][j].Blue() << " ";

        }
        ofs << std::endl;
    }
    ofs.close();
}