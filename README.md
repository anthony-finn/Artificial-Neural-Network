# Artificial-Neural-Network

## Input Files
* ```input.csv``` Comma-separated values which define the input for the neural network.
* ```network_topology.csv``` Comma-separated values which define the structure of the neural network. The leftmost value is the input layer, and the rightmost value is the output layer.
* ```network_activations.csv``` Comma-separated values which define the activation functions used for each layer in the network topology. See the `Activation Functions` section for the values.

## Activation Functions
* None = 0
* Sigmoid = 1
* ReLU = 2
* LeakyReLU = 3
* Softmax = 4
* Tanh = 5
* Swish = 6

## Build
A shell script is provided to build the required .cpp files.
```console
./build.sh
```
Otherwise, you may build the software utilizing the following code.
```console
g++ main.cpp src/Node.cpp src/NeuralNetwork.cpp
```
Then, run the executable.
```console
./a.out
```
