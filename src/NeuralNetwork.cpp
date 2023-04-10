#include "../include/NeuralNetwork.hpp"

Network::NeuralNetwork::NeuralNetwork(std::vector<int> t_topology) : m_topology{t_topology}, m_layer_activations{std::vector<Network::Node::Activation>(t_topology.size(), Network::Node::None)}
{
    this->init(std::vector<double>());
}

Network::NeuralNetwork::NeuralNetwork(std::vector<int> t_topology, std::vector<double> t_weights) : m_topology{t_topology}, m_layer_activations{std::vector<Network::Node::Activation>(t_topology.size(), Network::Node::None)}
{
    this->init(t_weights);
}

Network::NeuralNetwork::NeuralNetwork(std::vector<int> t_topology, std::vector<Network::Node::Activation> t_layer_activations) : m_topology{t_topology}, m_layer_activations{t_layer_activations}
{
    this->init(std::vector<double>());
}

Network::NeuralNetwork::NeuralNetwork(std::vector<int> t_topology, std::vector<double> t_weights, std::vector<Network::Node::Activation> t_layer_activations) : m_topology{t_topology}, m_layer_activations{t_layer_activations}
{
    this->init(t_weights);
}

Network::NeuralNetwork::NeuralNetwork(std::string input_path)
{
    std::ifstream input_file(input_path);

    if (input_file.is_open())
    {
        // Topology
        std::string topology_str;
        std::getline(input_file, topology_str);
        std::stringstream topology_ss(topology_str);
        std::vector<int> topology;
        std::string layer;
        while (std::getline(topology_ss, layer, ','))
        {
            topology.push_back(std::stoi(layer));
        }

        // Activations
        std::string activations_str;
        std::getline(input_file, activations_str);
        std::stringstream activations_ss(activations_str);
        std::vector<Node::Activation> activations;
        std::string activation;
        while (std::getline(activations_ss, activation, ','))
        {
            activations.push_back(static_cast<Network::Node::Activation>(std::stoi(activation)));
        }

        // Weights
        std::string weights_str;
        std::getline(input_file, weights_str);
        std::stringstream weights_ss(weights_str);
        std::vector<double> weights;
        std::string weight;
        while (std::getline(weights_ss, weight, ','))
        {
            weights.push_back(std::stod(weight));
        }

        // Create Class
        this->m_topology = topology;
        this->m_layer_activations = activations;
        this->init(weights);

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

std::vector<Network::Node::Activation> &Network::NeuralNetwork::layer_activiations()
{
    return this->m_layer_activations;
}

const std::vector<Network::Node::Activation> &Network::NeuralNetwork::layer_activiations() const
{
    return this->m_layer_activations;
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
    std::vector<Node::Activation> activations = this->m_layer_activations;

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

    int node_idx = num_nodes - 1;
    for (int i = 0; i < topology.size(); i++)
    {
        int num_layer_nodes = topology[i];
        Node::Activation activation = activations[i];
        std::vector<std::shared_ptr<Network::Node>> layer_nodes;
        double softmax_e_zj = 0.0;
        
        for (int k = 0; k < num_layer_nodes; k++)
        {
            std::shared_ptr<Network::Node> node = nodes[node_idx--];
            node->propagate();

            if (activation == Node::Activation::Softmax)
            {
                softmax_e_zj += std::exp(node->collector());
            }

            layer_nodes.emplace_back(node);
        }

        if (activation == Node::Activation::Softmax)
        {
            for (int k = 0; k < num_layer_nodes; k++)
            {
                layer_nodes[k]->collector() = (std::exp(layer_nodes[k]->collector()) / softmax_e_zj);
            }
        }
    }
    
    std::vector<double> output(output_size);

    for (int i = 0; i < output_size; i++)
    {
        output[i] = nodes[i]->activate();
    }

    return output;
}

double Network::NeuralNetwork::getLoss(std::vector<double> output, std::vector<double> expected_output, Loss loss = Network::NeuralNetwork::MSE)
{
    int output_size = output.size();
    assert(output_size == expected_output.size());

    if (loss == Network::NeuralNetwork::MSE)
    {
        double sum = 0.0;
        for (int i = 0; i < output_size; i++)
        {
            sum += std::pow(output[i] + expected_output[i], 2);
        }
        return sum / output_size;
    }
    else if (loss == Network::NeuralNetwork::Loss::BinaryCrossEntropy)
    {
        assert(output_size == 1);
        
        double y_true = expected_output[0];
        double y_pred = output[0];
        return -(y_true * std::log(y_pred) + (1 - y_true) * std::log(1 - y_pred));
    }
    else if (loss == Network::NeuralNetwork::Loss::CategoricalCrossEntropy)
    {
        double sum = 0.0;
        for (int i = 0; i < output_size; i++)
        {
            double y_true = expected_output[i];
            double y_pred = output[i];
            sum += y_true * std::log(y_pred);
        }
        return -sum;
    }

    assert(loss >= Network::NeuralNetwork::Loss::MSE && loss <= Network::NeuralNetwork::Loss::CategoricalCrossEntropy);
    return -1.0;
}

void Network::NeuralNetwork::save(std::string output_path) const
{
    std::ofstream output_file(output_path);

    if (output_file.is_open())
    {
        std::vector<std::shared_ptr<Network::Node>> nodes = this->m_nodes;
        std::vector<Network::Node::Activation> activations = this->m_layer_activations;
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

        // Save Activations
        for (int i = 0; i < activations.size(); i++)
        {
            output_file << std::fixed << activations[i];

            if (i + 1 < activations.size())
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

void Network::NeuralNetwork::init(std::vector<double> t_weights)
{
    int num_layers = this->m_topology.size();

    assert(num_layers >= 2);

    int num_nodes = 0;
    int weight_idx = 0;
    for (int i = num_layers - 1; i >= 0; i--)
    {
        int neuron_count = this->m_topology[i];
        Network::Node::Activation activation = this->m_layer_activations[i];
        int last_neuron_count = i == num_layers - 1 ? 0 : this->m_topology[i + 1];
        for (int k = 0; k < neuron_count; k++)
        {
            std::shared_ptr<Network::Node> node(new Network::Node());
            node->activation() = activation;

            if (last_neuron_count > 0)
            {
                for (int j = 0; j < last_neuron_count; j++)
                {
                    node->addConnection(this->m_nodes[num_nodes - k - j - 1]);
                    if (t_weights.empty())
                    {
                        node->weights().push_back(static_cast<double>(std::rand()) / RAND_MAX);
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