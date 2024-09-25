#include "hash_202.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>

// Method to set up the hash table with a given size, hash function, and collision strategy.
std::string Hash_202::Set_Up(size_t table_size, const std::string &fxn, const std::string &collision) {
    // Check if the hash table is already set up
    if (!Keys.empty()) return "Hash table already set up";
    // Check for a valid table size
    if (table_size == 0) return "Bad table size";

    // Determine which hash function to use
    if (fxn == "Last7") {
        Fxn = 'L'; // Use the "Last7" function
    } else if (fxn == "XOR") {
        Fxn = 'X'; // Use the "XOR" function
    } else {
        return "Bad hash function";
    }

    // Determine which collision resolution strategy to use
    if (collision == "Linear") {
        Coll = 'L'; // Use linear probing
    } else if (collision == "Double") {
        Coll = 'D'; // Use double hashing
    } else {
        return "Bad collision resolution strategy";
    }

    // Initialize the keys and values with the specified table size
    Keys.resize(table_size);
    Vals.resize(table_size);
    Nkeys = 0; // Set the number of keys to zero

    return ""; // Return an empty string if successful
}

// Method to add a key-value pair to the hash table
std::string Hash_202::Add(const std::string &key, const std::string &val) {
    // Check if the hash table is set up
    if (Keys.empty()) return "Hash table not set up";
    // Check if the key or value is empty
    if (key.empty()) return "Empty key";
    if (val.empty()) return "Empty val";

    // Check if the key contains only hexadecimal digits
    for (char c : key) {
        if (!isxdigit(c)) return "Bad key (not all hex digits)";
    }

    // Check if the hash table is full
    if (Nkeys == Keys.size()) return "Hash table full";

    // Calculate the starting index for the key
    size_t index = Hash(key);
    size_t original_index = index;
    size_t step = 1;

    // Find an empty spot using the collision resolution strategy
    while (!Keys[index].empty()) {
        // Check if the key already exists in the table
        if (Keys[index] == key) return "Key already in the table";
        if (Coll == 'L') {
            // Linear probing
            index = (index + 1) % Keys.size();
        } else {
            // Double hashing
            size_t double_hash = DoubleHash(key);
            index = (index + double_hash) % Keys.size();
        }
        // If we return to the original index, we cannot insert the key
        if (index == original_index) return "Cannot insert key";
    }

    // Insert the key and value into the table
    Keys[index] = key;
    Vals[index] = val;
    Nkeys++; // Increment the count of keys
    return ""; // Return an empty string if successful
}

// Method to find a value associated with a key
std::string Hash_202::Find(const std::string &key) {
    // Return an empty string if the hash table is not set up
    if (Keys.empty()) return "";
    // Check if the key contains only hexadecimal digits
    for (char c : key) {
        if (!isxdigit(c)) return "";
    }

    // Calculate the starting index for the key
    size_t index = Hash(key);
    size_t original_index = index;
    size_t step = 1;
    Nprobes = 0; // Reset the number of probes

    // Search for the key in the table
    while (!Keys[index].empty()) {
        Nprobes++; // Increment the probe count
        // Return the value if the key is found
        if (Keys[index] == key) return Vals[index];
        if (Coll == 'L') {
            // Linear probing
            index = (index + 1) % Keys.size();
        } else {
            // Double hashing
            size_t double_hash = DoubleHash(key);
            index = (index + double_hash) % Keys.size();
        }
        // If we return to the original index, the key is not in the table
        if (index == original_index) break;
    }

    return ""; // Return an empty string if the key is not found
}

// Method to print all the non-empty entries in the hash table
void Hash_202::Print() const {
    if (Keys.empty()) return; // Do nothing if the table is not set up
    for (size_t i = 0; i < Keys.size(); ++i) {
        if (!Keys[i].empty()) {
            // Print the index, key, and value
            std::cout << std::setw(5) << i << " " << Keys[i] << " " << Vals[i] << std::endl;
        }
    }
}

// Method to calculate the total number of probes needed to find all keys
size_t Hash_202::Total_Probes() {
    if (Keys.empty()) return 0; // Return zero if the table is not set up
    size_t total_probes = 0;
    for (const auto &key : Keys) {
        if (!key.empty()) {
            Find(key);  // This sets Nprobes for the current key
            total_probes += Nprobes; // Add the probe count to the total
        }
    }
    return total_probes; // Return the total number of probes
}

// Method to compute the hash of a key
size_t Hash_202::Hash(const std::string &key) const {
    if (Fxn == 'L') {
        // "Last7" hash function
        size_t result = 0;
        for (size_t i = key.size() >= 7 ? key.size() - 7 : 0; i < key.size(); ++i) {
            result = (result << 4) + (key[i] >= '0' && key[i] <= '9' ? key[i] - '0' : toupper(key[i]) - 'A' + 10);
        }
        return result % Keys.size();
    } else if (Fxn == 'X') {
        // "XOR" hash function
        size_t result = 0;
        for (char c : key) {
            result ^= (c >= '0' && c <= '9' ? c - '0' : toupper(c) - 'A' + 10);
        }
        return result % Keys.size();
    }
    return 0; // Return zero if no valid hash function is set
}

// Method to compute a secondary hash value for double hashing
size_t Hash_202::DoubleHash(const std::string &key) const {
    size_t hash_val = 0;
    for (char c : key) {
        hash_val = (hash_val << 4) + (c >= '0' && c <= '9' ? c - '0' : toupper(c) - 'A' + 10);
    }
    // Ensure the double hash step is non-zero
    return (hash_val % (Keys.size() - 1)) + 1;
}
s