#include "code_processor.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

// Hash function for codes
unsigned long djb_hash(const std::string &str) {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

bool Code_Processor::New_Prize(const std::string &id, const std::string &description, int points, int quantity) {
    if (Prizes.find(id) != Prizes.end() || points <= 0 || quantity <= 0) {
        return false;
    }
    Prize *prize = new Prize{id, description, points, quantity};
    Prizes[id] = prize;
    return true;
}


bool Code_Processor::New_User(const std::string &username, const std::string &realname, int starting_points) {
    if (Names.find(username) != Names.end() || starting_points < 0) {
        return false;
    }
    User *user = new User{username, realname, starting_points, {}};
    Names[username] = user;
    return true;
}

bool Code_Processor::Delete_User(const std::string &username) {
    auto it = Names.find(username);
    if (it == Names.end()) {
        return false;
    }
    
    User *user = it->second;
    for (const std::string &phone : user->phone_numbers) {
        Phones.erase(phone);
    }
    
    delete user;
    Names.erase(it);
    return true;
}

bool Code_Processor::Add_Phone(const std::string &username, const std::string &phone) {
    auto it = Names.find(username);
    if (it == Names.end() || Phones.find(phone) != Phones.end()) {
        return false;
    }
    
    User *user = it->second;
    user->phone_numbers.insert(phone);
    Phones[phone] = user;
    return true;
}

bool Code_Processor::Remove_Phone(const std::string &username, const std::string &phone) {
    auto it = Names.find(username);
    if (it == Names.end()) {
        return false;
    }
    
    User *user = it->second;
    if (user->phone_numbers.find(phone) == user->phone_numbers.end() || Phones[phone] != user) {
        return false;
    }
    
    user->phone_numbers.erase(phone);
    Phones.erase(phone);
    return true;
}

std::string Code_Processor::Show_Phones(const std::string &username) const {
    auto it = Names.find(username);
    if (it == Names.end()) {
        return "BAD USER";
    }
    
    const User *user = it->second;
    std::string result;
    for (const std::string &phone : user->phone_numbers) {
        result += phone + "\n";
    }
    return result;
}

int Code_Processor::Enter_Code(const std::string &username, const std::string &code) {
    auto it = Names.find(username);
    if (it == Names.end() || Codes.find(code) != Codes.end()) {
        return -1;
    }
    
    unsigned long hash = djb_hash(code);
    int points = 0;
    if (hash % 17 == 0) {
        points = 10;
    } else if (hash % 13 == 0) {
        points = 3;
    } else {
        return 0;
    }
    
    Codes.insert(code);
    it->second->points += points;
    return points;
}

int Code_Processor::Text_Code(const std::string &phone, const std::string &code) {
    auto it = Phones.find(phone);
    if (it == Phones.end() || Codes.find(code) != Codes.end()) {
        return -1;
    }
    
    return Enter_Code(it->second->username, code);
}

bool Code_Processor::Mark_Code_Used(const std::string &code) {
    unsigned long hash = djb_hash(code);
    if (Codes.find(code) != Codes.end() || (hash % 17 != 0 && hash % 13 != 0)) {
        return false;
    }
    
    Codes.insert(code);
    return true;
}

int Code_Processor::Balance(const std::string &username) const {
    auto it = Names.find(username);
    if (it == Names.end()) {
        return -1;
    }
    return it->second->points;
}

bool Code_Processor::Redeem_Prize(const std::string &username, const std::string &prize_id) {
    auto user_it = Names.find(username);
    auto prize_it = Prizes.find(prize_id);
    if (user_it == Names.end() || prize_it == Prizes.end()) {
        return false;
    }
    
    User *user = user_it->second;
    Prize *prize = prize_it->second;
    
    if (user->points < prize->points || prize->quantity <= 0) {
        return false;
    }

    user->points -= prize->points;
    prize->quantity -= 1;

    if (prize->quantity == 0) {
        delete prize;
        Prizes.erase(prize_it);
    }
    
    return true;
}

Code_Processor::~Code_Processor() {
    for (auto &pair : Names) {
        delete pair.second;
    }
    for (auto &pair : Prizes) {
        delete pair.second;
    }
}

bool Code_Processor::Write(const std::string &filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto &pair : Names) {
        const User *user = pair.second;
        file << "ADD_USER " << user->username << " " << user->points << " " << user->realname << "\n";
        for (const std::string &phone : user->phone_numbers) {
            file << "ADD_PHONE " << user->username << " " << phone << "\n";
        }
    }
    
    for (const auto &pair : Prizes) {
        const Prize *prize = pair.second;
        file << "PRIZE " << prize->id << " " << prize->points << " " << prize->quantity << " " << prize->description << "\n";
    }
    
    for (const std::string &code : Codes) {
        file << "MARK_USED " << code << "\n";
    }
    
    file.close();
    return true;
}