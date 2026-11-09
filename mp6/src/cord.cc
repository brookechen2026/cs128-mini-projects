// cord.cc

#include "cord.hpp"

#include <stdexcept>

#include "cord_utilities.hpp"

// Cord::Cord(std::string_view sv) {
//     // Check if string view is empty
//     if (sv.empty()) {
//         throw std::runtime_error("String view cannot be empty");
//     }

//     // Allocate memory for data_ and copy string contents
//     data_ = new char[sv.size() + 1];

//     length_ = sv.size();
//     for (unsigned int i = 0; i < length_; i++) {
//         data_[i] = sv.at(i);
//         data_[i] = sv.at(i);
//     }
//     data_[length_] = '\0';
// }

// Cord::~Cord() { delete[] data_; }

// SharedPointer<char> Cord::ToString() const {
//   SharedPointer<char> point;
//   char* shared = new char[length_ + 1];
//   int count = 0;
//   CordHelper(shared, count);
//   shared[length_] = '\0';
//   point = SharedPointer(shared, kSharedPointerType::kArray);
//   return point;
// }

// // SharedPointer<char> Cord::ToString() const {
// //   SharedPointer<char> sp;
// //   char* s = new char[length_ + 1];
// //   int count = 0;
// //   Helper(s, count);
// //   s[length_] = '\0';
// //   sp = SharedPointer(s, kSharedPointerType::kArray);
// //   return sp;
// // }


// void Cord::CordHelper(char* character, int& count) const {
//   if (data_ != nullptr) {
//     for (unsigned int i = 0; i < length_; i++) {
//       character[count] = data_[i];
//       count++;
//     }
//   } else if (left_.Get() != nullptr) {
//     left_.Get()->CordHelper(character, count);
//   } else if (right_.Get() != nullptr) {
//     right_.Get()->CordHelper(character, count);
//   }
// }



Cord::Cord(std::string_view sv) {
    if (sv.empty()) {
        throw std::runtime_error("String view cannot be empty");
    }
    data_ = new char[sv.size() + 1];
    length_ = sv.size();
    for (unsigned int i = 0; i < length_; i++) {
        data_[i] = sv[i];
    }
    data_[length_] = '\0';
}

Cord::~Cord() { delete[] data_; }

SharedPointer<char> Cord::ToString() const {
    SharedPointer<char> sp;
    char* s = new char[length_ + 1];
    int count = 0;
    CordHelper(s, count);
    s[length_] = '\0';
    sp = SharedPointer(s, kSharedPointerType::kArray);
    return sp;
}

void Cord::CordHelper(char* ch, int& count) const {
    if (data_ != nullptr) {
        for (unsigned int i = 0; i < length_; i++) {
            ch[count] = data_[i];
        count++;
        }
    } else {
        if (left_.Get() != nullptr) {
            left_.Get()->CordHelper(ch, count);
        }
        if (right_.Get() != nullptr) {
            right_.Get()->CordHelper(ch, count);
        }
    }
}