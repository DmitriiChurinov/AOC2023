#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>

std::string replaceChars(std::string hand, bool part2) {
    std::string replacedHand = hand;
    std::replace(replacedHand.begin(), replacedHand.end(), 'T', char('9' + 1));

    char replaceJ = (part2) ? char('2' - 1) : char('9' + 2);
    std::replace(replacedHand.begin(), replacedHand.end(), 'J', replaceJ);

    std::replace(replacedHand.begin(), replacedHand.end(), 'Q', char('9' + 3));
    std::replace(replacedHand.begin(), replacedHand.end(), 'K', char('9' + 4));
    std::replace(replacedHand.begin(), replacedHand.end(), 'A', char('9' + 5));

    return replacedHand;
}

std::pair<int, std::string> strength(const std::string& hand, bool part2) {
    std::string replacedHand = replaceChars(hand, part2);
    std::map<char, int> C;
    for (char c : replacedHand) {
        C[c]++;
    }

    if (part2) {
        char target = C.begin()->first;
        for (const auto& kvp : C) {
            if (kvp.first != '1') {
                if (kvp.second > C[target] || target == '1') {
                    target = kvp.first;
                }
            }
        }
        assert(target != '1' || (C.size() == 1 && C.find('1') != C.end()));
        if (C.find('1') != C.end() && target != '1') {
            C[target] += C['1'];
            C.erase('1');
        }
        assert(C.find('1') == C.end() || (C.size() == 1 && C.find('1') != C.end()));
    }

    std::vector<int> sortedValues;
    for (const auto& kvp : C) {
        sortedValues.push_back(kvp.second);
    }
    std::sort(sortedValues.begin(), sortedValues.end());

    if (sortedValues == std::vector<int>({ 5 })) {
        return std::make_pair(10, replacedHand);
    } else if (sortedValues == std::vector<int>({ 1, 4 })) {
        return std::make_pair(9, replacedHand);
    } else if (sortedValues == std::vector<int>({ 2, 3 })) {
        return std::make_pair(8, replacedHand);
    } else if (sortedValues == std::vector<int>({ 1, 1, 3 })) {
        return std::make_pair(7, replacedHand);
    } else if (sortedValues == std::vector<int>({ 1, 2, 2 })) {
        return std::make_pair(6, replacedHand);
    } else if (sortedValues == std::vector<int>({ 1, 1, 1, 2 })) {
        return std::make_pair(5, replacedHand);
    } else if (sortedValues == std::vector<int>({ 1, 1, 1, 1, 1 })) {
        return std::make_pair(4, replacedHand);
    } else {
        assert(false);
        return std::make_pair(-1, hand);
    }
}

int main(int argc, char *argv[]) {
    std::ifstream file("1.txt");
    if (!file.is_open()) {
        std::cerr << "Unable to open file\n";
        return 1;
    }

    std::vector<std::pair<std::string, std::string>> H;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string hand, bid;
        iss >> hand >> bid;
        H.emplace_back(hand, bid);
    }
    file.close();

    for (bool part2 : {false, true}) {
        std::sort(H.begin(), H.end(), [part2](const auto& hb1, const auto& hb2) {
            return strength(hb1.first, part2) < strength(hb2.first, part2);
        });

        int ans = 0;
        for (size_t i = 0; i < H.size(); ++i) {
            ans += (i + 1) * std::stoi(H[i].second);
        }
        std::cout << ans << std::endl;
    }

    return 0;
}