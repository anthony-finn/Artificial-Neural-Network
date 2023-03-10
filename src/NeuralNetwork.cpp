#include "../include/NeuralNetwork.hpp"
#include <iostream>

Network::NeuralNetwork::NeuralNetwork(std::vector<int> t_topology) : m_topology{t_topology}
{ 
    int num_layers = this->m_topology.size();
    for (int i = num_layers - 1; i >= 0; i--)
    {
        int neuron_count = this->m_topology[i];
        int last_neuron_count = i == num_layers - 1 ? 0 : this->m_topology[i + 1];
        for (int k = 0; k < neuron_count; k++)
        {
            std::shared_ptr<Network::Node> node(new Network::Node());

            if (last_neuron_count > 0)
            {
                for (int j = 0; j < last_neuron_count; j++)
                {
                    node->connections().emplace_back(this->m_nodes[this->m_nodes.size() - k - j - 1]);
                }
            }

            this->m_nodes.emplace_back(node);
        }
    }
}

std::vector<int> &Network::NeuralNetwork::topology()
{
    return this->m_topology;
}

const std::vector<int> &Network::NeuralNetwork::topology() const
{
    return this->m_topology;
}

std::vector<std::shared_ptr<Network::Node>> &Network::NeuralNetwork::nodes()
{
    return this->m_nodes;
}

const std::vector<std::shared_ptr<Network::Node>> &Network::NeuralNetwork::nodes() const
{
    return this->m_nodes;
}