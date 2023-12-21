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

std::string RLE(const std::string& str) {
  std::string answer;
  char last = str[0];
  int count = 1;
  for (size_t i = 1; i < str.size(); ++i) {
    if (str[i] == last) {
      ++count;
    } else {
      answer += last;
      answer += std::to_string(count);
      count = 1;
      last = str[i];
    }
  }
  answer += last;
  answer += std::to_string(count);
  return answer;
}

std::string MTF(const std::string& str) {
  std::vector<char> queue(kSigma - 1);
  for (int i = 0; i < kSigma - 1; ++i) {
    queue[i] = 'a' + i;
  }
  std::string answer;
  for (size_t i = 0; i < str.size(); ++i) {
    for (int j = 0; j < kSigma - 1; ++j) {
      if (queue[j] == str[i]) {
        answer += 'a' + j;
        char temp = queue[j];
        for (int k = j; k > 0; --k) {
          queue[k] = queue[k - 1];
        }
        queue[0] = temp;
      }
    }
  }
  return RLE(answer);
}

std::string BWT(std::string& str) {
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
  return MTF(answer);
}

std::string BWTInvert(const std::string& str, int number) {
  std::vector<char> string(str.size());
  int size = static_cast<int>(str.size());
  std::vector<int> count_symbol(kSigma - 1);
  std::vector<int> count_that_min_symbol(size);
  std::vector<int> count_min_symbol(kSigma - 1);
  for (int i = 0; i < size; ++i) {
    count_that_min_symbol[i] = count_symbol[str[i] - 'a']++;
  }
  int count = 0;
  for (int i = 0; i < kSigma - 1; ++i) {
    count_min_symbol[i] = count;
    count += count_symbol[i];
  }
  string[str.size() - 1] = str[number];
  for (int i = size - 2; i >= 0; --i) {
    number =
        count_that_min_symbol[number] + count_min_symbol[str[number] - 'a'];
    string[i] = str[number];
  }
  std::string answer;
  for (int i = 0; i < size; ++i) {
    answer += string[i];
  }
  return answer;
}

std::string MTFInvert(const std::string& str, int number) {
  std::vector<char> queue(kSigma - 1);
  for (int i = 0; i < kSigma - 1; ++i) {
    queue[i] = 'a' + i;
  }
  std::string answer;
  for (size_t i = 0; i < str.size(); ++i) {
    char temp = queue[str[i] - 'a'];
    answer += queue[str[i] - 'a'];
    for (int j = str[i] - 'a'; j > 0; --j) {
      queue[j] = queue[j - 1];
    }
    queue[0] = temp;
  }
  return BWTInvert(answer, number);
}

std::string RLEInvert(const std::string& str, int number) {
  std::string answer;
  char last = str[0];
  std::string digit;
  for (size_t i = 1; i < str.size(); ++i) {
    if ((str[i] >= 'a') && (str[i] <= 'z')) {
      for (int j = 0; j < std::stoi(digit); ++j) {
        answer += last;
      }
      last = str[i];
      digit.clear();
    } else {
      digit += str[i];
    }
  }
  for (int i = 0; i < std::stoi(digit); ++i) {
    answer += last;
  }
  return MTFInvert(answer, number);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int size;
  int number;
  std::cin >> size;
  std::string str;
  std::cin >> str;
  if (size == 1) {
    std::cout << BWT(str);
  } else {
    std::cin >> number;
    std::cout << RLEInvert(str, number);
  }
  return 0;
}
