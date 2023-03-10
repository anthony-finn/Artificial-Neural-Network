#include "../include/Node.hpp"
#include <iostream>

Network::Node::Node()
{ }

Network::Node::Node(double t_collector): m_collector{t_collector}
{ }

Network::Node::Node(std::vector<std::shared_ptr<Network::Node>> t_connections): m_connections{t_connections}
{ }

Network::Node::Node(double t_collector, std::vector<std::shared_ptr<Network::Node>> t_connections) : m_collector{t_collector}, m_connections{t_connections}
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

void Network::Node::propagate()
{
    std::vector<std::shared_ptr<Network::Node>> connections = this->m_connections;
    for (int i = 0; i < connections.size(); i++)
    {
        connections[i]->collector() += this->collector();
    }
}