#include "volunteer.hpp"

Volunteer::Volunteer(const std::string& name, int building_experience, int physical_stamina, bool returning) 
    : name_(name), 
      building_experience_(EnsureWithinRange(building_experience, kMinBuildingExp, kMaxBuildingExp)), 
      physical_stamina_(EnsureWithinRange(physical_stamina, kMinStamina, kMaxStamina)), 
      returning_(returning) {

    if (building_experience_ < kMinBuildingExp) {
        building_experience_ = kMinBuildingExp;
    } else if (building_experience_ > kMaxBuildingExp) {
        building_experience_ = kMaxBuildingExp;
    }

    if (physical_stamina_ < kMinStamina) {
        physical_stamina_ = kMinStamina;
    } else if (physical_stamina_ > kMaxStamina) {
        physical_stamina_ = kMaxStamina;
    }

}

std::string Volunteer::GetName() const {
    return name_;
}

int Volunteer::GetBuildingExperience() const {
    return building_experience_;
}

int Volunteer::GetPhysicalStamina() const {
    return physical_stamina_;
}

bool Volunteer::IsReturning() const {
    return returning_;
}

int Volunteer::EnsureWithinRange(int value, int min, int max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}