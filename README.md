# Projeto 1 — Mortgage: Design de Software, GitHub Workflow e Google Test

Calcula o valor total de financiamento (mortgage) a partir de gênero, idade e salário:

    Mortgage = Salary × Factor

## Regras

- Gênero: `M` ou `F`
- Idade: 18 a 55
- Salário: 0 a 10.000

| Gênero | Faixa etária | Fator |
|--------|--------------|-------|
| M | 18–35 | 75 |
| M | 36–45 | 55 |
| M | 46–55 | 30 |
| F | 18–30 | 70 |
| F | 31–40 | 50 |
| F | 41–50 | 35 |

**Decisão de projeto:** mulheres de 51 a 55 anos não têm fator na tabela do enunciado.
Nesse caso o sistema lança `std::invalid_argument` (comportamento documentado e testado).

## Estrutura

    include/    mortgage.hpp
    src/        mortgage.cpp, main.cpp
    tests/      test_mortgage.cpp (Google Test)
    docs/       documentação e relatório
    .github/workflows/ci.yml   pipeline de CI

## Como compilar e testar

    cmake -S . -B build
    cmake --build build
    ctest --test-dir build --output-on-failure
    ./build/mortgage_app

## Fluxo GitHub

Estratégia de branches: `main ← develop ← feature/* ← test/*`

1. Abrir uma Issue para cada requisito
2. Criar branch `feature/<nome>` a partir de `develop`
3. Commits pequenos referenciando a Issue (`#n`)
4. Pull Request com Code Review obrigatório
5. CI executa build + Google Test; só faz merge se todos os testes passarem
6. Release com tag (`v1.0.0`)
