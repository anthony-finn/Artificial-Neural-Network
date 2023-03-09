#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>

namespace Network 
{
    class Node
    {
    private:
        double m_collector{0.0};
        std::vector<Node *> *m_connections;

    public:
        // Constructors
        Node();
        Node(double);
        Node(double, int);
        Node(std::vector<Node *> *);
        Node(double, std::vector<Node *> *);

        // Accessors
        double &collector();
        const double &collector() const;
        std::vector<Node *> *&connections();
        const std::vector<Node *> *connections() const;
    };
}

#endif