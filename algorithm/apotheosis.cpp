#include "apotheosis.hpp"

namespace apotheosis {

std::string tolower_ru(const std::string &utf8_str) {
    static const std::unordered_map<std::string, std::string> upperToLower = {
        {"А", "а"}, {"Б", "б"}, {"В", "в"}, {"Г", "г"}, {"Д", "д"}, {"Е", "е"},
        {"Ё", "ё"}, {"Ж", "ж"}, {"З", "з"}, {"И", "и"}, {"Й", "й"}, {"К", "к"},
        {"Л", "л"}, {"М", "м"}, {"Н", "н"}, {"О", "о"}, {"П", "п"}, {"Р", "р"},
        {"С", "с"}, {"Т", "т"}, {"У", "у"}, {"Ф", "ф"}, {"Х", "х"}, {"Ц", "ц"},
        {"Ч", "ч"}, {"Ш", "ш"}, {"Щ", "щ"}, {"Ъ", "ъ"}, {"Ы", "ы"}, {"Ь", "ь"},
        {"Э", "э"}, {"Ю", "ю"}, {"Я", "я"}};

    std::string result;
    for (size_t i = 0; i < utf8_str.size();) {
        bool replaced = false;
        if ((utf8_str[i] & 0xE0) == 0xC0 && i + 1 < utf8_str.size()) {
            std::string char2bytes = utf8_str.substr(i, 2);
            auto it = upperToLower.find(char2bytes);
            if (it != upperToLower.end()) {
                result += it->second;
                i += 2;
                replaced = true;
            }
        }
        if (!replaced) {
            result += utf8_str[i];
            i++;
        }
    }

    for (char &c : result) {
        c = tolower(static_cast<unsigned char>(c));
    }

    return result;
}

void Graph::add_vertex(std::string type) {
    vertex_map[type].push_back(m_V);
    vertex_table.push_back(type);
    m_roots.insert(m_V);
    m_V++;
    m_edges.push_back({});
}

void Subgraph::add_vertex(std::string type) {
    Graph::add_vertex(type);
    m_reversed_edges.push_back({});
}

void Graph::add_edge(int v1, int v2) {
    m_edges[v1].insert(v2);
    m_roots.erase(v2);
}

void Subgraph::add_edge(int v1, int v2) {
    Graph::add_edge(v1, v2);
    m_reversed_edges[v2].insert(v1);
}

std::vector<int> Graph::bfs(int root) {
    std::vector<int> result;
    std::queue<int> queue;
    std::vector<int> dist(m_V, 100);

    dist[root] = 0;

    queue.push(root);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        if (dist[current] >= DEPTH_OF_DEVISION) {
            break;
        }

        result.push_back(current);

        for (int neighbor : m_edges[current]) {
            if (dist[neighbor] == 100) {
                dist[neighbor] = dist[current] + 1;
                queue.push(neighbor);
            }
        }
    }
    return result;
}

Subgraph Graph::make_subgraph(const std::vector<int> vertexes, int number) {
    Subgraph result(this->m_name + "_subgraph_" + std::to_string(number));
    for (auto v : vertexes) {
        result.add_vertex(vertex_table[v]);
    }
    for (int i = 0; i < DEPTH_OF_DEVISION; i++) {
        for (int j = 0; j < DEPTH_OF_DEVISION; j++) {
            if (m_edges[vertexes[i]].contains(vertexes[j])) {
                result.add_edge(i, j);
            }
        }
    }
    return result;
}

void Subgraph::dfs(int v, std::vector<int> &used) {
    if (used[v]) {
        return;
    }
    used[v] = 1;
    for (auto u : m_edges[v]) {
        dfs(u, used);
    }
    for (auto u : m_reversed_edges[v]) {
        dfs(u, used);
    }
}

bool check_svyaznost(Subgraph &subgraph) {
    std::vector<int> used(DEPTH_OF_DEVISION, 0);
    subgraph.dfs(0, used);
    if (std::find(used.begin(), used.end(), 0) == used.end()) {
        return true;
    }
    return false;
}

void Graph::make_subgraphs_and_put_into_vector(
    int root,
    std::set<std::vector<int>> &est_li
) {
    std::vector<int> close_vertexes = bfs(root);
    int size = close_vertexes.size();
    int depth = DEPTH_OF_DEVISION;
    sort(close_vertexes.begin(), close_vertexes.end());

    // 5 форов, для DEPTH_OF_DIVISION = 5
    // Вполне естественно, что это надо поменять после MVP
    int i1 = 0;
    for (int i2 = i1 + 1; i2 < size - depth + 2; i2++) {
        for (int i3 = i2 + 1; i3 < size - depth + 3; i3++) {
            for (int i4 = i3 + 1; i4 < size - depth + 4; i4++) {
                for (int i5 = i4 + 1; i5 < size - depth + 5; i5++) {
                    std::vector<int> vertexes = {
                        close_vertexes[i1], close_vertexes[i2],
                        close_vertexes[i3], close_vertexes[i4],
                        close_vertexes[i5]};
                    sort(vertexes.begin(), vertexes.end());
                    if (!est_li.contains(vertexes)) {
                        est_li.insert(vertexes);
                        Subgraph maybe_subgraph =
                            make_subgraph(vertexes, m_subgraphs.size());
                        if (check_svyaznost(maybe_subgraph)) {
                            m_subgraphs.push_back(std::move(maybe_subgraph));
                        }
                    }
                }
            }
        }
    }
}

std::vector<Subgraph> Graph::devide_into_subgraphs() {
    if (m_subgraphs.empty()) {
        std::set<std::vector<int>> est_li;
        for (int root = 0; root < m_V; root++) {
            if (!m_roots.contains(root)) {
                make_subgraphs_and_put_into_vector(root, est_li);
            }
        }
    }
    return m_subgraphs;
}

Graph::Graph(std::string name) : m_V(0), m_E(0), m_name(name) {
}

}  // namespace apotheosis