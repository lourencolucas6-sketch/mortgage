#include <gtest/gtest.h>

#include <stdexcept>

#include "mortgage.hpp"

using namespace mortgage_calc;

// ---------------------------------------------------------------
// 1. Boundary Value Testing (REQ-02, REQ-03, REQ-04)
// ---------------------------------------------------------------
TEST(BoundaryValue, AgeLowerLimit) {
    EXPECT_TRUE(validateInputs('M', 18, 5000));
    EXPECT_FALSE(validateInputs('M', 17, 5000));
    EXPECT_TRUE(validateInputs('M', 19, 5000));
}

TEST(BoundaryValue, AgeUpperLimit) {
    EXPECT_TRUE(validateInputs('M', 55, 5000));
    EXPECT_FALSE(validateInputs('M', 56, 5000));
    EXPECT_TRUE(validateInputs('M', 54, 5000));
}

TEST(BoundaryValue, SalaryLowerLimit) {
    EXPECT_TRUE(validateInputs('M', 30, 0));
    EXPECT_FALSE(validateInputs('M', 30, -1));
    EXPECT_TRUE(validateInputs('M', 30, 1));
}

TEST(BoundaryValue, SalaryUpperLimit) {
    EXPECT_TRUE(validateInputs('M', 30, 10000));
    EXPECT_FALSE(validateInputs('M', 30, 10001));
    EXPECT_TRUE(validateInputs('M', 30, 9999));
}

// Fronteiras entre faixas etarias da tabela de fatores (REQ-05)
TEST(BoundaryValue, MaleFactorBoundaries) {
    EXPECT_EQ(selectFactor('M', 18), 75);
    EXPECT_EQ(selectFactor('M', 35), 75);
    EXPECT_EQ(selectFactor('M', 36), 55);
    EXPECT_EQ(selectFactor('M', 45), 55);
    EXPECT_EQ(selectFactor('M', 46), 30);
    EXPECT_EQ(selectFactor('M', 55), 30);
}

TEST(BoundaryValue, FemaleFactorBoundaries) {
    EXPECT_EQ(selectFactor('F', 18), 70);
    EXPECT_EQ(selectFactor('F', 30), 70);
    EXPECT_EQ(selectFactor('F', 31), 50);
    EXPECT_EQ(selectFactor('F', 40), 50);
    EXPECT_EQ(selectFactor('F', 41), 35);
    EXPECT_EQ(selectFactor('F', 50), 35);
}

// Exemplo do slide: homem, 18 anos, 5000 -> 375000
TEST(BoundaryValue, MaleYoungBoundary) {
    EXPECT_DOUBLE_EQ(mortgage('M', 18, 5000), 375000.0);
}

// ---------------------------------------------------------------
// 2. Robustness Testing (REQ-08: tratamento de entradas invalidas)
// ---------------------------------------------------------------
TEST(Robustness, InvalidGender) {
    EXPECT_FALSE(validateInputs('X', 30, 5000));
    EXPECT_FALSE(validateInputs('m', 30, 5000));  // minuscula nao aceita
    EXPECT_FALSE(validateInputs('\0', 30, 5000));
}

TEST(Robustness, InvalidAgeThrows) {
    EXPECT_THROW(mortgage('M', 17, 5000), std::invalid_argument);
    EXPECT_THROW(mortgage('M', 56, 5000), std::invalid_argument);
    EXPECT_THROW(mortgage('F', -5, 5000), std::invalid_argument);
}

TEST(Robustness, InvalidSalaryThrows) {
    EXPECT_THROW(mortgage('M', 30, -1), std::invalid_argument);
    EXPECT_THROW(mortgage('F', 30, 10001), std::invalid_argument);
}

TEST(Robustness, InvalidGenderThrows) {
    EXPECT_THROW(mortgage('X', 30, 5000), std::invalid_argument);
}

// Mulher 51-55: sem fator definido na tabela -> tratamento explicito
TEST(Robustness, FemaleAge51To55HasNoFactor) {
    for (int age = 51; age <= 55; ++age) {
        EXPECT_THROW(selectFactor('F', age), std::invalid_argument);
        EXPECT_THROW(mortgage('F', age, 5000), std::invalid_argument);
    }
}

// ---------------------------------------------------------------
// 3. Equivalence Partitioning (um representante por classe)
// ---------------------------------------------------------------
TEST(EquivalencePartitioning, MaleClasses) {
    EXPECT_DOUBLE_EQ(mortgage('M', 25, 1000), 75000.0);  // 18-35
    EXPECT_DOUBLE_EQ(mortgage('M', 40, 1000), 55000.0);  // 36-45
    EXPECT_DOUBLE_EQ(mortgage('M', 50, 1000), 30000.0);  // 46-55
}

TEST(EquivalencePartitioning, FemaleClasses) {
    EXPECT_DOUBLE_EQ(mortgage('F', 25, 1000), 70000.0);  // 18-30
    EXPECT_DOUBLE_EQ(mortgage('F', 35, 1000), 50000.0);  // 31-40
    EXPECT_DOUBLE_EQ(mortgage('F', 45, 1000), 35000.0);  // 41-50
}

TEST(EquivalencePartitioning, ZeroSalaryGivesZeroMortgage) {
    EXPECT_DOUBLE_EQ(mortgage('M', 30, 0), 0.0);
}

TEST(EquivalencePartitioning, CalculateMortgageFormula) {
    EXPECT_DOUBLE_EQ(calculateMortgage(2000.0, 55.0), 110000.0);
    EXPECT_DOUBLE_EQ(calculateMortgage(0.0, 75.0), 0.0);
}

// ---------------------------------------------------------------
// 4. Decision-Table Testing (genero x faixa etaria -> fator)
// ---------------------------------------------------------------
struct DecisionCase {
    char gender;
    int age;
    int expectedFactor;
};

class DecisionTable : public ::testing::TestWithParam<DecisionCase> {};

TEST_P(DecisionTable, FactorMatchesTable) {
    const auto c = GetParam();
    EXPECT_EQ(selectFactor(c.gender, c.age), c.expectedFactor);
    EXPECT_DOUBLE_EQ(mortgage(c.gender, c.age, 1000),
                     1000.0 * c.expectedFactor);
}

INSTANTIATE_TEST_SUITE_P(
    AllRules, DecisionTable,
    ::testing::Values(DecisionCase{'M', 18, 75}, DecisionCase{'M', 30, 75},
                      DecisionCase{'M', 36, 55}, DecisionCase{'M', 45, 55},
                      DecisionCase{'M', 46, 30}, DecisionCase{'M', 55, 30},
                      DecisionCase{'F', 18, 70}, DecisionCase{'F', 30, 70},
                      DecisionCase{'F', 31, 50}, DecisionCase{'F', 40, 50},
                      DecisionCase{'F', 41, 35}, DecisionCase{'F', 50, 35}));
