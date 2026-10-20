#ifndef MORTGAGE_HPP
#define MORTGAGE_HPP

#include <string>

namespace mortgage_calc {

// Requisitos de dominio (REQ-02..REQ-04)
constexpr int    kMinAge    = 18;
constexpr int    kMaxAge    = 55;
constexpr double kMinSalary = 0.0;
constexpr double kMaxSalary = 10000.0;

// REQ-01..REQ-04: valida genero ('M'/'F'), idade [18,55] e salario [0,10000].
// Retorna true se as entradas forem validas.
bool validateInputs(char gender, int age, double salary);

// REQ-05: seleciona o fator da tabela por genero e faixa etaria.
// Lanca std::invalid_argument se a combinacao nao tiver fator definido
// (ex.: mulher com 51-55 anos).
int selectFactor(char gender, int age);

// REQ-06: Mortgage = Salary x Factor.
double calculateMortgage(double salary, double factor);

// REQ-07: funcao principal. Valida, seleciona o fator e calcula.
// Lanca std::invalid_argument em caso de entrada invalida.
double mortgage(char gender, int age, double salary);

}  // namespace mortgage_calc

#endif  // MORTGAGE_HPP
