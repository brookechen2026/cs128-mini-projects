#include <iostream>
#include "group.hpp"
#include "volunteer.hpp"

#include <cstdlib>
#include <ctime>


void Group::AddVolunteer(const Volunteer& vol) {
    volunteers_.push_back(vol);
}

double Group::GetAvgBuildingExp() const {
    if (volunteers_.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& vol : volunteers_) {
        sum += vol.GetBuildingExperience();
    }
    return sum / static_cast<double>(volunteers_.size());
}

int Group::GetReturningMembers() const {
    int returning_count = 0;
    for (const auto& vol : volunteers_) {
        if (vol.IsReturning()) {
            returning_count++;
        }
    }
    return returning_count;
}

double Group::GetAvgStamina() const {
    if (volunteers_.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& vol : volunteers_) {
        sum += vol.GetPhysicalStamina();
    }
    return sum / static_cast<double>(volunteers_.size());
}

std::ostream& operator<<(std::ostream& os, const Group& g_in) {
    os << "Avg Building Exp: " << g_in.GetAvgBuildingExp() << ", Avg Stamina: " << g_in.GetAvgStamina() << ", Returning Members: " << g_in.GetReturningMembers() << std::endl;
    for (const auto& vol : g_in.volunteers_) {
        os << vol.GetName() << std::endl;
    }
    return os;
}

Volunteer& Group::GetRandomVolunteer() {
    unsigned int index = rand() % volunteers_.size(); // Generate a random index
    return volunteers_[index];
}