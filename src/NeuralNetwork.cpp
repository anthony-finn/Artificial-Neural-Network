#include "../include/NeuralNetwork.hpp"

Network::NeuralNetwork::NeuralNetwork(std::vector<int> t_topology) : m_topology{t_topology}
{ 
    int num_layers = this->m_topology.size();
    this->m_layers.reserve(num_layers);
    std::vector<Node *> last_layer;
    for (int i = num_layers - 1; i >= 0; i--)
    {
        int neuron_count = this->m_topology[i];
        std::vector<Node *> layer(neuron_count);
        this->m_layers.push_back(layer);
        
        for (int k = 0; k < neuron_count; k++)
        {
            Node *neuron = new Node();
            neuron->connections() = last_layer;
            layer[k] = neuron;
        }

        last_layer = layer;
    }
}

Network::NeuralNetwork::~NeuralNetwork()
{
    for (int i = 0; i < this->m_layers.size(); i++)
    {
        for (int k = 0; k < this->m_layers[i].size(); k++)
        {
            delete this->m_layers[i][k];
            this->m_layers[i][k] = nullptr;
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

std::vector<std::vector<Network::Node *>> &Network::NeuralNetwork::layers()
{
    return this->m_layers;
}

const std::vector<std::vector<Network::Node *>> &Network::NeuralNetwork::layers() const
{
    return this->m_layers;
}