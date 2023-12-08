#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <cstdint>
#include <numeric>

bool readData(const std::string &filename, std::string &command, std::map<std::string, std::pair<std::string, std::string>> &nodes, std::vector<std::string> &start_nodes) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file >> command;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            std::istringstream iss(line);
            std::string node, edges;
            iss >> node;
            if (node[2] == 'A') {
                start_nodes.push_back(node);
            }
            std::string left_node = line.substr(7, 3);
            std::string rigth_node = line.substr(12, 3);
            nodes[node] = {left_node, rigth_node};
        }
    }

    file.close();
    return true;
}

int getStepCount(const std::string &start, std::string &command, std::map<std::string, std::pair<std::string, std::string>> &nodes) {
    int steps = 0;

    int command_index = 0;
    std::string node = start;
    while (node[2] != 'Z') {
        auto pair = nodes[node];
        char ch = command[command_index];
        ++command_index;
        command_index %= static_cast<int>(command.size());
        if (ch == 'R') {
            node = pair.second;
        } else {
            node = pair.first;
        }

        ++steps;
    }

    return steps;
}

int main() {
    std::map<std::string, std::pair<std::string, std::string>> nodes;
    std::vector<std::string> start_nodes;
    std::string command;
    if (readData("1.txt", command, nodes, start_nodes)) {
        std::cout << "Part1 : " << getStepCount("AAA", command, nodes) << std::endl;
        int64_t total_steps = 1;
        for (auto &start_node : start_nodes) {
            total_steps = std::lcm(getStepCount(start_node, command, nodes), total_steps);
        }
        std::cout << "Part2 : " << total_steps << std::endl;
    } else {
        std::cout << "Open file error!" << std::endl;
    }

    return 0;
}
