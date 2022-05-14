#include "core.h"

int check_presence(int &initial, std::vector<std::string> &nonterm, std::string b) {
    for (int i = 0; i < nonterm.size(); i++) {
        if (nonterm[i] == b) {
            if (b == "S")
                initial = i;
            return i;
        }
    }
    if (b == "S")
        initial = nonterm.size();
    return nonterm.size();
}

int input_rule(int &initial,
               std::vector<Rule> &eps_rules,
               std::unordered_map<std::string, std::vector<int>> &terminal_symbol_table,
               std::ifstream &fin,
               std::vector<std::string> &nonterminals,
               std::vector<Rule> &rules,
               bool spaced_rhs = true) {
    std::string left;
    std::vector<std::string> right;
    int r_size;
    if (!spaced_rhs) {
        std::string _right;
        fin >> left >> _right;
        int r_size = _right.size();
        for (char c : _right)
            right.push_back(std::string(1, c));
    } else {
        std::vector<std::string> _right;
        std::string raw_str;
        std::getline(fin, raw_str);
        //std::cout<<raw_str<<std::endl;
        std::istringstream iss(raw_str);
        iss >> left;
        std::string tmp_str;
        while (iss >> tmp_str)
            right.push_back(tmp_str);
    }
    r_size = right.size();
    if (r_size != 2 && r_size != 1) {
        std::cout << "Wrong size of rule rhs" << std::endl;
        return -1;
    } else if (r_size == 1) {
        int term_index = check_presence(initial, nonterminals, left);
        if (term_index == nonterminals.size())
            nonterminals.push_back(left);
        if (right[0] != "0")
            terminal_symbol_table[right[0]].push_back(term_index);
        else {
            terminal_symbol_table["0"].push_back(term_index);
            eps_rules.push_back({0, term_index, {0, 0}, right[0]});
        }
        rules.push_back({type_term_rhs,
                         term_index,
                         {0, 0},
                         right[0]});
    } else {
        std::string rhs_1, rhs_2;
        rhs_1 = right[0], rhs_2 = right[1];
        int term_index_lhs = check_presence(initial, nonterminals, left);
        int term_index_rhs_1, term_index_rhs_2;
        if (term_index_lhs == nonterminals.size())
            nonterminals.push_back(left);
        term_index_rhs_1 = check_presence(initial, nonterminals, rhs_1);
        if (term_index_rhs_1 == nonterminals.size())
            nonterminals.push_back(rhs_1);
        term_index_rhs_2 = check_presence(initial, nonterminals, rhs_2);
        if (term_index_rhs_2 == nonterminals.size())
            nonterminals.push_back(rhs_2);
        rules.push_back({type_non_term_rhs,
                         term_index_lhs,
                         {term_index_rhs_1, term_index_rhs_2},
                         {0, 0}});
    }
    return 0;
}

int filling_loops(int j, int P, Rule &i, std::deque<std::vector<int>> &W,
                  std::vector<std::vector<std::vector<unsigned int>>> &H1v,
                  std::vector<std::vector<std::vector<unsigned int>>> &H2v,
                  std::vector<std::vector<std::unordered_set<int>>> &H1u, 
                  std::vector<std::vector<std::unordered_set<int>>> &H2u, 
                  void (*add_value)(std::unordered_set<int> &u, 
                  std::vector<unsigned int> &v, int a, int P)) {
    W.push_back({j, i.left, j});
    add_value(H1u[i.left][j], H1v[i.left][j], j, P);
    add_value(H2u[i.left][j], H2v[i.left][j], j, P);
    return 0;
}

int filling_edge_matrices(int P, std::ifstream &fin,
                          std::unordered_map<std::string, std::vector<int>> &terminal_symbol_table,
                          std::deque<std::vector<int>> &W,
                          std::vector<std::vector<std::vector<unsigned int>>> &H1v,
                          std::vector<std::vector<std::vector<unsigned int>>> &H2v,
                          std::vector<std::vector<std::unordered_set<int>>> &H1u,
                          std::vector<std::vector<std::unordered_set<int>>> &H2u,
                          void (*add_value)(std::unordered_set<int> &u, std::vector<unsigned int> &v, int a, int P)) {
    int u1, u2 = 0;
    std::string s;
    fin >> u1 >> u2 >> s;

    for (auto j : terminal_symbol_table[s]) {
        W.push_back({u1, j, u2});
        add_value(H1u[j][u1], H1v[j][u1], u2, P);
        add_value(H2u[j][u2], H2v[j][u2], u1, P);
    }
    return 0;
}

std::vector<int> path_find(int i, int j, int nonterm, std::vector<std::vector<std::vector<std::vector<int>>>> &prev) {
    std::vector<int> u = prev[i][nonterm][j];
    if (u[0] == -1)
        return {i, j};
    std::vector<int> left = path_find(i, u[0], u[1], prev), right = path_find(u[0], j, u[2], prev), way = left;
    way.pop_back();
    for (auto k : right)
        way.push_back(k);
    return way;
}

int baseline_cfl(bool is_fast, int flag, int i2, int i3, 
                 std::vector<std::vector<std::vector<unsigned int>>> &Hiv, 
                 std::vector<std::vector<std::unordered_set<int>>> &Hiu, 
                 std::vector<std::vector<int>> side_rules, int B, int P, int V, 
                 std::vector<Rule> &rules, 
                 std::vector<std::vector<std::vector<std::vector<int>>>> &prev, 
                 std::deque<std::vector<int>> &W, 
                 std::vector<std::vector<std::vector<unsigned int>>> &H1v, 
                 std::vector<std::vector<std::vector<unsigned int>>> &H2v, 
                 std::vector<std::vector<std::unordered_set<int>>> &H1u, 
                 std::vector<std::vector<std::unordered_set<int>>> &H2u, 
                 void (*add_value)(std::unordered_set<int> &u, 
                 std::vector<unsigned int> &v, int a, int P), 
                 std::vector<int> (*create_wv)(int P, int V, 
                 std::vector<unsigned int> &v1, 
                 std::vector<unsigned int> &v2, 
                 std::unordered_set<int> &u1, 
                 std::unordered_set<int> &u2), 
                 std::unordered_set<int> (*create_wu)(int P, int V, std::vector<unsigned int> &v1, std::vector<unsigned int> &v2, std::unordered_set<int> &u1, std::unordered_set<int> &u2)) {
    for (auto i : side_rules[B]) {
        int C = rules[i].right1[flag], A = rules[i].left, i4, i5;
        std::vector<int> wv = create_wv(P, V, Hiv[C][i2], Hiv[A][i3], Hiu[C][i2], Hiu[A][i3]);
        std::unordered_set<int> wu = create_wu(P, V, Hiv[C][i2], Hiv[A][i3], Hiu[C][i2], Hiu[A][i3]);
        auto lambda_expr = [&flag, &i2, &C, &B, &W, &i4, &i5, &A, &H2u, &H2v, &H1u, &H1v, &prev, &P, &i3, &add_value](auto w) {
            for (auto d : w) {
                (flag) ? (i4 = d, i5 = i3) : (i4 = i3, i5 = d);
                add_value(H2u[A][i4], H2v[A][i4], i5, P);
                add_value(H1u[A][i5], H1v[A][i5], i4, P);
                W.push_back({i5, A, i4});
                if (prev[i5][A][i4][0] == -1)
                    prev[i5][A][i4] = {i2, (1 - flag) * C + flag * B, B * (1 - flag) + flag * C};
            }
        };
        (is_fast) ? lambda_expr(wv) : lambda_expr(wu);
    }
    return 0;
}

int output(bool is_fast, int P, int V, int initial, std::vector<std::vector<std::vector<std::vector<int>>>> &prev, 
           std::vector<std::vector<std::vector<unsigned int>>> &H1, 
           std::vector<std::vector<std::unordered_set<int>>> &H1u, 
           std::vector<int> (*create_q)(int P, std::vector<unsigned int> v)) {
    int counter = 0;
    for (int i = 0; i < V; i++) {
        std::vector<int> q = create_q(P, H1[initial][i]);
        auto lambda_expr = [&initial, &prev, &counter, &i](auto qi) {
            for (auto j : qi) {
                counter++;
                std::cout << i << " " << j << std::endl;
                std::vector<int> way = path_find(i, j, initial, prev);
                for (auto k : way)
                    std::cout << k << " ";
                std::cout << std::endl;
            }
        };
        (is_fast) ? lambda_expr(q) : lambda_expr(H1u[initial][i]);
    }
    std::cout << counter << std::endl;
    return 0;
}

int find_arg(int argc, char *argv[], const char *arg) {
    for (int i = 0; i < argc; i++)
        if (strcmp(argv[i], arg) == 0)
            return i;
    return -1;
}

int check_funcs(funcs &func) {
    int ret = 0;
    if (func.add_value == NULL) {
        std::cout << "add_value() not found" << std::endl;
        ret = -1;
    }
    if (func.difference == NULL) {
        std::cout << "difference() not found" << std::endl;
        ret = -1;
    }
    if (func.not_null == NULL) {
        std::cout << "not_null() not found" << std::endl;
        ret = -1;
    }
    if (func.create_wv == NULL) {
        std::cout << "create_wv() not found" << std::endl;
        ret = -1;
    }
    if (func.create_wu == NULL) {
        std::cout << "create_wu() not found" << std::endl;
        ret = -1;
    }
    if (func.create_q == NULL) {
        std::cout << "create_q() not found" << std::endl;
        ret = -1;
    }
    if (func.create_Hv == NULL) {
        std::cout << "create_Hv() not found" << std::endl;
        ret = -1;
    }
    if (func.create_Hu == NULL) {
        std::cout << "create_Hu() not found" << std::endl;
        ret = -1;
    }
    if (func.create_P == NULL) {
        std::cout << "create_P() not found" << std::endl;
        ret = -1;
    }
    return ret;
}