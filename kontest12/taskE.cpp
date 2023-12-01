#include <iostream>

int Pow(int64_t digit, int64_t deg, int64_t prime) {
  int64_t res = 1;
  while (deg > 0) {
    res %= prime;
    digit %= prime;
    if (deg % 2 == 1) {
      res *= digit;
    }
    digit *= digit;
    deg /= 2;
  }
  return static_cast<int>(res % prime);
}

bool IsQuadratic(int64_t digit, int64_t prime) {
  return (Pow(digit, (prime - 1) / 2, prime) == 1);
}

int Degree(int64_t first, int64_t prime) {
  int answer = 0;
  while (Pow(first, Pow(2, answer, prime), prime) != 1) {
    ++answer;
  }
  return answer;
}

int SolForDifficultCase(int64_t digit, int64_t prime) {
  int temp = static_cast<int>(prime - 1);
  int deg_of_two = 0;
  int main_of_digit = 1;
  while ((temp % 2) == 0) {
    temp /= 2;
    ++deg_of_two;
    main_of_digit = temp;
  }
  int first = Pow(digit, main_of_digit, prime);
  int second = Pow(digit, (main_of_digit + 1) / 2, prime);
  while ((first % prime) != 1) {
    int degree = (Degree(first, prime));
    int not_quadratic_digit = rand() % prime;
    while ((not_quadratic_digit == 0) ||
           (IsQuadratic(not_quadratic_digit, prime))) {
      not_quadratic_digit = rand() % prime;
    }
    int temp_for_iter = Pow(not_quadratic_digit, main_of_digit, prime);
    first = static_cast<int>(
        (static_cast<int64_t>(first) *
         static_cast<int64_t>(
             Pow(temp_for_iter, Pow(2, deg_of_two - degree, prime), prime))) %
        prime);
    second = static_cast<int>(
        (static_cast<int64_t>(second) *
         static_cast<int64_t>(Pow(
             temp_for_iter, Pow(2, deg_of_two - degree - 1, prime), prime))) %
        prime);
  }
  return second;
}

void Sol(int digit, int prime) {
  if ((prime == 2) || (prime == 1)) {
    if (digit == 0) {
      std::cout << 0 << '\n';
      return;
    }
    std::cout << 1 << '\n';
    return;
  }
  if (!IsQuadratic(digit, prime)) {
    std::cout << "IMPOSSIBLE\n";
    return;
  }
  if ((prime % 4) == 3) {
    std::cout << Pow(digit, (prime + 1) / 4, prime) << '\n';
    return;
  }
  std::cout << SolForDifficultCase(digit, prime) << '\n';
}

int main() {
  int size;
  int prime;
  int digit;
  std::cin >> size;
  for (int i = 0; i < size; ++i) {
    std::cin >> digit >> prime;
    Sol(digit, prime);
  }
  return 0;
}
