#include <vector>
#include "Node.hpp"

namespace Network 
{
    class Network
    {
    private:
        std::vector<int> m_topology;
        std::vector<std::vector<Node *>> m_layers;

    public:
        // Constructors
        Network(std::vector<int>);
    };
}