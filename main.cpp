#include "include/Node.hpp"
#include "include/NeuralNetwork.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>

using namespace std;

int main(int argc, char **argv)
{
    vector<int> topology;
    vector<double> input;
    
    // Read Neural Network Topology
    string layer_neurons;
    ifstream file("network_topology.csv");
    while (getline(file, layer_neurons, ','))
    {
        topology.push_back(stoi(layer_neurons));
    }

    // Read User Input
    for (int i = 1; i < argc; i++)
    {
        input.push_back(stof(argv[i]));
    }

    assert(input.size() == topology.size());

    Network::NeuralNetwork neural_network(topology);
    vector<double> outputs = neural_network.getOutput(input);

    for (int i = 0; i < outputs.size(); i++)
    {
        cout << fixed << "Output: " << outputs[i] << endl;
    }

    return 0;
}