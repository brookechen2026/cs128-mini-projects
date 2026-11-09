#include "color.hpp"
#include <stdexcept>

Color::Color(int red, int green, int blue): red_(red), green_(green), blue_(blue) {

  // need to ensure valid color has been constructed...
  if (red > kColorValueMax || red < kColorValueMin) {
    throw std::runtime_error("invalid argument");
  }
  if (green > kColorValueMax || green < kColorValueMin) {
    throw std::runtime_error("invalid argument");
  }
  if (blue > kColorValueMax || blue < kColorValueMin) {
    throw std::runtime_error("invalid argument");
  }

}

// do not modify
bool operator==(const Color& rhs, const Color& lhs) {
  return (rhs.Red() == lhs.Red() && rhs.Green() == lhs.Green() &&
          rhs.Blue() == lhs.Blue());
}