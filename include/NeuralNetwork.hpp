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
        std::vector<Network::Node::Activation> m_layer_activations;
        std::vector<std::shared_ptr<Network::Node>> m_nodes;

        void init(std::vector<double>);
    public:
        // Constructors
        NeuralNetwork(std::vector<int>);
        NeuralNetwork(std::vector<int>, std::vector<double>);
        NeuralNetwork(std::vector<int>, std::vector<Network::Node::Activation>);
        NeuralNetwork(std::vector<int>, std::vector<double>, std::vector<Network::Node::Activation>);
        NeuralNetwork(std::string);

        // Accessors
        std::vector<int> &topology();
        const std::vector<int> &topology() const;
        std::vector<Network::Node::Activation> &layer_activiations();
        const std::vector<Network::Node::Activation> &layer_activiations() const;
        std::vector<std::shared_ptr<Network::Node>> &nodes();
        const std::vector<std::shared_ptr<Network::Node>> &nodes() const;

        // Functions
        std::vector<double> getOutput(std::vector<double>);
        void save(std::string) const;
    };
}

#endif