// no_concepts
#include <cstring>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

const int kSigma = 26;

struct Node {
  int to[kSigma];
  bool term;
  int number_string = -1;
  Node() {
    memset(to, -1, sizeof(to));
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
    trie[v].number_string = number_patern++;
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

struct AhoKarasik {
 public:
  Trie trie;
  std::string str;
  std::vector<int> vector_suffix;
  std::vector<int> uniq_patern_number;
  std::vector<std::vector<int>> uniq_patern_first;
  std::unordered_map<std::string, int> set_patern;
  std::vector<std::vector<int>> start_patern;
  std::vector<std::string> patern;
  AhoKarasik() = default;

  void MakeStartPatern() {
    for (int i = 0; i < static_cast<int>(str.size()); ++i) {
      int index = trie.trie[vector_suffix[i]].number_string;
      if (trie.trie[vector_suffix[i]].term) {
        start_patern[uniq_patern_number[index]].push_back(
            i - static_cast<int>(patern[index].size()) + 1);
      }
      int v = trie.compressed_link[vector_suffix[i]];
      do {
        index = trie.trie[v].number_string;  //
        if (index >= 0) {
          start_patern[uniq_patern_number[index]].push_back(
              i - static_cast<int>(patern[index].size()) + 1);
        }
        v = trie.compressed_link[v];
      } while (v != 0);
    }
  }

  AhoKarasik(const std::vector<std::string>& other_patern,
             const std::string& other_str) {
    int size = 0;
    for (int i = 0; i < static_cast<int>(other_patern.size()); ++i) {
      if (set_patern.find(other_patern[i]) != set_patern.end()) {
        uniq_patern_first[set_patern[other_patern[i]]].push_back(i);
      } else {
        set_patern[other_patern[i]] = size;
        patern.push_back(other_patern[i]);
        uniq_patern_number.push_back(i);
        uniq_patern_first.resize(++size);
      }
    }
    trie = Trie(patern);
    str = other_str;
    start_patern.resize(other_patern.size());
    vector_suffix.resize(other_str.size());
    vector_suffix[0] = trie.go[vector_suffix[0]][str[0] - 'a'];
    for (size_t i = 1; i < str.size(); ++i) {
      vector_suffix[i] = trie.go[vector_suffix[i - 1]][str[i] - 'a'];
    }
    MakeStartPatern();
    for (size_t i = 0; i < uniq_patern_number.size(); ++i) {
      for (size_t j = 0; j < uniq_patern_first[i].size(); ++j) {
        start_patern[uniq_patern_first[i][j]] =
            start_patern[uniq_patern_number[i]];
      }
    }
  }
  void MakeNewAho(const std::string& other_str) {
    str = other_str;
    vector_suffix.clear();
    for (size_t i = 0; i < start_patern.size(); ++i) {
      start_patern[i].clear();
    }
    vector_suffix.resize(other_str.size());
    vector_suffix[0] = trie.go[vector_suffix[0]][str[0] - 'a'];
    for (size_t i = 1; i < str.size(); ++i) {
      vector_suffix[i] = trie.go[vector_suffix[i - 1]][str[i] - 'a'];
    }
    MakeStartPatern();
    for (size_t i = 0; i < uniq_patern_number.size(); ++i) {
      for (size_t j = 0; j < uniq_patern_first[i].size(); ++j) {
        start_patern[uniq_patern_first[i][j]] =
            start_patern[uniq_patern_number[i]];
      }
    }
  }
};

void MakeAnswerForString(const std::vector<std::string>& trie,
                         const std::vector<std::string>& str,
                         std::vector<std::vector<int>>& answer, int a, int b) {
  AhoKarasik aho = AhoKarasik(trie, str[0]);
  for (size_t i = 0; i < str.size(); ++i) {
    aho.MakeNewAho(str[i]);
    for (size_t j = 0; j < std::min(trie.size(), i + 1); ++j) {
      std::vector<int> vec = aho.start_patern[j];
      for (size_t k = 0; k < vec.size(); ++k) {
        if ((vec[k] < (static_cast<int>(str[i].size()) - b + 1)) &&
            ((i - j) < (str.size() - a + 1))) {
          answer[i - j][vec[k]] += 1;
        }
      }
    }
  }
}

void MakeAnswerForColumn(const std::vector<std::string>& trie,
                         const std::vector<std::string>& str,
                         std::vector<std::vector<int>>& answer, int a, int b) {
  AhoKarasik aho = AhoKarasik(trie, str[0]);
  for (size_t i = 0; i < str.size(); ++i) {
    aho.MakeNewAho(str[i]);
    for (size_t j = 0; j < std::min(trie.size(), i + 1); ++j) {
      std::vector<int> vec = aho.start_patern[j];
      for (size_t k = 0; k < vec.size(); ++k) {
        if ((vec[k] < (static_cast<int>(str[i].size()) - a + 1)) &&
            ((i - j) < (str.size() - b + 1))) {
          answer[vec[k]][i - j] += 1;
        }
      }
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m;
  std::cin >> n >> m;
  std::vector<std::string> moskov_region_string(n);
  std::vector<std::string> moskov_region_column(m);
  std::string str;
  for (int i = 0; i < n; ++i) {
    std::cin >> str;
    for (int j = 0; j < m; ++j) {
      moskov_region_string[i] += str[j];
      moskov_region_column[j] += str[j];
    }
  }
  int a, b;
  std::cin >> a >> b;
  std::vector<std::string> mipt_string(a);
  std::vector<std::string> mipt_column(b);
  for (int i = 0; i < a; ++i) {
    std::cin >> str;
    mipt_string[i] = str;
    for (int j = 0; j < b; ++j) {
      mipt_column[j] += str[j];
    }
  }
  std::vector<std::vector<int>> answer(n - a + 1);  //
  for (size_t i = 0; i < answer.size(); ++i) {
    answer[i].resize(m - b + 1);
  }
  AhoKarasik trie_string = AhoKarasik(mipt_string, moskov_region_string[0]);
  Trie trie_column = Trie(mipt_column);
  MakeAnswerForString(mipt_string, moskov_region_string, answer, a, b);
  MakeAnswerForColumn(mipt_column, moskov_region_column, answer, a, b);
  int answer_digit = 0;
  for (size_t i = 0; i < answer.size(); ++i) {
    for (size_t j = 0; j < answer[i].size(); ++j) {
      if (answer[i][j] >= a + b - 2) {
        ++answer_digit;
      }
    }
  }
  std::cout << answer_digit;
  return 0;
}
