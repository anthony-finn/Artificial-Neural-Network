#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <memory>
#include <string>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Node.hpp"

namespace Network 
{
    class NeuralNetwork
    {
    private:
        std::vector<int> m_topology;
        std::vector<std::shared_ptr<Network::Node>> m_nodes;

        void init(std::vector<int>, std::vector<double>);
    public:
        // Constructors
        NeuralNetwork(std::vector<int>);
        NeuralNetwork(std::vector<int>, std::vector<double>);
        NeuralNetwork(std::string);

        // Accessors
        std::vector<int> &topology();
        const std::vector<int> &topology() const;
        std::vector<std::shared_ptr<Network::Node>> &nodes();
        const std::vector<std::shared_ptr<Network::Node>> &nodes() const;

        // Functions
        std::vector<double> getOutput(std::vector<double>);
        void save(std::string) const;
    };
}

#endif