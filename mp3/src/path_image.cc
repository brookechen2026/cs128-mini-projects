#include <fstream>
#include <limits>
#include <iostream>
#include "path_image.hpp"



PathImage::PathImage(const GrayscaleImage& image, const ElevationDataset& dataset) 
    : width_(image.Width()), height_(image.Height()), path_image_(image.GetImage()) {

    const Color kRed = Color(252, 25, 63);
    const Color kGreen = Color(31, 253, 13);

    BuildPaths(image, dataset);

    size_t min_ele = paths_.at(0).EleChange();
    size_t lowest_row = 0;
    for (size_t i = 0; i < paths_.size(); i++) {
        if (paths_[i].EleChange() < min_ele) {
            min_ele = paths_[i].EleChange();
            lowest_row = i;
        }
    }

    MarkPathsWithColors(kRed, kGreen, lowest_row);
}

void PathImage::BuildPaths(const GrayscaleImage& /* image */, const ElevationDataset& dataset) {
    for (size_t i = 0; i < height_; i++) {
        Path p = Path(width_, i);
        p.SetLoc(0, i);
        size_t loc = i;
        for (size_t j = 0; j < width_ - 1; j++) {
            int current = dataset.DatumAt(loc, j);
            Update(dataset, current, loc, j, p);
        }
        paths_.push_back(p);
    }
}

void PathImage::MarkPathsWithColors(const Color& kRed, const Color& kGreen, size_t lowest_row) {
    for (size_t i = 0; i < height_; i++) {
        for (size_t j = 0; j < paths_.at(i).Length(); j++) {
            path_image_.at(paths_[i].GetPath()[j])[j] = kRed;
        }
    }

    for (size_t i = 0; i < paths_.at(lowest_row).Length(); i++) {
        path_image_.at(paths_[lowest_row].GetPath()[i])[i] = kGreen;
    }
}

size_t PathImage::Width() const {
    return width_;
}

size_t PathImage::Height() const {
    return height_;
}

unsigned int PathImage::MaxColorValue() const {
    return kMaxColorValue;
}

const std::vector<Path> &PathImage::Paths() const {
    return paths_;
}

const std::vector<std::vector<Color>> &PathImage::GetPathImage() const {
    return path_image_;
}

void PathImage::ToPpm(const std::string &name) const {
    std::ofstream ifs(name);
    ifs << "P3\n"
        << width_ << " " << height_ << "\n"
        << kMaxColorValue << "\n";
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            ifs << path_image_[i][j].Red() << " " 
                << path_image_[i][j].Green() << " " 
                << path_image_[i][j].Blue() << " ";
        }
        ifs << std::endl;
    }
    ifs.close();
}


void PathImage::Update(const ElevationDataset& dataset,
                               const int& current,
                               size_t& row,
                               const size_t& col,
                               Path& p) const {

    if (row == 0) {
        if (abs(dataset.DatumAt(row + 1, col + 1) - current) <
            abs(dataset.DatumAt(row, col + 1) - current)) {
        p.IncEleChange(abs(dataset.DatumAt(row + 1, col + 1) - current));
            row++;
        } else {
            p.IncEleChange(abs(dataset.DatumAt(row, col + 1) - current));
        }
    } else if (row == height_ - 1) {
        if (abs(dataset.DatumAt(row - 1, col + 1) - current) <
            abs(dataset.DatumAt(row, col + 1) - current)) {
            p.IncEleChange(abs(dataset.DatumAt(row - 1, col + 1) - current));
            row--;
        } else {
            p.IncEleChange(abs(dataset.DatumAt(row, col + 1) - current));
        }
    } else {
        if (abs(dataset.DatumAt(row + 1, col + 1) - current) <=
                abs(dataset.DatumAt(row - 1, col + 1) - current) &&
            abs(dataset.DatumAt(row + 1, col + 1) - current) <
                abs(dataset.DatumAt(row, col + 1) - current)) {
            p.IncEleChange(abs(dataset.DatumAt(row + 1, col + 1) - current));
            row++;
        } else if (abs(dataset.DatumAt(row - 1, col + 1) - current) <
                    abs(dataset.DatumAt(row, col + 1) - current) &&
                abs(dataset.DatumAt(row - 1, col + 1) - current) <
                    abs(dataset.DatumAt(row + 1, col + 1) - current)) {
            p.IncEleChange(abs(dataset.DatumAt(row - 1, col + 1) - current));
            row--;
        } else {
            p.IncEleChange(abs(dataset.DatumAt(row, col + 1) - current));
        }
    }
    p.SetLoc(col + 1, row);
}  


