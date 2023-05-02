#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <memory>
#include <string>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include "Node.hpp"

namespace Network 
{
    class NeuralNetwork
    {
    private:
        std::vector<int> m_topology;
        std::vector<Network::Node::Activation> m_layer_activations;
        std::vector<Network::Node *> m_nodes;
        std::vector<std::vector<Network::Node *>> m_layer_nodes;

        void init(std::vector<double>, std::vector<double>);
    public:
        // Enums
        enum Loss
        {
            MSE,
            BinaryCrossEntropy,
            CategoricalCrossEntropy,
        };

        // Constructors
        NeuralNetwork(std::vector<int>);
        NeuralNetwork(std::vector<int>, std::vector<double>);
        NeuralNetwork(std::vector<int>, std::vector<Network::Node::Activation>);
        NeuralNetwork(std::vector<int>, std::vector<double>, std::vector<Network::Node::Activation>);
        NeuralNetwork(std::string);
        ~NeuralNetwork();

        // Accessors
        std::vector<int> &topology();
        const std::vector<int> &topology() const;
        std::vector<Network::Node::Activation> &layer_activiations();
        const std::vector<Network::Node::Activation> &layer_activiations() const;
        std::vector<Network::Node *> &nodes();
        const std::vector<Network::Node *> &nodes() const;
        std::vector<std::vector<Network::Node *>> &layer_nodes();
        const std::vector<std::vector<Network::Node *>> &layer_nodes() const;

        // Functions
        std::vector<double> get_output(std::vector<double> &);
        double get_loss(std::vector<double> &, std::vector<double> &, Loss);
        double get_loss_derivative(std::vector<double> &, std::vector<double> &, Loss);
        void save(std::string);
        double train(std::vector<std::vector<double>> &, std::vector<std::vector<double>> &, double, Network::NeuralNetwork::Loss);
    };
}

#endif