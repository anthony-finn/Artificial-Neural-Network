#include "../include/Node.hpp"

Network::Node::Node()
{ }

Network::Node::Node(double t_collector): m_collector{t_collector}
{ }

Network::Node::Node(Activation t_activation): m_activation{t_activation}
{ }

Network::Node::Node(std::vector<Network::Node *> t_connections): m_connections{t_connections}
{ }

Network::Node::Node(double t_collector, Activation t_activation): m_collector{t_collector}, m_activation{t_activation}
{ }

Network::Node::Node(double t_collector, std::vector<Network::Node *> t_connections) : m_collector{t_collector}, m_connections{t_connections}
{ }

Network::Node::Node(Activation t_activation, std::vector<Network::Node *> t_connections) : m_activation{t_activation}, m_connections{t_connections}
{ }

Network::Node::Node(double t_collector, Activation t_activation, std::vector<Network::Node *> t_connections) : m_collector{t_collector}, m_activation{t_activation}, m_connections{t_connections}
{ }

double &Network::Node::collector()
{
    return this->m_collector;
}

const double &Network::Node::collector() const
{
    return this->m_collector;
}

std::vector<Network::Node *> &Network::Node::connections()
{
    return this->m_connections;
}

const std::vector<Network::Node *> &Network::Node::connections() const
{
    return this->m_connections;
}

std::vector<double> &Network::Node::weights()
{
    return this->m_weights;
}

const std::vector<double> &Network::Node::weights() const
{
    return this->m_weights;
}

std::vector<double> &Network::Node::delta_weights()
{
    return this->m_delta_weights;
}

const std::vector<double> &Network::Node::delta_weights() const
{
    return this->m_delta_weights;
}

Network::Node::Activation &Network::Node::activation()
{
    return this->m_activation;
}

const Network::Node::Activation &Network::Node::activation() const
{
    return this->m_activation;
}

double &Network::Node::bias()
{
    return this->m_bias;
}

const double &Network::Node::bias() const
{
    return this->m_bias;
}

double &Network::Node::delta()
{
    return this->m_delta;
}

const double &Network::Node::delta() const
{
    return this->m_delta;
}

void Network::Node::activate()
{
    this->collector() = this->transfer();
    
    std::vector<Network::Node *> *connections = &this->connections();
    for (int i = 0; i < connections->size(); i++)
    {
        Network::Node *node = (*connections)[i];
        node->collector() += this->weights()[i] * this->collector();
    }
}

double Network::Node::transfer()
{
    double collector = this->collector() + this->bias();
    Network::Node::Activation activation = this->activation();

    switch (activation)
    {
        case Network::Node::Activation::None:
            return collector;
        case Network::Node::Activation::Sigmoid:
            return 1.0 / (1.0 + std::exp(-collector));
        case Network::Node::Activation::ReLU:
            return std::max(0.0, collector);
        case Network::Node::Activation::LeakyReLU:
            return std::max(0.1 * collector, collector);
        case Network::Node::Softplus:
            return std::log(1 + std::exp(collector));
        case Network::Node::Activation::Tanh:
            return std::tanh(collector);
        case Network::Node::Activation::Swish:
            return collector / (1.0 + std::exp(-collector));
    };

    return collector;
}

double Network::Node::transfer_derivative()
{
    double collector = this->collector();
    Network::Node::Activation activation = this->activation();

    switch (activation)
    {
        case Network::Node::Activation::None:
            return 1.0;
        case Network::Node::Activation::Sigmoid:
            return collector * (1.0 - collector);
        case Network::Node::Activation::ReLU:
            return collector > 0.0 ? 1.0 : 0.0;
        case Network::Node::Activation::LeakyReLU:
            return collector > 0.0 ? 1.0 : 0.1;
        case Network::Node::Softplus:
            return std::exp(collector) / (1 + std::exp(collector));
        case Network::Node::Activation::Tanh:
            return 1 - std::tanh(collector);
        case Network::Node::Activation::Swish:
            return (collector * std::exp(-collector) + 1 + std::exp(-collector)) / std::pow((1 + std::exp(-collector)), 2.0);
    };

    return collector;
}

void Network::Node::addConnection(Network::Node *node)
{
    this->connections().emplace_back(node);
}

void Network::Node::removeConnection(Network::Node *node)
{

    auto ptr_to_remove = std::find_if(this->connections().begin(), this->connections().end(), [this](const Network::Node *ptr) 
    {
        return ptr == this;
    });

    if (ptr_to_remove != this->connections().end())
    {
        this->connections().erase(ptr_to_remove);
    }
}