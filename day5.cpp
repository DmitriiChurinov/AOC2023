#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>
#include <limits>

struct Info {
    int64_t destination_range_start, source_range_start, range_length;
};

int64_t GetMinSeedLocation(const std::vector<int64_t> &seeds, const std::vector<std::vector<Info>> &maps) {
    std::vector<int64_t> values(seeds);

    for (auto &value : values) {
        for (const auto &map : maps) {
            for (const auto &data : map) {
                if (data.source_range_start <= value && value < data.source_range_start + data.range_length) {
                    value = data.destination_range_start + value - data.source_range_start;
                    break;
                }
            }
        }
    }

    return *std::min_element(values.begin(), values.end());
}

int64_t GetMinSeedLocation2(const std::vector<int64_t> &seeds, const std::vector<std::vector<Info>> &maps) {
    std::vector<std::pair<int64_t, int64_t>> ranges;

    for (size_t i = 0; i + 1 < seeds.size(); i += 2) {
        ranges.emplace_back(seeds[i], seeds[i] + seeds[i + 1] - 1);
    }
    std::cout << std::endl;
    for (const auto &map : maps) {
        std::vector<std::pair<int64_t, int64_t>> new_ranges;
        while (!ranges.empty()) {
            auto [start_range, end_range] = ranges.back();
            ranges.pop_back();
            int run_layer = 0;
            for (const auto& layer : map) {
                int64_t layer_source_end = layer.source_range_start + layer.range_length;
                int64_t layer_destination_end = layer.destination_range_start + layer.range_length;
                int64_t layer_offset = layer.destination_range_start - layer.source_range_start;
                if (layer.source_range_start <= start_range
                    && start_range < layer_source_end)
                {
                    start_range = layer_offset + start_range;
                    if (layer.source_range_start <= end_range
                        && end_range < layer_source_end)
                    {
                        end_range = layer_offset + end_range;
                        new_ranges.emplace_back(start_range, end_range);
                    } else {
                        new_ranges.emplace_back(start_range, layer_destination_end - 1);
                        ranges.emplace_back(layer_source_end, end_range);
                    };
                    break;
                } else {
                    if (layer.source_range_start <= end_range
                        && end_range < layer_source_end)
                    {
                        end_range = layer_offset + end_range;
                        new_ranges.emplace_back(layer.destination_range_start, end_range);
                        end_range = layer.source_range_start - 1;
                        ranges.emplace_back(start_range, layer.source_range_start - 1);
                        break;
                    }
                    else if (start_range < layer.source_range_start && layer.source_range_start < end_range) {
                        ranges.emplace_back(start_range, layer.source_range_start - 1);
                        ranges.emplace_back(layer_source_end, end_range);
                        new_ranges.emplace_back(layer.destination_range_start,  layer_destination_end - 1);
                        break;
                    } else {
                        ++run_layer;
                    }
                }
            }
            if (run_layer == map.size()) {
                new_ranges.emplace_back(start_range, end_range);
            }
        }
        ranges = new_ranges;
    }

    int64_t min = ranges[0].first;
    for (auto &range : ranges) {
        min = std::min(min, range.first);
    }

    return min;
}

int main() {
    std::ifstream inputFile("1.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file\n";
        return 1;
    }

    std::vector<int64_t> seeds;
    std::vector<std::vector<Info>> maps;

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.find("seeds:") != std::string::npos) {
            std::istringstream iss(line.substr(line.find(":") + 1));
            int64_t value;
            while (iss >> value) {
                seeds.push_back(value);
            }
        } else if (line.find("map:") != std::string::npos) {
            std::vector<Info> map;
            while (std::getline(inputFile, line) && !line.empty()) {
                std::istringstream iss(line);
                Info data{};
                iss >> data.destination_range_start >> data.source_range_start >> data.range_length;
                map.push_back(data);
            }
            maps.push_back(map);
        }
    }

    inputFile.close();

    int64_t min_location = GetMinSeedLocation(seeds, maps);
    std::cout << "Day5 part1: " << min_location << std::endl;
    int64_t min_location2 = GetMinSeedLocation2(seeds, maps);
    std::cout << "Day5 part2: " << min_location2 << std::endl;

    return 0;
}
