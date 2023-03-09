#ifndef NODE_H
#define NODE_H

#include <vector>

class Node
{
private:
    double m_collector{0.0};
    std::vector<Node *> m_connections;

public:
    Node();
    Node(double);
    Node(double, std::vector<Node *>);

    double & collector();
    const double & collector() const;
    std::vector<Node *> & connections();
    const std::vector<Node *> & connections() const;
};

#endif