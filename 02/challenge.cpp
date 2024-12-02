#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <ranges>
#include <algorithm>

enum class Gradient {
    Increasing,
    Decreasing
};

Gradient derive(int left, int right) {
    assert(left != right);
    return left <= right ? Gradient::Increasing : Gradient::Decreasing;
}

Gradient getGradient(std::ranges::input_range auto&& digits){
    auto it = std::ranges::begin(digits);    
    auto const left = *it;
    while(*it == left && it != std::ranges::end(digits)) it++;    
    auto const right = *it;
    return derive(left, right);
}


bool isMonotonic(std::ranges::input_range auto&& digits) {
    auto const gradient = getGradient(digits);

    auto gradients = digits 
        | std::views::adjacent<2>
        | std::views::transform([](auto const& pair){
            auto const [left, right] = pair; // TODO transform to spread tuple paramters
            // TODO transorm difference, transform sign, transform gradientt
            return derive(left, right);
        })
        | std::views::adjacent<2>;

    return std::ranges::all_of(gradients, [](auto const& pair){
        auto const [left, right] = pair; // TODO inelegant, is there a better ?
        return left == right;
    });
}

int main(int argc, char* argv[]){
    if (argc != 2){
        return EXIT_FAILURE;
    }

    auto const inputPath = argv[1];
    auto inputFilestream = std::ifstream{inputPath};

    std::stringstream input;
    input << inputFilestream.rdbuf();

    std::size_t result = 0;

    auto lines = input.view() 
        | std::ranges::views::split('\n')
        | std::views::transform([](auto e) { //TODO implement this as own pipe operator
            return std::string_view{e};
          });
    for(auto const line : lines){
        // std::cout << "Line: " << line << std::endl;
        if(line.size() == 0) continue;

        auto digits = line 
            | std::ranges::views::split(' ')
            | std::views::transform([](auto e) { //TODO implement this as own pipe operator
                return std::string_view{e};
              })
            | std::views::transform([](auto e) {
                return std::strtol(e.data(), nullptr, 10);
            });

        auto differences = digits 
            | std::views::adjacent<2>
            | std::views::transform([](auto const& pair){
                auto const [left, right] = pair; // TODO transform to spread tuple paramters
                // TODO transorm difference, transform sign, transform gradientt
                return right - left;
            })
            | std::views::transform([](auto const& e){
                return std::abs(e);
            });
        auto const flat = std::ranges::all_of(differences, [](auto const& e){
            return e >= 1 && e <= 3;
        });

        if(flat && isMonotonic(digits)){
            result++;
        }
    }

    std::cout << result << std::endl;

    return EXIT_SUCCESS;
}
