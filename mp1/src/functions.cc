#include "functions.hpp"

#include <iostream>

/**
 * TODO: Implement this function
 * @param word word to change to lowercase
 * @return result of changing every character in word to lowercase
 */
std::string StringToLower(const std::string& word) {
  std::cout << "Input to StringToLower() : " << word << std::endl;
  std::string result;
  for (char c : word) {
    result += static_cast<char>(std::tolower(c));
  }
  return result;
}

/**
 * TODO: Implement this function
 * @param v1 first character in potential vowel group
 * @param v2 second character in potential vowel group
 * @return true if v1 and v2 form a valid vowel group
 */
bool IsVowelGroup(const char& v1, const char& v2) {
  // std::cout << "Input to IsVowelGroup() : \"" << v1 << "\" \"" << v2 << "\""
  //           << std::endl;
  // std::string vowel_groups = "aeai aou eieu iuou oiau uie";
  // std::string group = std::string(1, v1) + v2;
  // return vowel_groups.find(group) != std::string::npos;
  if (!IsVowel(v1)) {
    return false;
  }

  if (v1 == 'a') {
    if (v2 != 'i' && v2 != 'e' && v2 != 'o' && v2 != 'u') {
      return false;
    }
  } else if (v1 == 'e' || v1 == 'o') {
    if (v2 != 'i' && v2 != 'u') {
      return false;
    }
  } else if (v1 == 'i') {
    if (v2 != 'u') {
      return false;
    }
  } else if (v1 == 'u') {
    if (v2 != 'i') {
      return false;
    }
  }
  std::cout << "Input to IsVowelGroup() : \"" << v1 << "\" \"" << v2 << "\""
            << std::endl;
  return true;
}

/**
 * TODO: Complete this function
 * @param v potential vowel character
 * @return true if v is a vowel
 */
bool IsVowel(const char& v) {
  std::cout << "Input to IsVowel() : \"" << v << "\"" << std::endl;
  std::string vowels = "aeiou";
  return vowels.find(v) != std::string::npos;
}

/**
 * TODO: Complete this function
 * @param c potential consonant character
 * @return true if c is a consonant
 */
bool IsConsonant(const char& c) {
  std::cout << "Input to IsConsonant() : \"" << c << "\"" << std::endl;
  std::string consonants = "hklmnpw";
  return consonants.find(c) != std::string::npos;
}

/**
 * TODO: Implement this function
 * @param c character to check validity of
 * @return true if input c is a valid character in the Hawaiian language, false
 * otherwise
 */
bool IsValidCharacter(const char& c) {
  std::cout << "Input to IsValidCharacter() : \"" << c << "\"" << std::endl;
  const std::string kValidChars = "aeioupklmnhw '"; 
  return kValidChars.find(c) != std::string::npos;

}

/**
 * TODO: Implement this function
 * @param word word to check validity of
 * @return true if every character in word is a valid character in the Hawaiian
 * language, false otherwise
 */
bool IsValidWord(const std::string& word) {
  std::cout << "Input to IsValidWord() : " << word << std::endl;
  for (char c : word) {
    if (!IsValidCharacter(c)) {
      return false; 
    }
  }
  return true;
}

/**
 * TODO: Implement this function
 * @param c consonant to get pronunciation of
 * @param prev character before c, used for determining w translation
 *    set to null character as default if no value is passed in
 * @return pronunciation of the consonant c as a char
 */
char ConsonantPronunciation(const char& c, const char& prev) {
  std::cout << "Input to ConsonantPronunciation() : \"" << c << "\" \"" << prev
            << "\"" << std::endl;
  if (c == 'w') {
    // if (prev == 'a' || prev == ' ' || prev == '\0') {
    //   return 'w';
    // } else if (prev == 'i' || prev == 'e') {
    //   return 'v';
    // } else {
    //   return 'w';
    // }


    if (prev == 'i' || prev == 'e') {
      return 'v';
    } else {
      return 'w';
    }
  }
  return c;
}

/**
 * TODO: Implement this function
 * @param v1 first vowel in a vowel grouping
 * @param v2 second vowel in a vowel grouping
 * @return the pronunciation of the vowel grouping made up of v1 and v2
 * as a string
 */
std::string VowelGroupPronunciation(const char& v1, const char& v2) {
  std::cout << "Input to VowelGroupPronunciation() : \"" << v1 << "\" \"" << v2
            << "\"" << std::endl;

  if (v1 == 'a' && v2 == 'i') return "eye";
  if (v1 == 'a' && v2 == 'e') return "eye";
  if (v1 == 'a' && v2 == 'o') return "ow";
  if (v1 == 'a' && v2 == 'u') return "ow";
  if (v1 == 'e' && v2 == 'i') return "ay";
  if (v1 == 'e' && v2 == 'u') return "eh-oo";
  if (v1 == 'i' && v2 == 'u') return "ew";
  if (v1 == 'o' && v2 == 'i') return "oy";
  if (v1 == 'o' && v2 == 'u') return "ow";
  if (v1 == 'u' && v2 == 'i') return "ooey";

  return "";
}

/**
 * TODO: Implement this function
 * @param v single vowel to get pronunciation of
 * @return the pronunciation of v as a string
 */
std::string SingleVowelPronunciation(const char& v) {
  std::cout << "Input to SingleVowelPronunciation() : \"" << v << "\""
            << std::endl;
  std::string pronunciation;
    switch (v) {
      case 'a':
        pronunciation = "ah";
        break;
      case 'e':
        pronunciation = "eh";
        break;
      case 'i':
        pronunciation = "ee";
        break;
      case 'o':
        pronunciation = "oh";
        break;
      case 'u':
        pronunciation = "oo";
        break;
      default:
        break;
    }
  return pronunciation;
}

/**
 * TODO: Implement this function
 * @param prev first character in set of three passed to function
 * @param curr second character in set of three passed to function
 * @param next third character in set of three passed to function
 * @return pronunciation of curr using next and prev as needed to determine
 * result
 */
std::string ProcessCharacter(const char& prev,
                             const char& curr,
                             const char& next) {
  std::cout << "Input to ProcessCharacter() : \"" << prev << "\" \"" << curr
            << "\" \"" << next << "\"" << std::endl;
  if (IsConsonant(curr)) {
    return {ConsonantPronunciation(curr, prev)};
  } else if (IsVowel(curr)) {
    if (IsVowel(next) && IsVowelGroup(curr, next)) {
        return VowelGroupPronunciation(curr, next);
    } else {
        return SingleVowelPronunciation(curr);
    }
  } else if (curr == ' ') {
    return " ";
  } else if (curr == '\'') {
    return "'";
  } else {
    throw std::invalid_argument("Invalid character");

  } 
  //throw std::invalid_argument("Invalid character");

}

/**
 * TODO: Implement this function
 * @param word string to get pronunciation of
 * @return pronunciation of word
 */
std::string Pronunciation(const std::string& word) {
  std::cout << "Input to Pronunciation() : " << word << std::endl;
  std::string result;
  std::string lowercase_word = StringToLower(word);
  char prev = ' ';
  char next = ' ';
  for (size_t i = 0; i < lowercase_word.length(); ++i) {
    char curr = lowercase_word[i];

    if (i == 0) {
      prev = '\0'; 
    } else {
      prev = lowercase_word[i - 1];
    }
    //less character
    if (i == lowercase_word.length() - 1) {
      next = '\0';
    } else {
      next = lowercase_word[i + 1];
    }
    result += ProcessCharacter(prev, curr, next);
    // Skip next character if we're using a vowel group
    if (IsVowelGroup(curr, next)) {
      if (lowercase_word[i + 2] != ' ' && lowercase_word[i + 2] != '\'' && i + 1 != lowercase_word.length() - 1) {
        result += '-';
      }
      i++;
    } else if (IsVowel(curr)) {
        if (next != ' ' && next != '\'' && i != lowercase_word.length() - 1) {
          result += '-';
        }
    }
  }
  return result;
}

/**
 * TODO: Implement this function
 * @param hawaiian_word word to get pronunciation of
 * @return the pronunciation of hawaiian_word as a string if valid
 *  and throws an invalid_argument error if not valid
 */
std::string GetPronunciation(const std::string& hawaiian_word) {
  std::cout << "Input to GetPronunciation() : " << hawaiian_word << std::endl;
  std::string lower_hawaiian = StringToLower(hawaiian_word);
  if (!IsValidWord(lower_hawaiian)) {
    throw std::invalid_argument("Invalid Hawaiian word");
  }
  try {
    return Pronunciation(lower_hawaiian);
  } catch (const std::invalid_argument& e) {
    std::cerr << "Error in GetPronunciation: " << e.what() << std::endl;
    throw; 
  }
  //return Pronunciation(lower_hawaiian);
}