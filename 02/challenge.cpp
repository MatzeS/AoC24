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

bool isStrictMonotonic(int a, int b, int c){
    if (a < b && b < c) return true;
    if (a > b && b > c) return true;
    return false;
}

bool isFlat(int a, int b){
    int difference = std::abs(a - b);
    return difference >= 1 && difference <= 3;
}

bool isFlat(int a, int b, int c){
    return isFlat(a, b) && isFlat(b, c);
}

bool isGood(int a, int b, int c) {
    if(not isFlat(a, b, c)) return false;
    if(a == b || b == c) return false;
    if(a < b && b > c) return false;
    if(a > b && b < c) return false;
    return true;
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
        if(line.size() == 0) continue;

        auto digits = line 
            | std::ranges::views::split(' ')
            | std::views::transform([](auto e) { //TODO implement this as own pipe operator
                return std::string_view{e};
              })
            | std::views::transform([](auto e) {
                return std::strtol(e.data(), nullptr, 10);
            });

        // auto differences = digits 
        //     | std::views::adjacent<2>
        //     | std::views::transform([](auto const& pair){
        //         auto const [left, right] = pair; // TODO transform to spread tuple paramters
        //         // TODO transorm difference, transform sign, transform gradientt
        //         return right - left;
        //     })
        //     | std::views::transform([](auto const& e){
        //         return std::abs(e);
        //     });
        // auto const flat = std::ranges::all_of(differences, [](auto const& e){
        //     return e >= 1 && e <= 3;
        // });

        // if(flat && isMonotonic(digits)){
        //     result++;
        // }


        auto windows = digits 
            | std::views::adjacent<3>
            | std::views::transform([](auto const& triple){
                auto [a, b, c] = triple;
                // std::cout << "Window: " << a << "," << b << "," << c << std::endl;
                // std::cout << "strict monotonic: " << isStrictMonotonic(a, b, c) << std::endl; 
                // std::cout << "flat: " << isFlat(a, b, c) << std::endl; 
                
                bool good = isFlat(a, b, c) && isStrictMonotonic(a, b, c);

                return not good;
            });

        auto numBad = std::ranges::count_if(windows, std::identity{});
        if(numBad > 3) continue;
        if(numBad == 0){
            result += 1;
            continue;
        }

        std::size_t firstBad = std::distance(windows.begin(), std::ranges::find(windows, true));
        auto x = std::ranges::find_last(windows, true);
        std::size_t lastBad = std::distance(windows.begin(), x.begin());
        
                //  check that all errors are one contigous window
        auto distance = lastBad - firstBad + 1;
        auto badWindowsContigous = distance == numBad;

        if(not badWindowsContigous) continue;

        // if the outlier is in the middle, than we need to be flat over outer values
        auto it = digits.begin();
        for(std::size_t i = 0; i < firstBad; i++) it++;
        auto a = *it; it++;
        auto b = *it; it++;
        auto c = *it; it++;
        auto d = *it; it++;
        auto e = *it; it++;

        // auto const flatAroundMiddleOutlier = isFlat(b, d);
        // if(not flatAroundMiddleOutlier) continue;

        // if there is excatly one error, it is eithehr
        // at the beginning or end, which is fine, because we can simply delete the first or alst
        // or if it is in the midde it is a saddle point which is not fine
        if(numBad == 1){
            if (firstBad == 0){
                // beginning, fine
                result++;
                continue;
            } else if (firstBad == std::distance(windows.begin(), windows.end()) - 1){
                // last, fine
                result++;
                continue;
            } else {
                // saddle point, bad
                continue;
            }
        }

        if(numBad == 3) continue;

        if(numBad == 2){
            if(a == d){
                // also a saddle point
                continue;
            }

            if( 
                // removing one of the bad candidates should make this flat...
                isGood(a, c, d) 
                || isGood(a, b, d) 
            ) {
                result++;
                continue;
            }

            // // all remaining look good
            // result++;
            // continue;
        }




        std::cout << "Line: " << line << std::endl;
        std::cout << "          ";
        for(auto x : windows) {
            std::cout << x << "  ";
        }
        std::cout << std::endl;


        std::cout << a << "," << b << "," << c << "," << d << "," << e << std::endl;
        std::cout << firstBad << "/" << lastBad << std::endl;



        // result += 1;
    }

    std::cout << result << std::endl;

    return EXIT_SUCCESS;
}

// between 360 and 388