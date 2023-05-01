#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cmath>
#include <algorithm>

namespace Network 
{
    class Node
    {
    public:
        // Enums
        enum Activation
        {
            None,
            Sigmoid,
            ReLU,
            Softmax,
            Softplus,
            LeakyReLU,
            Tanh,
            Swish
        };

        // Constructors
        Node();
        Node(double);
        Node(Activation);
        Node(std::vector<Node *>);
        Node(double, Activation);
        Node(double, std::vector<Node *>);
        Node(Activation, std::vector<Node *>);
        Node(double, Activation, std::vector<Node *>);

        // Accessors
        double &collector();
        const double &collector() const;
        std::vector<Node *> &connections();
        const std::vector<Node *> &connections() const;
        std::vector<double> &weights();
        const std::vector<double> &weights() const;
        std::vector<double> &delta_weights();
        const std::vector<double> &delta_weights() const;
        Activation &activation();
        const Activation &activation() const;
        double &bias();
        const double &bias() const;
        double &delta();
        const double &delta() const;

        // Functions
        void activate();
        double transfer();
        double transfer_derivative();
        void addConnection(Node *);
        void removeConnection(Node *);

    private:
        double m_collector{0.0};
        std::vector<Node *> m_connections;
        std::vector<double> m_weights;
        std::vector<double> m_delta_weights;
        Activation m_activation = None;
        double m_bias{0.0};
        double m_delta{0.0};
    };
}

#endif