#include "Node.hpp"

Network::Node::Node()
{ }

Network::Node::Node(double t_collector): m_collector{t_collector}
{ }

Network::Node::Node(std::vector<Node *> t_connections): m_connections{t_connections}
{ }

Network::Node::Node(double t_collector, std::vector<Node *> t_connections) : m_collector{t_collector}, m_connections{t_connections}
{ }

double & Network::Node::collector()
{
    return this->m_collector;
}

const double & Network::Node::collector() const
{
    return this->m_collector;
}

std::vector<Network::Node *> & Network::Node::connections()
{
    return this->m_connections;
}
const std::vector<Network::Node *> & Network::Node::connections() const
{
    return this->m_connections;
}