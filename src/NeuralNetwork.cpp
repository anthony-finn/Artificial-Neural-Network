#include "../include/NeuralNetwork.hpp"

Network::NeuralNetwork::NeuralNetwork(std::vector<int> t_topology) : m_topology{t_topology}
{ 
    assert(t_topology.size() >= 2);

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

std::vector<double> Network::NeuralNetwork::getOutput(std::vector<double> input)
{
    std::vector<std::shared_ptr<Network::Node>> nodes = this->m_nodes;
    std::vector<int> topology = this->m_topology;

    int input_size = topology[0];

    assert(input_size == input.size());

    int num_nodes = nodes.size();
    for (int i = 0; i < num_nodes; i++)
    {
        nodes[i]->collector() = 0;
    }

    for (int i = 0; i < input_size; i++)
    {
        nodes[nodes.size() - i - 1]->collector() = input[i];
    }

    for (int i = nodes.size() - 1; i >= 0; i--)
    {
        nodes[i]->propagate();
    }
    
    int output_size = topology[topology.size() - 1];
    std::vector<double> output(output_size);

    for (int i = 0; i < output_size; i++)
    {
        output[i] = nodes[i]->collector();
    }

    return output;
}