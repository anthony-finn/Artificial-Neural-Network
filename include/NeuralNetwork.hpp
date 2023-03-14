#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <memory>
#include <assert.h>

#include "Node.hpp"

namespace Network 
{
    class NeuralNetwork
    {
    private:
        std::vector<int> m_topology;
        std::vector<std::shared_ptr<Network::Node>> m_nodes;

    public:
        // Constructors
        NeuralNetwork(std::vector<int>);

        // Accessors
        std::vector<int> &topology();
        const std::vector<int> &topology() const;
        std::vector<std::shared_ptr<Network::Node>> &nodes();
        const std::vector<std::shared_ptr<Network::Node>> &nodes() const;

        // Functions
        std::vector<double> getOutput(std::vector<double>);
    };
}

#endif