#include "include/Node.hpp"
#include "include/NeuralNetwork.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <assert.h>

using namespace std;

int main(int argc, char **argv)
{
    std::srand(std::time(nullptr));

    vector<int> topology;
    vector<Network::Node::Activation> activations;
    vector<double> input;
    
    // Read Neural Network Topology
    string inputs;
    ifstream network_topology_file("input/network_topology.csv");
    while (getline(network_topology_file, inputs, ','))
    {
        topology.push_back(stod(inputs));
    }

    // Read Neural Network Activations
    ifstream network_activations("input/network_activations.csv");
    while (getline(network_activations, inputs, ','))
    {
        activations.push_back(static_cast<Network::Node::Activation>(stoi(inputs)));
    }
    
    // Read Network Inputs
    ifstream input_file("input/input.csv");
    while (getline(input_file, inputs, ','))
    {
        input.push_back(stof(inputs));
    }

    Network::NeuralNetwork neural_network(topology, activations);
    vector<double> outputs = neural_network.getOutput(input);

    for (int i = 0; i < outputs.size(); i++)
    {
        cout << fixed << "Output: " << outputs[i] << endl;
    }

    neural_network.save("model.txt");
    return 0;
}