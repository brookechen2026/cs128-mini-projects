#include "db_table.hpp"

//const unsigned int kRowGrowthRate = 2;

void DbTable::AddColumn(const std::pair<std::string, DataType>& col_desc) {
    if (col_descs_.size() == row_col_capacity_) {
        row_col_capacity_ *= 2;
        
        for (auto& row : rows_) {
            void** new_row = new void*[row_col_capacity_];
            std::copy(row.second, row.second + col_descs_.size(), new_row);
            delete[] row.second;
            row.second = new_row;
        }
    }

    col_descs_.push_back(col_desc);

    for (auto& row : rows_) {
        switch (col_desc.second) {
            case DataType::kString:
                row.second[col_descs_.size() - 1] = new std::string();
                break;
            case DataType::kDouble:
                row.second[col_descs_.size() - 1] = new double(0.0);
                break;
            case DataType::kInt:
                row.second[col_descs_.size() - 1] = new int(0);
                break;
        }
    }
}

void DbTable::DeleteColumnByIdx(unsigned int col_idx) {
    if (col_idx >= col_descs_.size()) {
        throw std::out_of_range("col_idx is outside the range of col_descs_.");
    }
    if (col_descs_.size() == 1 && !rows_.empty()) {
        throw std::runtime_error("exception: any table with at least one row must have at least one column.");
    }

    for (auto& row : rows_) {
        switch (col_descs_[col_idx].second) {
            case DataType::kString:
                delete static_cast<std::string*>(row.second[col_idx]);
                break;
            case DataType::kDouble:
                delete static_cast<double*>(row.second[col_idx]);
                break;
            case DataType::kInt:
                delete static_cast<int*>(row.second[col_idx]);
                break;
        }

        for (unsigned int i = col_idx; i < col_descs_.size() - 1; i++) {
            row.second[i] = row.second[i + 1];
        }
    }

    col_descs_.erase(col_descs_.begin() + col_idx);
}

void DbTable::AddRow(const std::initializer_list<std::string>& col_data) {
    if (col_data.size() != col_descs_.size()) {
        throw std::invalid_argument("Number of columns in col_data does not match number of columns in the table");
    }

    void** new_row = new void*[row_col_capacity_];
    rows_.insert({next_unique_id_, new_row});

    unsigned int i = 0;
    for (const std::string& str : col_data) {
        switch (col_descs_[i].second) {
            case DataType::kString:
                new_row[i] = static_cast<void*>(new std::string(str));
                break;
            case DataType::kDouble:
                new_row[i] = static_cast<void*>(new double(std::stod(str)));
                break;
            case DataType::kInt:
                new_row[i] = static_cast<void*>(new int(std::stoi(str)));
                break;
        }
        i++;
    }

    next_unique_id_++;
}

void DbTable::DeleteRowById(unsigned int id) {
    auto it = rows_.find(id);
    if (it == rows_.end()) {
        throw std::invalid_argument("Row with specified ID does not exist");
    }

    for (unsigned int i = 0; i < col_descs_.size(); i++) {
        switch (col_descs_[i].second) {
            case DataType::kString:
                delete static_cast<std::string*>(it->second[i]);
                break;
            case DataType::kDouble:
                delete static_cast<double*>(it->second[i]);
                break;
            case DataType::kInt:
                delete static_cast<int*>(it->second[i]);
                break;
        }
    }

    delete[] it->second;

    rows_.erase(it);
}

DbTable::DbTable(const DbTable& rhs) {
    row_col_capacity_ = rhs.row_col_capacity_;
    next_unique_id_ = rhs.next_unique_id_;
    col_descs_ = rhs.col_descs_;

    for (const auto& [key, value] : rhs.rows_) {
        void** new_row = new void*[row_col_capacity_];

        for (unsigned int i = 0; i < col_descs_.size(); i++) {
            switch (col_descs_[i].second) {
                case DataType::kString:
                    new_row[i] = new std::string(*static_cast<std::string*>(value[i]));
                    break;
                case DataType::kDouble:
                    new_row[i] = new double(*static_cast<double*>(value[i]));
                    break;
                case DataType::kInt:
                    new_row[i] = new int(*static_cast<int*>(value[i]));
                    break;
            }
        }

        rows_.insert({key, new_row});
    }
}

DbTable& DbTable::operator=(const DbTable& rhs) {
    if (this == &rhs) {
        return *this;
    }

    for (auto& row : rows_) {
        for (size_t i = 0; i < col_descs_.size(); ++i) {
            DeleteData(row.second[i], col_descs_[i].second);
        }
        delete[] row.second;
    }
    rows_.clear();

    next_unique_id_ = rhs.next_unique_id_;
    row_col_capacity_ = rhs.row_col_capacity_;
    col_descs_ = rhs.col_descs_;

    for (const auto& row : rhs.rows_) {
        void** new_row = new void*[row_col_capacity_];
        for (size_t i = 0; i < col_descs_.size(); ++i) {
            new_row[i] = CopyData(row.second[i], col_descs_[i].second);
        }
        rows_[row.first] = new_row;
    }

    return *this;
}

void DbTable::DeleteData(void* data, DataType type) {
    switch (type) {
        case DataType::kString:
            delete static_cast<std::string*>(data);
            break;
        case DataType::kInt:
            delete static_cast<int*>(data);
            break;
        case DataType::kDouble:
            delete static_cast<double*>(data);
            break;
    }
}

void* DbTable::CopyData(void* data, DataType type) {
    switch (type) {
        case DataType::kString:
            return new std::string(*static_cast<std::string*>(data));
        case DataType::kInt:
            return new int(*static_cast<int*>(data));
        case DataType::kDouble:
            return new double(*static_cast<double*>(data));
    }
    return nullptr;
}

  
DbTable::~DbTable() {
    for (auto const& [id,value] : rows_) {
        for (unsigned int i = 0; i < col_descs_.size(); i++) {
            if (col_descs_[i].second == DataType::kDouble) {
                delete static_cast<double*>(value[i]);
            } else if (col_descs_[i].second == DataType::kInt) {
                delete static_cast<int*>(value[i]);
            } else {
                delete static_cast<std::string*>(value[i]);
            }
        }
        delete[] value;
        rows_.at(id) = nullptr;
    }
    for (unsigned int i = 0; i < next_unique_id_; i++) {
        rows_.erase(i);
    }
}

std::ostream& operator<<(std::ostream& os, const DbTable& table) {
    for (const auto& desc : table.col_descs_) {
        os << desc.first << "(";
        switch (desc.second) {
            case DataType::kString:
                os << "std::string)";
                break;
            case DataType::kInt:
                os << "int)";
                break;
            case DataType::kDouble:
                os << "double)";
                break;
        }
        if (&desc != &table.col_descs_.back()) {
            os << ", ";
        } else {
            os << "\n";
        }
    }
    for (const auto& [_, row] : table.rows_) {
        for (size_t col_idx = 0; col_idx < table.col_descs_.size(); ++col_idx) {
            DataType type = table.col_descs_[col_idx].second;
            switch (type) {
                case DataType::kString:
                    os << *(static_cast<std::string*>(row[col_idx]));
                    break;
                case DataType::kInt:
                    os << *(static_cast<int*>(row[col_idx]));
                    break;
                case DataType::kDouble:
                    os << *(static_cast<double*>(row[col_idx]));
                    break;
            }
            if (col_idx < table.col_descs_.size() - 1) os << ", ";
        }
        os << std::endl;
    }
    return os;
}