#include "../include/NeuralNetwork.hpp"

Network::NeuralNetwork::NeuralNetwork(std::vector<int> t_topology) : m_topology{t_topology}
{
    this->init(t_topology, std::vector<double>());
}

Network::NeuralNetwork::NeuralNetwork(std::vector<int> t_topology, std::vector<double> t_weights) : m_topology{t_topology}
{
    this->init(t_topology, t_weights);
}

Network::NeuralNetwork::NeuralNetwork(std::string input_path)
{
    std::ifstream input_file(input_path);

    if (input_file.is_open())
    {
        // Topology
        std::string topology_str;
        std::getline(input_file, topology_str);
        std::cout << topology_str << std::endl;
        std::stringstream topology_ss(topology_str);
        std::vector<int> topology;
        std::string layer;
        while (std::getline(topology_ss, layer, ','))
        {
            topology.push_back(std::stoi(layer));
        }

        // Weights
        std::string weights_str;
        std::getline(input_file, weights_str);
        std::cout << weights_str << std::endl;
        std::stringstream weights_ss(weights_str);
        std::vector<double> weights;
        std::string weight;
        while (std::getline(weights_ss, weight, ','))
        {
            weights.push_back(std::stod(weight));
        }

        // Create Class
        this->m_topology = topology;
        this->init(topology, weights);

        input_file.close();
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
    int output_size = topology[topology.size() - 1];

    assert(input_size == input.size());

    int num_nodes = nodes.size();
    for (int i = 0; i < num_nodes; i++)
    {
        nodes[i]->collector() = 0;
    }

    for (int i = 0; i < input_size; i++)
    {
        nodes[num_nodes - i - 1]->collector() = input[i];
    }

    for (int i = num_nodes - 1; i >= output_size; i--)
    {
        nodes[i]->propagate();
    }
    
    std::vector<double> output(output_size);

    for (int i = 0; i < output_size; i++)
    {
        output[i] = nodes[i]->collector();
    }

    return output;
}

void Network::NeuralNetwork::save(std::string output_path) const
{
    std::ofstream output_file(output_path);

    if (output_file.is_open())
    {
        std::vector<std::shared_ptr<Network::Node>> nodes = this->m_nodes;
        std::vector<int> topology = this->m_topology;

        // Save Topology
        for (int i = 0; i < topology.size(); i++)
        {
            output_file << std::fixed << topology[i];

            if (i + 1 < topology.size())
            {
                output_file << ",";
            }
        }

        output_file << std::endl;

        // Save Weights
        int num_nodes = nodes.size();
        for (int i = 0; i < num_nodes; i++)
        {
            std::shared_ptr<Network::Node> node = nodes[i];
            std::vector<double> weights = node->weights();
            for (int k = 0; k < weights.size(); k++)
            {
                output_file << std::fixed << weights[k];

                if (i + 1 == num_nodes && k + 1 >= weights.size())
                {
                    break;
                }

                output_file << ",";
            }
        }

        output_file.close();
    }
}

void Network::NeuralNetwork::init(std::vector<int> t_topology, std::vector<double> t_weights)
{
    assert(t_topology.size() >= 2);

    int num_layers = this->m_topology.size();
    int num_nodes = 0;
    int weight_idx = 0;
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
                    node->connections().emplace_back(this->m_nodes[num_nodes - k - j - 1]);
                    if (t_weights.empty())
                    {
                        node->weights().push_back(1.0);
                    }
                    else
                    {
                        node->weights().push_back(t_weights[weight_idx++]);
                    }
                }
            }

            this->m_nodes.emplace_back(node);
            num_nodes++;
        }
    }
}