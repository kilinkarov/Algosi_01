// no_concepts
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

const int kSigma = 26;

struct Node {
  std::vector<int> to;
  bool term;
  std::vector<int> number_string;
  Node() {
    to.resize(kSigma, -1);
    term = false;
  }
};

struct Trie {
 public:
  int number_patern = 0;
  std::vector<Node> trie;
  std::vector<int> link;
  std::vector<int> compressed_link;
  std::vector<std::vector<int>> go;
  Trie() = default;

  void Add(const std::string& str) {
    if (trie.empty()) {
      trie.push_back(Node());
    }
    int v = 0;
    for (size_t i = 0; i < str.size(); ++i) {
      if (trie[v].to[str[i] - 'a'] == -1) {
        trie[v].to[str[i] - 'a'] = static_cast<int>(trie.size());
        trie.push_back(Node());
      }
      v = trie[v].to[str[i] - 'a'];
    }
    trie[v].term = true;
    trie[v].number_string.push_back(number_patern++);
  }

  std::vector<int> Find(const std::string& string) const {
    int v = 0;
    for (size_t i = 0; i < string.size(); ++i) {
      v = trie[v].to[string[i] - 'a'];
      if (v == -1) {
        return std::vector<int>();
      }
    }
    return trie[v].number_string;
  }

  void MakeGoAndLink() {
    link[0] = 0;
    compressed_link[0] = 0;
    for (int i = 0; i < kSigma; ++i) {
      if (trie[0].to[i] != -1) {
        go[0][i] = trie[0].to[i];
      } else {
        go[0][i] = 0;
      }
    }
    std::queue<int> q;
    q.push(0);
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int i = 0; i < kSigma; ++i) {
        int u = trie[v].to[i];
        if (u == -1) {
          continue;
        }
        link[u] = (v == 0 ? 0 : go[link[v]][i]);
        compressed_link[u] =
            (trie[link[u]].term ? link[u] : compressed_link[link[u]]);  //
        for (int d = 0; d < kSigma; ++d) {
          if (trie[u].to[d] != -1) {
            go[u][d] = trie[u].to[d];
          } else {
            go[u][d] = go[link[u]][d];
          }
        }
        q.push(u);
      }
    }
  }

  Trie(const std::vector<std::string>& patern) {
    for (size_t i = 0; i < patern.size(); ++i) {
      Add(patern[i]);
    }
    link.resize(trie.size());
    go.resize(trie.size());
    compressed_link.resize(trie.size());
    for (size_t i = 0; i < go.size(); ++i) {
      go[i].resize(kSigma);
    }
    MakeGoAndLink();
  }
};

bool IsPalindrom(const std::string& str) {
  int i = 0;
  int j = static_cast<int>(str.size()) - 1;
  while ((str[i] == str[j]) && (j >= 0)) {
    ++i;
    --j;
  }
  return i >= j;
}

void MakeAnswerFullWord(const Trie& trie,
                        const std::vector<std::string>& strings,
                        std::vector<std::vector<int>>& answer) {
  for (size_t i = 0; i < strings.size(); ++i) {
    std::string str = strings[i];
    std::string sub_str(str.rbegin(), str.rend());
    auto vector = trie.Find(sub_str);
    for (size_t q = 0; q < vector.size(); ++q) {
      if (vector[q] != static_cast<int>(i)) {
        answer[i].push_back(vector[q]);
      }
    }
  }
}
void MakeAnswer(const Trie& trie, const std::vector<std::string>& strings,
                std::vector<std::vector<int>>& answer) {
  MakeAnswerFullWord(trie, strings, answer);
  for (size_t i = 0; i < strings.size(); ++i) {
    for (size_t j = 1; j < strings[i].size(); ++j) {
      std::string str = strings[i];
      if (IsPalindrom(str.substr(0, j))) {
        std::string sub_str(str.rbegin(), str.rend() - j);
        auto vector = trie.Find(sub_str);
        for (size_t q = 0; q < vector.size(); ++q) {
          if (vector[q] != static_cast<int>(i)) {
            answer[vector[q]].push_back(static_cast<int>(i));  //
          }
        }
      }
    }
  }
  for (size_t i = 0; i < strings.size(); ++i) {
    for (size_t j = 1; j < strings[i].size(); ++j) {
      std::string str = strings[i];
      if (IsPalindrom(str.substr(str.size() - j))) {
        std::string sub_str(strings[i].rbegin() + j, strings[i].rend());
        auto vector = trie.Find(sub_str);
        for (size_t q = 0; q < vector.size(); ++q) {
          answer[i].push_back(vector[q]);
        }
      }
    }
  }
}

int main() {
  int n;
  std::cin >> n;
  std::vector<std::string> strings(n);
  std::vector<std::vector<int>> answer(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> strings[i];
  }
  Trie trie = Trie(strings);
  MakeAnswer(trie, strings, answer);
  int64_t size = 0;
  for (size_t i = 0; i < answer.size(); ++i) {
    std::sort(answer[i].begin(), answer[i].end());
    size += answer[i].size();
  }
  std::cout << size << '\n';
  for (size_t i = 0; i < strings.size(); ++i) {
    for (size_t j = 0; j < answer[i].size(); ++j) {
      std::cout << i + 1 << ' ' << answer[i][j] + 1 << '\n';
    }
  }
  return 0;
}
