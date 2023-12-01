#include <algorithm>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

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

int LowerBound(const std::vector<int>& vector, const int& value) {
  int lenght = 0;
  int right = static_cast<int>(vector.size()) - 1;
  int mid;
  while (lenght < right) {
    mid = (lenght + right) / 2;
    if (vector[mid] < value) {
      lenght = mid + 1;
    } else {
      right = mid;
    }
  }
  return lenght;
}

void Sol(int prime, int64_t digit_in_deg, int64_t answer_for_sol) {
  int64_t constant = std::sqrt(prime);
  std::vector<std::pair<int, int>> vector_for_p_pair;
  std::vector<int> vector_for_p;
  for (int64_t i = 1; i <= (prime / constant) + 1; ++i) {
    int answer_for_i = Pow(digit_in_deg, constant * i, prime);
    vector_for_p_pair.push_back(std::pair<int, int>(answer_for_i, i));
    vector_for_p.push_back(answer_for_i);
  }
  std::sort(vector_for_p_pair.begin(), vector_for_p_pair.end());
  std::sort(vector_for_p.begin(), vector_for_p.end());
  std::vector<int> vector_for_q(constant);
  int min_index_p = static_cast<int>(prime / constant + 3);
  int min_index_q = 0;
  for (int i = 0; i < constant; ++i) {
    vector_for_q[i] = (answer_for_sol * Pow(digit_in_deg, i, prime)) % prime;
    int index = LowerBound(vector_for_p, vector_for_q[i]);
    if (vector_for_p_pair[index].first == vector_for_q[i]) {
      if (min_index_p > vector_for_p_pair[index].second) {
        min_index_p = vector_for_p_pair[index].second;
        min_index_q = i;
      } else if ((min_index_p == vector_for_p_pair[index].second) &&
                 (min_index_q < i)) {
        min_index_q = i;
      }
    }
  }
  if (min_index_p != prime / constant + 3) {
    std::cout << constant * min_index_p - min_index_q << '\n';
    return;
  }
  std::cout << "no solution\n";
}

int main() {
  int prime;
  int64_t digit_in_deg;
  int64_t answer_for_sol;
  while (std::cin >> prime) {
    std::cin >> digit_in_deg >> answer_for_sol;
    if (answer_for_sol == 1) {
      std::cout << 0 << '\n';
      continue;
    }
    Sol(prime, digit_in_deg, answer_for_sol);
  }
  return 0;
}
