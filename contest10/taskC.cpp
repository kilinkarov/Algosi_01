// no_concepts
#include <cstring>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

struct Node {
  static const int kSigma = 26;
  int to[kSigma];
  bool term;
  int number_string = -1;
  int link;
  int compressed_link;
  int go[kSigma];
  Node() {
    memset(to, -1, sizeof(to));
    memset(go, 0, sizeof(to));
    link = 0;
    compressed_link = 0;
    term = false;
  }
};

struct Trie {
 public:
  static const int kSigma = 26;
  int number_patern = 0;
  std::vector<Node> trie;
  std::string str;
  std::vector<int> vector_suffix;
  std::vector<size_t> uniq_patern_number;
  std::vector<std::vector<size_t>> uniq_patern_first;
  std::unordered_map<std::string, size_t> set_patern;
  std::vector<std::vector<size_t>> start_patern;
  std::vector<std::string> patern;

  Trie() = default;

  void Add(const std::string& str) {
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
    trie[0].link = 0;
    trie[0].compressed_link = 0;
    for (int i = 0; i < kSigma; ++i) {
      if (trie[0].to[i] != -1) {
        trie[0].go[i] = trie[0].to[i];
      } else {
        trie[0].go[i] = 0;
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
        trie[u].link = (v == 0 ? 0 : trie[trie[v].link].go[i]);
        trie[u].compressed_link =
            (trie[trie[u].link].term ? trie[u].link
                                     : trie[trie[u].link].compressed_link);  //
        for (int d = 0; d < kSigma; ++d) {
          if (trie[u].to[d] != -1) {
            trie[u].go[d] = trie[u].to[d];
          } else {
            trie[u].go[d] = trie[trie[u].link].go[d];
          }
        }
        q.push(u);
      }
    }
  }

  void MakeStartPatern() {
    for (size_t i = 0; i < str.size(); ++i) {
      int index = trie[vector_suffix[i]].number_string;
      if (trie[vector_suffix[i]].term) {
        start_patern[uniq_patern_number[index]].push_back(
            i - patern[index].size() + 1);
      }
      int v = trie[vector_suffix[i]].compressed_link;
      do {
        index = trie[v].number_string;  //
        if (index >= 0) {
          start_patern[uniq_patern_number[index]].push_back(
              i - patern[index].size() + 1);
        }
        v = trie[v].compressed_link;
      } while (v != 0);
    }
  }

  Trie(const std::vector<std::string>& other_patern,
       const std::string& other_str) {
    int size = 0;
    for (size_t i = 0; i < other_patern.size(); ++i) {
      if (set_patern.find(other_patern[i]) != set_patern.end()) {
        uniq_patern_first[set_patern[other_patern[i]]].push_back(i);
      } else {
        set_patern[other_patern[i]] = size;
        patern.push_back(other_patern[i]);
        uniq_patern_number.push_back(i);
        uniq_patern_first.resize(++size);
      }
    }

    trie.push_back(Node());
    for (size_t i = 0; i < patern.size(); ++i) {
      Add(patern[i]);
    }
    MakeGoAndLink();

    str = other_str;
    start_patern.resize(other_patern.size());
    vector_suffix.resize(other_str.size());
    vector_suffix[0] = trie[vector_suffix[0]].go[str[0] - 'a'];
    for (size_t i = 1; i < str.size(); ++i) {
      vector_suffix[i] = trie[vector_suffix[i - 1]].go[str[i] - 'a'];
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
  Trie aho = Trie(patern, str);
  for (size_t i = 0; i < aho.start_patern.size(); ++i) {
    std::cout << aho.start_patern[i].size() << ' ';
    for (size_t j = 0; j < aho.start_patern[i].size(); ++j) {
      std::cout << aho.start_patern[i][j] + 1 << ' ';
    }
    std::cout << '\n';
  }
  return 0;
}
