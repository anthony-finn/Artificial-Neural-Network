#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>

namespace Network 
{
    class Node
    {
    private:
        double m_collector{0.0};
        std::vector<std::shared_ptr<Node>> m_connections;
        std::vector<double> m_weights;

    public:
        // Constructors
        Node();
        Node(double);
        Node(std::vector<std::shared_ptr<Node>>);
        Node(double, std::vector<std::shared_ptr<Node>>);

        // Accessors
        double &collector();
        const double &collector() const;
        std::vector<std::shared_ptr<Node>> &connections();
        const std::vector<std::shared_ptr<Node>> &connections() const;
        std::vector<double> &weights();
        const std::vector<double> &weights() const;

        // Functions
        void propagate();
    };
}

#endif