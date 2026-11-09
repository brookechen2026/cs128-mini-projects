#ifndef PATH_IMAGE_H
#define PATH_IMAGE_H

#include <cstdlib>  // for size_t
#include <vector>
#include <string>

#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"
#include "color.hpp"
#include "path.hpp"

class PathImage {
public:
  // write behavior declarations here; define in path_image.cc.
  PathImage(const GrayscaleImage &image, const ElevationDataset &dataset);
  size_t Width() const;
  size_t Height() const;
  unsigned int MaxColorValue() const;
  const std::vector<Path>& Paths() const;
  const std::vector<std::vector<Color> >& GetPathImage() const;
  void ToPpm(const std::string& name) const;
  void Update(const ElevationDataset& dataset, const int& current, size_t& row, const size_t& col, Path& p) const;
  
private:
  void MarkPathsWithColors(const Color& kRed, const Color& kGreen, size_t lowest_row);
  void BuildPaths(const GrayscaleImage& image, const ElevationDataset& dataset);
  std::vector<Path> paths_;
  size_t width_ = 0;
  size_t height_ = 0;
  std::vector<std::vector<Color>> path_image_;
  static const int kMaxColorValue = 255;
  
};

#endif