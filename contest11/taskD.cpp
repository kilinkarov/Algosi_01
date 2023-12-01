#include <iostream>
#include <string>
#include <vector>
const int kSigma = 27;

int Pow(int digit, int deg) {
  int res = 1;
  while (deg > 0) {
    if ((deg % 2) == 0) {
      deg /= 2;
      digit *= digit;
    } else {
      res *= digit;
      --deg;
    }
  }
  return res;
}

void FirstIteration(const std::string& str, std::vector<int>& cnt,
                    std::vector<int>& permutation, std::vector<int>& classes) {
  int size = static_cast<int>(str.size());
  for (int i = 0; i < size; ++i) {
    ++cnt[str[i] - 'a'];
  }
  for (int i = 1; i < kSigma; ++i) {
    cnt[i] += cnt[i - 1];
  }
  for (int i = size - 1; i >= 0; --i) {
    permutation[--cnt[str[i] - 'a']] = i;
  }
  classes[permutation[0]] = 0;
  for (int i = 1; i < size; ++i) {
    classes[permutation[i]] = classes[permutation[i - 1]];
    if (str[permutation[i]] != str[permutation[i - 1]]) {
      ++classes[permutation[i]];
    }
  }
}

void Step(const std::string& str, std::vector<int>& cnt,
          std::vector<int>& permutation, std::vector<int>& classes,
          int number) {
  int size = static_cast<int>(str.size());
  std::vector<int> pn(str.size());
  pn[0] = (permutation[0] - Pow(2, number) + 2 * size) % size;
  for (int i = 0; i < size; ++i) {
    pn[i] = (permutation[i] - Pow(2, number) + 2 * size) % size;
  }
  cnt.resize(size);
  for (int i = 0; i < size; ++i) {
    cnt[i] = 0;
  }
  for (int i = 0; i < size; ++i) {
    ++cnt[classes[i]];
  }
  for (int i = 1; i < size; ++i) {
    cnt[i] += cnt[i - 1];
  }
  for (int i = size - 1; i >= 0; --i) {
    permutation[--cnt[classes[pn[i]]]] = pn[i];
  }
  std::vector<int> cn(size);
  cn[permutation[0]] = 0;
  for (int i = 1; i < size; ++i) {
    cn[permutation[i]] = cn[permutation[i - 1]];
    if (classes[permutation[i]] != classes[permutation[i - 1]]) {
      ++cn[permutation[i]];
    } else if (classes[(permutation[i] + Pow(2, number)) % size] !=
               classes[(permutation[i - 1] + Pow(2, number)) % size]) {
      ++cn[permutation[i]];
    }
  }
  classes = cn;
}

std::vector<int> SufMas(const std::string& str) {
  int size = static_cast<int>(str.size());
  std::vector<int> permutation(size);
  std::vector<int> cnt(kSigma);
  std::vector<int> classes(size);
  FirstIteration(str, cnt, permutation, classes);
  std::vector<int> pn(size);
  int number = 0;
  while (true) {
    Step(str, cnt, permutation, classes, number);
    if (Pow(2, number) >= size) {
      break;
    }
    ++number;
  }
  return permutation;
}

std::string MakeAnswer(std::string& str) {
  for (int i = 0; i < static_cast<int>(str.size()); ++i) {
    ++str[i];
  }
  int size = static_cast<int>(str.size());
  str += str;
  str += 'a';
  std::vector answer_for_double_string = SufMas(str);
  std::string answer;
  for (size_t i = 0; i < str.size(); ++i) {
    if (answer_for_double_string[i] >= (size)) {
      continue;
    }
    answer += str[answer_for_double_string[i] + size - 1];
  }
  for (int i = 0; i < size; ++i) {
    --answer[i];
  }
  return answer;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::string str;
  std::cin >> str;
  std::cout << MakeAnswer(str);
  return 0;
}
