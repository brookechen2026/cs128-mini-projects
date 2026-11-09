#include <ctime>
#include <fstream>
#include <iostream>

#include "constants.hpp"
#include "group.hpp"

#include "volunteer.hpp"
#include <vector>
#include <string>
#include <cstdlib> // For rand()
#include <ctime> 

/*Write your main driver in this file! Remember to seperate
out things into functions to help with readability and reusability.

See specifications for how to get started and
go to office hours if you need help!*/

std::vector<Volunteer> ReadVolunteers(const std::string& input_file);
bool WriteGroups(const std::string& output_file, const std::vector<Group>& groups);
std::vector<Group> VolunteersToGroups(const std::string& input_file);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[3];

    std::vector<Group> groups = VolunteersToGroups(input_file);

    int swap = 0;
    for (size_t i = 0; i < groups.size(); i++) {
        if (groups.at(i).GetAvgBuildingExp() < kMinAvgBuildingExp || groups.at(i).GetAvgStamina() < kMinAvgStamina || groups.at(i).GetReturningMembers() < kMinReturning) {

            unsigned int replace1 = rand() % groups.size();
            unsigned int replace2 = rand() % groups.size();
            while (replace2 == replace1) {
                replace2 = rand() % groups.size();
            }
            
            Volunteer& replace_v1 = groups.at(replace1).GetRandomVolunteer();
            Volunteer& replace_v2 = groups.at(replace2).GetRandomVolunteer();
            
            std::swap(replace_v1, replace_v2);
            i = -1;
            swap++;
        }
        if (swap >= int(kMaxIterations)) {
            std::cout << "No groups found" << std::endl;
            groups = std::vector<Group>();
            break; 
        }
    }

    if (WriteGroups(output_file, groups)) {
        std::cout << "Groups written to file: " << output_file << std::endl;
        return 0;
    } 
    return 1;
}

 std::vector<Volunteer> ReadVolunteers(const std::string& input_file) {
    std::ifstream infile(input_file);
    std::vector<Volunteer> volunteer_vector;

    if (!infile.is_open()) {
        return volunteer_vector;
    }

    std::string name;
    int building_experience = 0;
    int physical_stamina = 0;
    bool returning = false;

    while (infile >> name >> building_experience >> physical_stamina >> returning) {
        Volunteer vol(name, building_experience, physical_stamina, returning);
        
        volunteer_vector.push_back(vol);
    }
    infile.close();
    return volunteer_vector;
}

bool WriteGroups(const std::string& output_file, const std::vector<Group>& groups) {
    std::ofstream outfile(output_file);
    if (!outfile.is_open()) {
        return false;
    }

    for (const auto& group : groups) {
        outfile << group << std::endl;
    }
    outfile.close();
    return true;
}

std::vector<Group> VolunteersToGroups(const std::string& input_file) {
    std::vector<Volunteer> volunteers = ReadVolunteers(input_file);
    std::vector<Group> groups;
    int c = 0;

    Group group;
    for (size_t i = 0; i < volunteers.size(); i++) {
        if (c < kSizeGroups) {
            group.AddVolunteer(volunteers.at(i));
            c++;
        } 
        if (c == kSizeGroups && i != volunteers.size() - 1) {
            groups.push_back(group);
            group = Group();
            c = 0;
        }
        if (c == kSizeGroups && i == volunteers.size() - 1) {
           groups.push_back(group);
        }
    }

    return groups;
}

// std::vector<Volunteer> volunteers = ReadVolunteers(input_file);
    // std::vector<Group> groups;
    // int c = 0;

    // Group group;
    // for (size_t i = 0; i < volunteers.size(); i++) {
    //     if (c < kSizeGroups) {
    //         group.AddVolunteer(volunteers.at(i));
    //         c++;
    //     } 
    //     if (c == kSizeGroups && i != volunteers.size() - 1) {
    //         groups.push_back(group);
    //         group = Group();
    //         c = 0;
    //     }
    //     if (c == kSizeGroups && i == volunteers.size() - 1) {
    //        groups.push_back(group);
    //     }
    // }