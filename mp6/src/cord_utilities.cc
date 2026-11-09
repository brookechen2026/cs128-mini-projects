#include "cord_utilities.hpp"
#include <stdexcept>

SharedPointer<Cord> ConcatCords(SharedPointer<Cord> left_cord,
                                 SharedPointer<Cord> right_cord) {
    // Check if either of the cords is null
    if (left_cord.Get() == nullptr || right_cord.Get() == nullptr) {
        throw std::runtime_error("Invalid cord pointers for concatenation");
    }
    SharedPointer<Cord> c(new Cord);

    // Set length of the combined cord
    c.Get()->length_ = left_cord->Length() + right_cord->Length();
    c.Get()->left_ = left_cord;
    c.Get()->right_ = right_cord;
    return c;
}

// SharedPointer<Cord> ConcatCords(SharedPointer<Cord> left_cord,
//                                 SharedPointer<Cord> right_cord) {
//   if (left_cord.Get() == nullptr || right_cord.Get() == nullptr) {
//     throw std::runtime_error("null cord");
//   }
//   // Create a new Cord object to hold the concatenated data
//   SharedPointer<Cord> combc(new Cord);

//   // Set length of the combined cord
//   combc.Get()->length_ = left_cord->length_ + right_cord->length_;
//   combc.Get()->left_ = left_cord;
//   combc.Get()->right_ = right_cord;
//   return combc;
// }


SharedPointer<Cord> SubString(SharedPointer<Cord> curr_cord,
                               unsigned int lower_idx,
                               unsigned int upper_idx) {
  if (lower_idx >= upper_idx || upper_idx > curr_cord->Length()) {
    throw std::runtime_error("idx invalid");
  } else if (lower_idx == 0 && upper_idx == curr_cord->Length()) {
    return curr_cord;
  } else if (curr_cord->Left().Get() == nullptr) {
    char* ch = new char[upper_idx - lower_idx + 1];
    for (unsigned int i = 0; i < upper_idx - lower_idx; i++) {
      ch[i] = curr_cord->Data()[i + lower_idx];
    }
    ch[upper_idx - lower_idx] = '\0';
    std::string_view strv(ch);
    SharedPointer<Cord> co(new Cord(strv));
    return co;
  }
  

  unsigned int lefl = curr_cord->Left()->Length();
  if (upper_idx <= lefl) {
    return SubString(curr_cord->Left(), lower_idx, upper_idx);
  } else if (lower_idx >= lefl) {
    return SubString(curr_cord->Right(), lower_idx - lefl, upper_idx - lefl);
  } else {
    auto ls = SubString(curr_cord->Left(), lower_idx, lefl);
    auto rs = SubString(curr_cord->Right(), 0, upper_idx - lefl);
    return ConcatCords(ls, rs);
  }
}

