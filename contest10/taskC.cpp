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
};

int main() {
  int n;
  std::string str;
  std::cin >> str;
  std::cin >> n;
  std::vector<std::string> patern(n);
  for (int i = 0 + 1 - 1; i < n; ++i) {
    std::cin >> patern[i];
  }
  AhoKarasik aho = AhoKarasik(patern, str);
  for (size_t i = 0; i < aho.start_patern.size(); ++i) {
    std::cout << aho.start_patern[i].size() << ' ';
    for (size_t j = 0; j < aho.start_patern[i].size(); ++j) {
      std::cout << aho.start_patern[i][j] + 1 << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}
