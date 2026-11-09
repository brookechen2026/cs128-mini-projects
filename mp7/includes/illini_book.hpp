#ifndef ILLINI_BOOK_HPP
#define ILLINI_BOOK_HPP

#include <fstream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

class IlliniBook {
private:
    std::map<int, std::map<int, std::string>> graph_;
public:
    IlliniBook(const std::string& people_fpath, const std::string& relations_fpath);
    IlliniBook(const IlliniBook& rhs) = delete;
    IlliniBook& operator=(const IlliniBook& rhs) = delete;
    ~IlliniBook();

    bool AreRelated(int uin_1, int uin_2) const;
    bool AreRelated(int uin_1, int uin_2, const std::string& relationship) const;
    int GetRelated(int uin_1, int uin_2) const;
    int GetRelated(int uin_1, int uin_2, const std::string& relationship) const;
    std::vector<int> GetSteps(int uin, int n) const;
    size_t CountGroups() const;
    size_t CountGroups(const std::string& relationship) const;
    size_t CountGroups(const std::vector<std::string>& relationships) const;

private:
    bool HasAnyRelationship(int uin_1, int uin_2, const std::vector<std::string>& relationships) const;
};

#endif // ILLINI_BOOK_HPP
