#include <iostream>
#include <stdexcept>

#include "mortgage.hpp"

int main() {
    char gender;
    int age;
    double salary;

    std::cout << "Genero (M/F): ";
    std::cin >> gender;
    std::cout << "Idade (18-55): ";
    std::cin >> age;
    std::cout << "Salario (0-10000): ";
    std::cin >> salary;

    try {
        std::cout << "Mortgage total: "
                  << mortgage_calc::mortgage(gender, age, salary) << "\n";
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
