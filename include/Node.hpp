#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>

namespace Network 
{
    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        // Enums
        enum Activation
        {
            None,
            Sigmoid,
            ReLU,
            LeakyReLU,
            Softmax,
            Tanh,
            Swish
        };

        // Constructors
        Node();
        Node(double);
        Node(Activation);
        Node(std::vector<std::shared_ptr<Node>>);
        Node(double, Activation);
        Node(double, std::vector<std::shared_ptr<Node>>);
        Node(Activation, std::vector<std::shared_ptr<Node>>);
        Node(double, Activation, std::vector<std::shared_ptr<Node>>);

        // Accessors
        double &collector();
        const double &collector() const;
        std::vector<std::shared_ptr<Node>> &connections();
        const std::vector<std::shared_ptr<Node>> &connections() const;
        std::vector<std::shared_ptr<Node>> &back_connections();
        const std::vector<std::shared_ptr<Node>> &back_connections() const;
        std::vector<double> &weights();
        const std::vector<double> &weights() const;
        Activation &activation();
        const Activation &activation() const;

        // Functions
        void propagate();
        double activate();
        void addConnection(std::shared_ptr<Node>);
        void removeConnection(std::shared_ptr<Node>);

    private:
        double m_collector{0.0};
        std::vector<std::shared_ptr<Node>> m_connections;
        std::vector<std::shared_ptr<Node>> m_back_connections;
        std::vector<double> m_weights;
        Activation m_activation = None;
    };
}

#endif