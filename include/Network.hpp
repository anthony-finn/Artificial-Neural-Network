#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "Node.hpp"

namespace Network 
{
    class Network
    {
    private:
        std::vector<int> m_topology;
        std::vector<std::vector<Node *>> m_layers;

    public:
        // Constructors
        Network(std::vector<int>);

        // Accessors
        std::vector<int> &topology();
        const std::vector<int> &topology() const;
        std::vector<std::vector<Node *>> &layers();
        const std::vector<std::vector<Node *>> &layers() const;
    };
}

#endif