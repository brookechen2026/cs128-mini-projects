#include "elevation_dataset.hpp"
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

ElevationDataset::ElevationDataset(const std::string &filename, size_t width, size_t height)
    : width_(width), height_(height) {

    max_ele_=std::numeric_limits<int>::min();
    min_ele_=std::numeric_limits<int>::max();
    std::ifstream input(filename, std::ios::in);
    if (!input.is_open()) {
        throw std::runtime_error("invalid file name");
    }
    int value = 0;
    for (size_t i = 0; i < height; i++) {
        std::vector<int> buffer;
        for (size_t j = 0; j < width; j++) {
            if (input >> value) {
                buffer.push_back(value);
                max_ele_ = std::max(max_ele_, value);
                min_ele_ = std::min(min_ele_, value);
            } else
                throw std::invalid_argument("invalid argument");
        }
        data_.push_back(buffer);
    }
    if (input >> value) {
        throw std::invalid_argument("invalid argument");
    }

    input.close();
}

size_t ElevationDataset::Width() const {
    return width_;
}
size_t ElevationDataset::Height() const {
    return height_;
}
int ElevationDataset::MaxEle() const{
    return max_ele_;
}
int ElevationDataset::MinEle() const {
    return min_ele_;
}
int ElevationDataset::DatumAt(size_t row, size_t col) const {
    return data_[row][col];
}
const std::vector<std::vector<int> >& ElevationDataset::GetData() const {
    return data_;
}


