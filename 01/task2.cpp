#include <cstdlib>
#include <exception>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <string>
#include <string_view>
#include <algorithm>
#include <vector>
#include <utility>
#include <cstdlib>

// Based on
// https://stackoverflow.com/questions/2291802/is-there-a-c-iterator-that-can-iterate-over-a-file-line-by-line
class Line : public std::string {
  friend std::istream &operator>>(std::istream &stream, Line &line) {
    return std::getline(stream, line);
  }
};

using LineIterator = std::istream_iterator<Line>;

class Lines {
private:
    std::ifstream stream;

public:
    Lines(std::ifstream stream) : stream{std::move(stream)} {}

    LineIterator begin() {
        return LineIterator{stream};
    }

    LineIterator end() {
        return LineIterator{};
    }
};

std::pair<uint32_t, uint32_t> parseLine(std::string_view input){
    char* end;
    uint32_t first = std::strtol(input.data(), &end, 10);
    uint32_t second = std::strtol(end, nullptr, 10);
    return {first, second};
}

void throwingMain(int argc, char* argv[]) {
    if(argc != 2){
        throw std::invalid_argument("Expecting one argument as input file.");
    }

    auto const inputArgument = argv[1];
    auto const inputPath = std::filesystem::path{inputArgument};
    auto inputFile = std::ifstream{inputPath};

    auto numberPairs = Lines{std::move(inputFile)} | std::views::transform(parseLine);
    std::vector<uint32_t> left;
    std::vector<uint32_t> right;
    for(auto const [l, r] : numberPairs) {
        left.push_back(l);
        right.push_back(r);
    }

    std::sort(std::begin(left), std::end(left));
    std::sort(std::begin(right), std::end(right));

    uint32_t sum = 0;
    std::size_t rightIndex = 0;
    for(uint32_t l : left){
        while(right[rightIndex] < l && rightIndex < right.size()) {
            rightIndex++;            
        }

        std::size_t countRight = 0;
        while(right[rightIndex] == l && rightIndex < right.size()) {
            rightIndex++;            
            countRight++;
        }

        sum += l * countRight;

        // we are double counting elements in the left list, so we reset the right index to double count them on thte right as well.
        rightIndex -= countRight; 
    }

    std::cout << sum << std::endl;
}

int main(int argc, char* argv[]){
    try{
        throwingMain(argc, argv);
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
    return EXIT_SUCCESS;
}
