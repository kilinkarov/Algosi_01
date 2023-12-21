#include <complex>
#include <iostream>
#include <numbers>
#include <vector>

std::vector<std::complex<double>> DFT(
    const std::vector<std::complex<double>>& vector, int size,
    double main_corner) {
  std::vector<std::complex<double>> vector_even;
  std::vector<std::complex<double>> vector_odd;
  if (size == 1) {
    return std::vector<std::complex<double>>{vector[0]};
  }

  for (size_t i = 0; i < vector.size(); ++i) {
    if ((i % 2) == 0) {
      vector_even.push_back(vector[i]);
    } else {
      vector_odd.push_back(vector[i]);
    }
  }
  auto vector_answer_one = DFT(vector_even, size / 2, 2 * main_corner);
  auto vector_answer_two = DFT(vector_odd, size / 2, 2 * main_corner);
  std::vector<std::complex<double>> vector_answer(size);
  for (int i = 0; i < size; ++i) {
    std::complex<double> main_sqrt(std::cos(main_corner * i),
                                   std::sin(main_corner * i));
    vector_answer[i] = vector_answer_one[i % (size / 2)] +
                       (main_sqrt * vector_answer_two[i % (size / 2)]);
  }
  return vector_answer;
}

std::vector<int> FFT(std::vector<int>& vector_index1,
                     std::vector<int>& vector_index2) {
  int size = 1;
  if ((vector_index1.empty()) || (vector_index2.empty())) {
    return std::vector<int>();
  }
  if ((vector_index1.size() + vector_index2.size()) == 2) {
    return std::vector<int>{vector_index1[0] * vector_index2[0]};
  }
  while (static_cast<size_t>(size) <
         (vector_index1.size() + vector_index2.size()) - 1) {
    size *= 2;
  }
  std::vector<std::complex<double>> vector_index_one(vector_index1.size());
  std::vector<std::complex<double>> vector_index_two(vector_index2.size());
  for (size_t i = 0; i < vector_index1.size(); ++i) {
    vector_index_one[i] = vector_index1[i];
  }
  for (size_t i = 0; i < vector_index2.size(); ++i) {
    vector_index_two[i] = vector_index2[i];
  }
  vector_index_one.resize(size);
  vector_index_two.resize(size);
  auto vector_digit1 =
      DFT(vector_index_one, size, (2 * std::numbers::pi) / size);
  auto vector_digit2 =
      DFT(vector_index_two, size, (2 * std::numbers::pi) / size);
  std::vector<std::complex<double>> vector_digit(size);
  for (int i = 0; i < size; ++i) {
    vector_digit[i] = vector_digit1[i] * vector_digit2[i];
  }
  auto vector_potencial_answer =
      DFT(vector_digit, size, -(2 * std::numbers::pi) / size);
  std::vector<int> answer(size);
  for (int i = 0; i < size; ++i) {
    answer[i] = std::round(vector_potencial_answer[i].real()) / size;
  }
  return answer;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int size1;
  std::cin >> size1;
  std::vector<int> vector_index_one(size1 + 1);
  for (int i = size1; i >= 0; --i) {
    std::cin >> vector_index_one[i];
  }
  int size2;
  std::cin >> size2;
  std::vector<int> vector_index_two(size2 + 1);
  for (int i = size2; i >= 0; --i) {
    std::cin >> vector_index_two[i];
  }
  std::cout << size1 + size2 << ' ';
  auto answer = FFT(vector_index_one, vector_index_two);
  bool flag_is_fix_null = true;
  for (int i = static_cast<int>(answer.size()) - 1; i >= 0; --i) {
    if (flag_is_fix_null && (answer[i] == 0)) {
      continue;
    }
    std::cout << answer[i] << ' ';
    flag_is_fix_null = false;
  }
  return 0;
}
