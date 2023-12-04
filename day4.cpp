#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

long long int calculatePoints(const std::vector<std::vector<int>>& wins, const std::vector<std::vector<int>>& cards, long long int &totalCardCount) {
    long long int totalPoints = 0;
    totalCardCount = 0;
    std::vector<int> cardCounts(wins.size(), 1);

    for (int idx = 0; idx < wins.size(); ++idx) {
        std::vector<int> win = wins[idx];
        std::map<int, long long int> winPoints;

        for (auto &winNumber : win) {
            winPoints.insert({winNumber, 0});
        }
        long long int points = 0;
        long long int matchedCount = 0;

        std::vector<int> card = cards[idx];
        for (size_t i = 0; i < card.size(); ++i) {
            if (winPoints.find(card[i]) != winPoints.end()) {
                ++matchedCount;
                points *= 2;
                if (matchedCount == 1) {
                    points = 1;
                }
            }
        }

        totalPoints += points;
        for (int i = 0; i < matchedCount; ++i) {
            cardCounts[idx + 1 + i] += cardCounts[idx];
        }
        totalCardCount += cardCounts[idx];
    }

    return totalPoints;
}

int main() {
    std::ifstream inputFile("1.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Не удалось открыть файл." << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> wins;
    std::vector<std::vector<int>> cards;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::vector<std::string> tokens = splitString(line, '|');
        std::vector<int> cardNumbers;
        std::vector<int> winNumbers;
        std::istringstream iss(tokens[0]);
        std::istringstream iss2(tokens[1]);

        std::string str;
        iss >> str >> str;
        int number;
        while (iss >> number) {
            winNumbers.push_back(number);
        }
        while (iss2 >> number) {
            cardNumbers.push_back(number);
        }

        cards.push_back(cardNumbers);
        wins.push_back(winNumbers);
    }

    inputFile.close();

    long long int totalCount, totalPoints = calculatePoints(wins, cards, totalCount);
    std::cout << "Points: " << totalPoints << std::endl;
    std::cout << "Card's count: " << totalCount << std::endl;

    return 0;
}