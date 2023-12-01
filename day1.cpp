#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>

int CheckWithDigitStrings(const std::string& str, const std::vector<std::string>& digit_string, int index) {
    int size = static_cast<int>(digit_string.size());
    for (int i = 0; i < size; ++i) {
        bool check = true;
        if (str.size() - 1 >= digit_string[i].size()) {
            for (int j = 0; j < digit_string[i].size() && check; ++j) {
                if (digit_string[i][j] != str[index + j]) {
                    check = false;
                }
            }
            if (check) {
                return i + 1;
            }
        }
    }

    return 0;
}

int CharToDigit(char letter) {
    return static_cast<int>(letter) - static_cast<int>('0');
}

bool CheckDigit(const std::string& str, const std::vector<std::string>& digit_strings, bool with_string_digit, int index, int &value) {
    if (isdigit(str[index])) {
        value = CharToDigit(str[index]);

        return true;
    }
    if (with_string_digit) {
        int digit_value = CheckWithDigitStrings(str, digit_strings, index);
        if (digit_value != 0) {
            value = digit_value;

            return true;
        }
    }

    return false;
}

int GetNumber(const std::string& str, const std::vector<std::string>& digit_strings, bool with_string_digit) {
    int first = 0, second = 0;

    int str_size = static_cast<int>(str.size());
    for (int index = 0; index < str_size; ++index) {
        if (CheckDigit(str, digit_strings, with_string_digit, index, first)) {
            break;
        }
    }
    for (int index = str_size - 1; index >= 0; --index) {
        if (CheckDigit(str, digit_strings, with_string_digit, index, second)) {
            break;
        }
    }

    return first * 10 + second;
}

int GetNumberWithReplaceString(const std::string& str, const std::vector<std::string>& digit_strings, bool with_string_digit) {
    std::string replace_str(str);
    if (with_string_digit) {
        for (int digit_string_index = 0; digit_string_index < digit_strings.size(); ++digit_string_index) {
            size_t find_index = str.find( digit_strings[digit_string_index]);
            if (find_index != std::string::npos) {
                replace_str[find_index] = static_cast<char>(digit_string_index + 1 + static_cast<int>('0'));
            }
            find_index = str.rfind( digit_strings[digit_string_index]);
            if (find_index != std::string::npos) {
                replace_str[find_index] = static_cast<char>(digit_string_index + 1 + static_cast<int>('0'));
            }
        }
    }

    int first = 0, second = 0;

    int str_size = static_cast<int>(replace_str.size());
    for (int index = 0; index < str_size; ++index) {
        if (isdigit(replace_str[index])) {
            first = CharToDigit(replace_str[index]);
            break;
        }
    }
    for (int index = str_size - 1; index >= 0; --index) {
        if (isdigit(replace_str[index])) {
            second = CharToDigit(replace_str[index]);
            break;
        }
    }

    return first * 10 + second;
}

bool SumTest(const std::vector<std::string>& test_data, const std::vector<std::string>& digit_strings, bool with_string_digit, int answer) {
    int sum = 0;
    for (auto str : test_data) {
        int string_number = GetNumber(str, digit_strings, with_string_digit);
        int string_number_with_replace = GetNumberWithReplaceString(str, digit_strings, with_string_digit);
        assert(string_number == string_number_with_replace);
        sum += string_number;
    }

    return sum == answer;
}
bool ValidateTest(const std::vector<std::string>& digit_strings) {
    const std::vector<std::string>& example1 = {"1abc2", "pqr3stu8vwx", "a1b2c3d4e5f", "treb7uchet"};
    const std::vector<std::string>& example2 = {"two1nine", "eightwothree", "abcone2threexyz", "xtwone3four", "4nineeightseven2", "zoneight234", "7pqrstsixteen"};
    return SumTest(example1, digit_strings, false, 142)
        && SumTest(example2, digit_strings, true, 281);
}

int main() {
    std::vector<std::string> digit_string = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    assert(ValidateTest(digit_string));
    std::ifstream fio;
    fio.open("1_2_test");
    int task1_1_sum(0), task1_2_sum(0);
    if (fio.is_open()) {
        std::string str;

        while (fio >> str) {
            int string_number = GetNumber(str, digit_string, false);
            int string_number_with_replace = GetNumberWithReplaceString(str, digit_string, false);
            assert(string_number == string_number_with_replace);
            task1_1_sum += string_number;
            int string_number_with_digit_string = GetNumber(str, digit_string, true);
            int string_number_with_digit_string_with_replace = GetNumberWithReplaceString(str, digit_string, true);
            assert(string_number_with_digit_string == string_number_with_digit_string_with_replace);
            task1_2_sum += string_number_with_digit_string;
        }
        fio.close();
    }


    std::cout << "1_1: " << task1_1_sum << std::endl;
    std::cout << "1_2: " << task1_2_sum << std::endl;

    return 0;
}
