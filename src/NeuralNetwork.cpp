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

Network::NeuralNetwork::~NeuralNetwork()
{
    std::vector<Network::Node *> *nodes = &this->nodes();
    for (int i = 0; i < nodes->size(); i++)
    {
        delete (*nodes)[i];
    }
    
    nodes->clear();
    nodes->shrink_to_fit();
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

std::vector<Network::Node *> &Network::NeuralNetwork::nodes()
{
    return this->m_nodes;
}

const std::vector<Network::Node *> &Network::NeuralNetwork::nodes() const
{
    return this->m_nodes;
}

std::vector<double> Network::NeuralNetwork::getOutput(std::vector<double> &input)
{
    std::vector<Network::Node *> *nodes = &this->nodes();
    std::vector<int> *topology = &this->topology();
    std::vector<Node::Activation> *activations = &this->layer_activiations();

    int input_size = (*topology)[0];
    int output_size = (*topology)[topology->size() - 1];

    assert(input_size == input.size());

    int num_nodes = nodes->size();
    for (int i = 0; i < input_size; i++)
    {
        Network::Node *node = (*nodes)[num_nodes - i - 1];
        node->collector() = input[i];
        node->activate();
    }

    for (int i = num_nodes - 1 - input_size; i >= 0; i--)
    {
        (*nodes)[i]->activate();
    }
    
    std::vector<double> output(output_size);
    for (int i = 0; i < output_size; i++)
    {
        output[i] = (*nodes)[i]->collector();
    }

    return output;
}

double Network::NeuralNetwork::getLoss(std::vector<double> &output, std::vector<double> &expected_output, Loss loss = Network::NeuralNetwork::MSE)
{
    int output_size = output.size();
    assert(output_size == expected_output.size());

    if (loss == Network::NeuralNetwork::MSE)
    {
        double sum = 0.0;
        for (int i = 0; i < output_size; i++)
        {
            sum += std::pow(expected_output[i] - output[i], 2);
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

    return -1.0;
}

void Network::NeuralNetwork::save(std::string output_path)
{
    std::ofstream output_file(output_path);

    if (output_file.is_open())
    {
        std::vector<Network::Node *> *nodes = &this->nodes();
        std::vector<int> *topology = &this->topology();
        std::vector<Node::Activation> *activations = &this->layer_activiations();
        int topology_size = topology->size();
        int nodes_size = nodes->size();
        int activations_size = activations->size();

        // Save Topology
        for (int i = 0; i < topology_size; i++)
        {
            output_file << std::fixed << (*topology)[i];

            if (i + 1 < topology_size)
            {
                output_file << ",";
            }
        }

        output_file << std::endl;

        // Save Activations
        for (int i = 0; i < activations_size; i++)
        {
            output_file << std::fixed << (*activations)[i];

            if (i + 1 < activations_size)
            {
                output_file << ",";
            }
        }

        output_file << std::endl;

        // Save Weights
        for (int i = 0; i < nodes_size; i++)
        {
            Network::Node *node = (*nodes)[i];
            std::vector<double> *weights = &node->weights();
            int weights_size = weights->size();
            for (int k = 0; k < weights_size; k++)
            {
                output_file << std::fixed << (*weights)[k];

                if (i + 1 == nodes_size && k + 1 >= weights_size)
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
    std::vector<int> *topology = &this->topology();
    std::vector<Network::Node *> *nodes = &this->nodes();
    int num_layers = topology->size();

    assert(num_layers >= 2);

    int num_nodes = 0;
    int weight_idx = 0;
    for (int i = num_layers - 1; i >= 0; i--)
    {
        int neuron_count = (*topology)[i];
        Network::Node::Activation activation = this->layer_activiations()[i];
        int last_neuron_count = i == num_layers - 1 ? 0 : (*topology)[i + 1];
        for (int k = 0; k < neuron_count; k++)
        {
            Network::Node * node = new Network::Node();
            node->activation() = activation;

            if (last_neuron_count > 0)
            {
                for (int j = 0; j < last_neuron_count; j++)
                {
                    node->addConnection((*nodes)[num_nodes - k - j - 1]);
                    if (t_weights.empty())
                    {
                        node->weights().push_back(0.5);
                    }
                    else
                    {
                        node->weights().push_back(t_weights[weight_idx++]);
                    }
                }
            }

            nodes->emplace_back(node);
            num_nodes++;
        }
    }
}

double Network::NeuralNetwork::fit(std::vector<std::vector<double>> x_train, std::vector<std::vector<double>> y_train, std::vector<std::vector<double>> x_test, std::vector<std::vector<double>> y_test)
{
    return 0.0;
}