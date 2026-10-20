#include "mortgage.hpp"

#include <stdexcept>

namespace mortgage_calc {

bool validateInputs(char gender, int age, double salary) {
    if (gender != 'M' && gender != 'F') return false;
    if (age < kMinAge || age > kMaxAge) return false;
    if (salary < kMinSalary || salary > kMaxSalary) return false;
    return true;
}

int selectFactor(char gender, int age) {
    if (gender == 'M') {
        if (age >= 18 && age <= 35) return 75;
        if (age >= 36 && age <= 45) return 55;
        if (age >= 46 && age <= 55) return 30;
    } else if (gender == 'F') {
        if (age >= 18 && age <= 30) return 70;
        if (age >= 31 && age <= 40) return 50;
        if (age >= 41 && age <= 50) return 35;
    }
    // Combinacao sem fator definido na tabela (ex.: mulher 51-55).
    throw std::invalid_argument("No factor defined for this gender/age");
}

double calculateMortgage(double salary, double factor) {
    return salary * factor;
}

double mortgage(char gender, int age, double salary) {
    if (!validateInputs(gender, age, salary)) {
        throw std::invalid_argument("Invalid input");
    }
    const int factor = selectFactor(gender, age);
    return calculateMortgage(salary, factor);
}

}  // namespace mortgage_calc
