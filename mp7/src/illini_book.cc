#include "illini_book.hpp"
#include "utilities.hpp"

IlliniBook::IlliniBook(const std::string& people_fpath, const std::string& relations_fpath) {
    std::ifstream people_file(people_fpath), relations_file(relations_fpath);
    if (!people_file.is_open() || !relations_file.is_open())
        throw std::runtime_error("Failed to open file");

    std::string line;
    while (std::getline(people_file, line)) {
        int uin = std::stoi(line);
        graph_[uin] = {}; // Initialize an empty map for each UIN
    }

    while (std::getline(relations_file, line)) {
        auto parts = utilities::Split(line, ',');
        int uin_a = std::stoi(parts[0]);
        int uin_b = std::stoi(parts[1]);
        std::string relationship = parts[2];
        graph_[uin_a][uin_b] = relationship;
        graph_[uin_b][uin_a] = relationship; // Assuming relationships are bidirectional
    }
}
IlliniBook::~IlliniBook() = default;

bool IlliniBook::AreRelated(int uin_1, int uin_2) const {
    std::set<int> visited;
    std::queue<int> queue;
    queue.push(uin_1);
    while (!queue.empty()) {
        int curr = queue.front();
        if (curr == uin_2) {
            return true;
        }
        visited.insert(curr);
        for (auto it = graph_.at(curr).begin(); it != graph_.at(curr).end(); it++) {
            if (!visited.contains(it->first)) {
                queue.push(it->first);
            }
        }
        queue.pop();
    }
    return false;
}

bool IlliniBook::AreRelated(int uin_1, int uin_2, const std::string &relationship) const {
    std::set<int> visited;
    std::queue<int> queue;
    queue.push(uin_1);
    while (!queue.empty()) {
        int curr = queue.front();
        visited.insert(curr);
        for (auto it = graph_.at(curr).begin(); it != graph_.at(curr).end(); it++) {
            if (!visited.contains(it->first) && it -> second == relationship) {
                if (it -> first == uin_2) {
                    return true;
                }
                queue.push(it->first);
            }
        }
        queue.pop();
    }
    return false;
}


int IlliniBook::GetRelated(int uin_1, int uin_2) const {
    if (graph_.find(uin_1) == graph_.end() || graph_.find(uin_2) == graph_.end())
        return -1;

    std::queue<std::pair<int, int>> to_visit;
    std::set<int> visited;
    to_visit.push({uin_1, 0});
    visited.insert(uin_1);

    while (!to_visit.empty()) {
        auto current = to_visit.front();
        to_visit.pop();
        for (const auto& [neighbor, relationship] : graph_.at(current.first)) {
            if (neighbor == uin_2)
                return current.second + 1;
            if (visited.find(neighbor) == visited.end()) {
                to_visit.push({neighbor, current.second + 1});
                visited.insert(neighbor);
            }
        }
    }
    return -1;
}

int IlliniBook::GetRelated(int uin_1, int uin_2, const std::string &relationship) const {
    std::set<int> visited;
    std::queue<std::pair<int, int>> queue;
    queue.push({uin_1, 0});

    while (!queue.empty()) {
        std::pair<int, int> packet = queue.front();
        int curr = packet.first;
        int dist = packet.second + 1;
        visited.insert(curr);
        for (auto it = graph_.at(curr).begin(); it != graph_.at(curr).end(); it++) {
            if (!visited.contains(it->first) && it -> second == relationship) {
                if (it -> first == uin_2) {
                    return dist;
                }
                queue.push({it->first, dist});
            }
        }
        queue.pop();
    }
    return -1;
}


std::vector<int> IlliniBook::GetSteps(int uin, int n) const {
    if (graph_.find(uin) == graph_.end())
        return {};

    std::vector<int> steps;
    std::queue<std::pair<int, int>> to_visit;
    std::set<int> visited;
    to_visit.push({uin, 0});
    visited.insert(uin);

    while (!to_visit.empty()) {
        auto current = to_visit.front();
        to_visit.pop();
        if (current.second == n)
            steps.push_back(current.first);
        else if (current.second > n)
            break;
        for (const auto& [neighbor, relationship] : graph_.at(current.first)) {
            if (visited.find(neighbor) == visited.end()) {
                to_visit.push({neighbor, current.second + 1});
                visited.insert(neighbor);
            }
        }
    }
    return steps;
}

size_t IlliniBook::CountGroups() const {
    std::set<int> visited;
    size_t groups = 0;
    for (const auto& [uin, neighbors] : graph_) {
        if (visited.find(uin) == visited.end()) {
            std::queue<int> to_visit;
            to_visit.push(uin);
            visited.insert(uin);
            groups++;

            while (!to_visit.empty()) {
                int current = to_visit.front();
                to_visit.pop();
                for (const auto& [neighbor, relationship] : graph_.at(current)) {
                    if (visited.find(neighbor) == visited.end()) {
                        to_visit.push(neighbor);
                        visited.insert(neighbor);
                    }
                }
            }
        }
    }
    return groups;
}

size_t IlliniBook::CountGroups(const std::string& relationship) const {
    std::set<int> visited;
    size_t groups = 0;
    for (const auto& [uin, neighbors] : graph_) {
        if (visited.find(uin) == visited.end()) {
            std::queue<int> to_visit;
            to_visit.push(uin);
            visited.insert(uin);
            groups++;

            while (!to_visit.empty()) {
                int current = to_visit.front();
                to_visit.pop();
                for (const auto& [neighbor, rel] : graph_.at(current)) {
                    if (visited.find(neighbor) == visited.end() && rel == relationship) {
                        to_visit.push(neighbor);
                        visited.insert(neighbor);
                    }
                }
            }
        }
    }
    return groups;
}

size_t IlliniBook::CountGroups(const std::vector<std::string>& relationships) const {
    std::set<int> visited;
    size_t groups = 0;
    for (const auto& [uin, neighbors] : graph_) {
        if (visited.find(uin) == visited.end()) {
            std::queue<int> to_visit;
            to_visit.push(uin);
            visited.insert(uin);
            groups++;

            while (!to_visit.empty()) {
                int current = to_visit.front();
                to_visit.pop();
                for (const auto& [neighbor, relationship] : graph_.at(current)) {
                    if (visited.find(neighbor) == visited.end() && HasAnyRelationship(current, neighbor, relationships)) {
                        to_visit.push(neighbor);
                        visited.insert(neighbor);
                    }
                }
            }
        }
    }
    return groups;
}

bool IlliniBook::HasAnyRelationship(int uin_1, int uin_2, const std::vector<std::string>& relationships) const {
    if (graph_.find(uin_1) == graph_.end() || graph_.find(uin_2) == graph_.end())
        return false;

    for (const auto& relationship : relationships) {
        if (graph_.at(uin_1).find(uin_2)->second == relationship)
            return true;
    }
    return false;
}
