#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

struct Coordinate {
    int x;
    int y;

    Coordinate() = default;

    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Coordinate& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
};

bool isValidCoordinate(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

bool isValidNumber(const std::vector<std::string>& engineLayout, int x, int y, int rows, int cols, std::set<Coordinate>& stars) {
    int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    bool result = false;
    const int DIRECTIONS = 8;
    for (int i = 0; i < DIRECTIONS; ++i) {
        int newX = x + dx[i];
        int newY = y + dy[i];

        if (isValidCoordinate(newX, newY, rows, cols)
            && engineLayout[newX][newY] != '.'
            && !isdigit(engineLayout[newX][newY])) {
            result = true;
            if (engineLayout[newX][newY] == '*') {
                Coordinate starCoord = {newX, newY};
                stars.insert(starCoord);
            }
        }
    }

    return result;
}

int sumAdjacentNumbers(const std::vector<std::string>& engineLayout, int &x, int &y, std::map<Coordinate, std::vector<int>>& starDigits) {
    int rows = static_cast<int>(engineLayout.size());
    int cols = static_cast<int>(engineLayout[0].size());

    int num = 0;
    bool isValid = false;
    std::set<Coordinate> stars;
    const int BASE = 10;
    while (isValidCoordinate(x, y, rows, cols)) {
        if (isdigit(engineLayout[x][y])) {
            num = num * BASE + (engineLayout[x][y] - '0');
        } else {
            break;
        }
        if (isValidNumber(engineLayout, x, y, rows, cols, stars)) {
            isValid = true;
        }
        ++y;
    }

    if (!isValid) {
        num = 0;
    } else {
        for (auto star: stars) {
            starDigits[star].push_back(num);
        }
    }

    return num;
}

int main() {
    std::ifstream inputFile("1.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
        return 1;
    }

    std::vector<std::string> engineLayout;
    std::string line;

    while (std::getline(inputFile, line)) {
        engineLayout.push_back(line);
    }

    inputFile.close();

    std::map<Coordinate, std::vector<int>> starDigits;
    int totalSum = 0;

    for (int i = 0; i < engineLayout.size(); ++i) {
        for (int j = 0; j < engineLayout[i].size(); ++j) {
            if (isdigit(engineLayout[i][j])) {
                totalSum += sumAdjacentNumbers(engineLayout, i, j, starDigits);
            }
        }
    }
    std::cout << "The sum of adjacent numbers in the engine layout is: " << totalSum << std::endl;

    long long int productSum = 0;

    for (const auto& pair : starDigits) {
        if (pair.second.size() == 2) {
            productSum += pair.second[0] * pair.second[1];
        }
    }

    std::cout << "Product of pairs surrounding stars: " << productSum << std::endl;

    return 0;
}