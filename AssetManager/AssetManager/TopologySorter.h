#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <stdexcept>

template<typename T>
class TopologySorter
{
private:
	std::unordered_map<T, std::vector<T>> m_topologyNodes;

public:
	void AddTopologyNode(T&& nodeIn);
	void AddPrequisite(T&& nodeIn, T&& prequisiteIn);

public:
	std::vector<T> GetTopologySort() const;
};

template<typename T>
void TopologySorter<T>::AddTopologyNode(T&& nodeIn)
{
    if (m_topologyNodes.find(std::forward<T>(nodeIn)) == m_topologyNodes.end())
    {
        m_topologyNodes.emplace(std::forward<T>(nodeIn), std::vector<T>());
    }

}

template<typename T>
void TopologySorter<T>::AddPrequisite(T&& nodeIn, T&& prequisiteIn)
{
    m_topologyNodes[std::forward<T>(nodeIn)].emplace_back(std::forward<T>(prequisiteIn));
}



template<typename T>
inline std::vector<T> TopologySorter<T>::GetTopologySort() const
{
    std::vector<T> result;
    std::unordered_map<T, bool> isVisited;
    std::unordered_map<T, bool> isResursive;
    bool hasCycle = false;

    std::function<void(const T&)> dfs = [&](const T& current)
        {
            if (isResursive[current])
            {
                hasCycle = true;
                return;
            }
            if (isVisited[current])
            {
                return;
            }

            isVisited[current] = true;
            isResursive[current] = true;

            // 인접 노드 방문
            for (const auto& neighbor : m_topologyNodes.at(current))
            {
                dfs(neighbor);
            }

            isResursive[current] = false;
            result.push_back(current);
        };

    for (const auto& nodeToPrequisites : m_topologyNodes)
    {
        const T& node = nodeToPrequisites.first;

        if (!isVisited[node])
        {
            dfs(node);

            if (hasCycle)
            {
                throw std::runtime_error("Cycle Detected While Topology Sorting");
            }
        }
    }

    return result;
}