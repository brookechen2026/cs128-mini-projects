#include "db.hpp"

void Database::CreateTable(const std::string& table_name) {
    auto* new_table = new DbTable();
    tables_.insert({table_name, new_table});
}

void Database::DropTable(const std::string& table_name) {
    if (!tables_.contains(table_name)) {
        throw std::invalid_argument("Table does not exist");
    }
    delete tables_[table_name];
    tables_.erase(table_name);
}


DbTable& Database::GetTable(const std::string& table_name) {
    if (!tables_.contains(table_name)) {
        throw std::invalid_argument("Table does not exist");
    }
    return *tables_[table_name];
}


Database::Database(const Database& rhs) {
    for (auto const& [key,value] : rhs.tables_) {
        auto* pair = new DbTable(*value);
        tables_.insert({key, pair});
    }
}

Database& Database::operator=(const Database& rhs) {
    if (&rhs == this) {
        return *this;
    }
    std::vector<std::string> keys;
    for (auto const& [key,value] : tables_) {
        keys.push_back(key);
        delete value;
    }
    for (unsigned int i = 0; i < keys.size(); i++) {
        tables_.erase(keys[i]);
    }
    for (auto const& [key,value] : rhs.tables_) {
        auto* pair = new DbTable(*value);
        tables_.insert({key, pair});
    }
    return *this;
}

Database::~Database() {
    for (auto& pair : tables_) {
        delete pair.second;
    }
}

std::ostream& operator<<(std::ostream& os, const Database& db) {
    for (const auto& pair : db.tables_) {
        os << *(pair.second) << std::endl;
    }
    return os;
}
