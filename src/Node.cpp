#include "../include/Node.hpp"
#include <iostream>

Network::Node::Node()
{ }

Network::Node::Node(double t_collector): m_collector{t_collector}
{ }

Network::Node::Node(Activation t_activation): m_activation{t_activation}
{ }

Network::Node::Node(std::vector<std::shared_ptr<Network::Node>> t_connections): m_connections{t_connections}
{ }

Network::Node::Node(double t_collector, Activation t_activation): m_collector{t_collector}, m_activation{t_activation}
{ }

Network::Node::Node(double t_collector, std::vector<std::shared_ptr<Network::Node>> t_connections) : m_collector{t_collector}, m_connections{t_connections}
{ }

Network::Node::Node(Activation t_activation, std::vector<std::shared_ptr<Network::Node>> t_connections) : m_activation{t_activation}, m_connections{t_connections}
{ }

Network::Node::Node(double t_collector, Activation t_activation, std::vector<std::shared_ptr<Network::Node>> t_connections) : m_collector{t_collector}, m_activation{t_activation}, m_connections{t_connections}
{ }

double &Network::Node::collector()
{
    return this->m_collector;
}

const double &Network::Node::collector() const
{
    return this->m_collector;
}

std::vector<std::shared_ptr<Network::Node>> &Network::Node::connections()
{
    return this->m_connections;
}

const std::vector<std::shared_ptr<Network::Node>> &Network::Node::connections() const
{
    return this->m_connections;
}

std::vector<std::shared_ptr<Network::Node>> &Network::Node::back_connections()
{
    return this->m_back_connections;
}

const std::vector<std::shared_ptr<Network::Node>> &Network::Node::back_connections() const
{
    return this->m_back_connections;
}

std::vector<double> &Network::Node::weights()
{
    return this->m_weights;
}

const std::vector<double> &Network::Node::weights() const
{
    return this->m_weights;
}

Network::Node::Activation &Network::Node::activation()
{
    return this->m_activation;
}

const Network::Node::Activation &Network::Node::activation() const
{
    return this->m_activation;
}

void Network::Node::propagate()
{
    double node_output = this->activate();
        
    if (this->activation() != Network::Node::Softmax)
    {
        std::vector<std::shared_ptr<Network::Node>> connections = this->m_connections;
        for (int i = 0; i < connections.size(); i++)
        {
            connections[i]->collector() += this->weights()[i] * node_output;
        }
    }
}

double Network::Node::activate()
{
    double collector = this->collector();
    if (this->m_activation == None)
    {
        return collector;
    }
    else if (this->m_activation == Sigmoid)
    {
        return 1.0 / (1.0 + std::exp(-collector));
    }
    else if (this->m_activation == ReLU)
    {
        return std::max(0.0, collector);
    }
    else if (this->m_activation == LeakyReLU)
    {
        return std::max(0.1 * collector, collector);
    }
    else if (this->m_activation == Softmax)
    {
        // NOTE: Full Softmax activation is not applied yet, as we need to take into account for the whole layers outputs.
        // We just calculate the e^collector, we we have the full outputs of the layers, then we divide by the sum of the e^collector for that layer.
        return collector;
    }
    else if (this->m_activation == Tanh)
    {
        return (std::exp(collector) - std::exp(-collector)) / (std::exp(collector) + std::exp(-collector));
    }
    else if (this->m_activation == Swish)
    {
        return collector / (1.0 + std::exp(-collector));
    }

    return collector;
}

void Network::Node::addConnection(std::shared_ptr<Node> node)
{
    this->connections().emplace_back(node);
    node->back_connections().emplace_back(shared_from_this());
}

void Network::Node::removeConnection(std::shared_ptr<Node> node)
{
    auto iter = std::remove(this->connections().begin(), this->connections().end(), node);
    this->connections().erase(iter, this->connections().end());

    auto ptr_to_remove = std::find_if(node->back_connections().begin(), node->back_connections().end(), [this](const std::shared_ptr<Node>& ptr) 
    {
        return ptr == shared_from_this();
    });

    if (ptr_to_remove != node->back_connections().end())
    {
        node->back_connections().erase(ptr_to_remove);
    }
}