#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>
#include <cstdint>

int64_t GetWays(int64_t time, int64_t distance) {
    int64_t count = 0;
    for (int64_t speed = 1; speed < time; ++speed) {
        int64_t total = (time - speed) * speed;

        if (total > distance) {
            ++count;
        }
    }
    return count;
}

int main() {
    std::ifstream file("1.txt");

    if (!file.is_open()) {
        std::cout << "OpenFile Error!" << std::endl;
        return 1;
    }
    std::vector<int64_t> distances, times;
    std::string combined_distance_str, combined_time_str;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        std::vector<int64_t> *container = nullptr;
        std::string *combined_str;
        if (token.find("Time:") != std::string::npos) {
            container = &times;
            combined_str = &combined_time_str;
        } else if (token.find("Distance:") != std::string::npos) {
            container = &distances;
            combined_str = &combined_distance_str;
        }

        if (container != nullptr) {
            int64_t value;
            while (iss >> value) {
                container->push_back(value);
                *combined_str += std::to_string(value);
            }
        }
    }
    file.close();

    int64_t ways = 1;
    for (size_t i = 0; i < times.size(); ++i) {
        ways *= GetWays(times[i], distances[i]);
    }
    std::cout << "Part1:" << ways << std::endl;

    int64_t combined_distance = std::stoll(combined_distance_str);
    int64_t combined_time = std::stoll(combined_time_str);
    std::cout << "Part2:" << GetWays(combined_time, combined_distance) << std::endl;

    return 0;
}