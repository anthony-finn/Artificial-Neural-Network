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
    string inputs;
    ifstream network_topology_file("network_topology.csv");
    while (getline(network_topology_file, inputs, ','))
    {
        topology.push_back(stoi(inputs));
    }

    // Read Network Inputs
    ifstream input_file("input.csv");
    while (getline(input_file, inputs, ','))
    {
        input.push_back(stof(inputs));
    }

    Network::NeuralNetwork neural_network(topology);
    vector<double> outputs = neural_network.getOutput(input);

    for (int i = 0; i < outputs.size(); i++)
    {
        cout << fixed << "Output: " << outputs[i] << endl;
    }

    return 0;
}