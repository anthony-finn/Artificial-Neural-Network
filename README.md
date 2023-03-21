# Artificial-Neural-Network

## Input Files
* ```input.csv``` Comma-separated values which define the input for the neural network.
* ```network_topology.csv``` Comma-separated values which define the structure of the neural network. The leftmost value is the input layer, and the rightmost value is the output layer.

## Build
A shell script is provided to build the required .cpp files.
```console
./build.sh
./a.out
```
Otherwise, you may build the software utilizing the following code.
```console
g++ main.cpp src/Node.cpp src/NeuralNetwork.cpp
```
