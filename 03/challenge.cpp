#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <ranges>
#include <algorithm>
#include <regex>
#include <iterator>

int main(int argc, char* argv[]){
    if (argc != 2){
        return EXIT_FAILURE;
    }

    auto const inputPath = argv[1];
    auto inputFilestream = std::ifstream{inputPath};

    std::stringstream input;
    input << inputFilestream.rdbuf();

    std::string inputString = input.str();


    std::regex expression("mul\\((\\d*),(\\d*)\\)");
    auto matchesBegin = std::sregex_iterator(inputString.begin(), inputString.end(), expression);
    auto matchesEnd = std::sregex_iterator();
 
    
    uint32_t sum = 0;
    for (std::sregex_iterator it = matchesBegin; it != matchesEnd; it++){
        std::smatch match = *it;

        auto groupIterator = match.begin();
        groupIterator++;
        auto const firstNumber = std::stoi(groupIterator->str()); 
        groupIterator++;
        auto const secondNumber = std::stoi(groupIterator->str()); 
        
        sum += firstNumber * secondNumber;
    }

    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}