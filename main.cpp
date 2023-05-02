#include "include/Node.hpp"
#include "include/NeuralNetwork.hpp"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <random>
#include <iomanip>
#include <assert.h>

using namespace std;

// Read NIST_19 Data located @ https://www.nist.gov/srd/nist-special-database-19
// Normalize Data by converting grayscale 255 to 0-1.
inline vector<vector<double>> load_nist_19_data()
{
    // Read data from handwritten NIST data.
    ifstream file("data/handwritten_data.csv", ios::binary | ios::ate);
    vector<vector<double>> data;

    if (file.is_open())
    {
        // Load file into character buffer
        streamsize size = file.tellg();
        file.seekg(0, ios::beg);

        char* buffer = new char[size];
        file.read(buffer, size);

        file.close();

        // Read file by line and parse data
        const char* end = buffer + size;
        const char* pos = buffer;

        while (pos < end)
        {
            vector<double> row;
            const char* line_end = find(pos, end, '\n');
            const char* line_pos = pos;
            
            while (line_pos < line_end)
            {
                const char* value_end = find(line_pos, line_end, ',');
                const char* value_pos = line_pos;
                double value = 0.0;

                while (value_pos < value_end)
                {
                    value = value * 10 + (*value_pos - '0');
                    ++value_pos;
                }

                // row[0] = Classification
                // row[1-785] = Image Data from 0-255 scaled between 0-1
                if (row.size() == 0)
                {
                    row.push_back(value == 4 ? 1 : 0);
                }
                else
                {
                    row.push_back(value / 255.0);
                }
                line_pos = value_end + 1;
            }

            data.push_back(row);
            pos = line_end + 1;
        }

        delete[] buffer;
    }

    return data;
}

inline void train_test_split(vector<vector<double>> &data, double train_percent, vector<vector<double>> &x_train,  vector<vector<double>> &y_train, vector<vector<double>> &x_test,  vector<vector<double>> &y_test)
{
    const auto split_idx = static_cast<size_t>(data.size() * train_percent);
    vector<vector<double>> train_data(data.begin(), data.begin() + split_idx);
    vector<vector<double>> test_data(data.begin() + split_idx, data.end());

    for (const vector<double> &vec : train_data)
    {
        vector<double> x(vec.begin() + 1, vec.end());
        vector<double> y(1, vec[0]);
        x_train.push_back(x);
        y_train.push_back(y);
    }

    for (const vector<double> &vec : test_data)
    {
        vector<double> x(vec.begin() + 1, vec.end());
        vector<double> y(1, vec[0]);
        x_test.push_back(x);
        y_test.push_back(y);
    }
}

int main(int argc, char **argv)
{
    // Load data
    std::random_device random_device{};
    std::mt19937 gen{random_device()};
    vector<vector<double>> data = load_nist_19_data();
    cout << "Loaded NIST-19 data" << endl;
    shuffle(data.begin(), data.end(), gen);
    assert(data.size() > 0);

    // Split data into 80% train and 20% test
    vector<vector<double>> x_train;  vector<vector<double>> y_train;
    vector<vector<double>> x_test;  vector<vector<double>> y_test;
    train_test_split(data, 0.000268528464018, x_train, y_train, x_test, y_test); // 0.00268528464018
    data.clear();
    data.shrink_to_fit();
    x_test.resize(100);
    y_test.resize(100);

    // Extract feature data
    int output_size = 1;
    int input_size = x_train[0].size();
    cout << "# Datapoints: " << x_train.size() << endl;

    // Create Neural Network
    Network::NeuralNetwork model(
        vector<int> 
        {
            input_size,
            800,
            128,
            64,
            10,
            output_size
        },
        vector<Network::Node::Activation>
        {
            Network::Node::Activation::Sigmoid,
            Network::Node::Activation::Sigmoid,
            Network::Node::Activation::Sigmoid,
            Network::Node::Activation::Sigmoid,
            Network::Node::Activation::Sigmoid,
            Network::Node::Activation::Sigmoid
        }
    );

    double LEARNING_RATE = 0.1;
    int EPOCHS = 500;
    for (int i = 0; i < EPOCHS; i++)
    {
        double loss = model.train(x_train, y_train, LEARNING_RATE, Network::NeuralNetwork::Loss::MSE);
        cout << fixed << ">epoch=" << i << ", learning_rate=" << LEARNING_RATE << ", loss=" << loss << endl;
    }

    model.save("model.txt");

    // Compute Training Accuracy
    int correct = 0;
    for (int i = 0; i < x_train.size(); i++)
    {
        double predicted = model.get_output(x_train[i])[0];
        double expected = y_train[i][0];

        if (predicted > 0.5 && expected == 1)
        {
            correct++;
        }
        else if (predicted <= 0.5 && expected == 0)
        {
            correct ++;
        }
    }
    cout << fixed << "Training Accuracy: " << (double)correct / (double)x_train.size() << "%" << endl;

    // Compute Test Accuracy
    int correct_test = 0;
    for (int i = 0; i < x_test.size(); i++)
    {
        double predicted = model.get_output(x_test[i])[0];
        double expected = y_train[i][0];

        if (predicted > 0.5 && expected == 1)
        {
            correct++;
        }
        else if (predicted <= 0.5 && expected == 0)
        {
            correct ++;
        }
    }
    cout << fixed << "Test Accuracy: " << (double)correct_test / (double)x_test.size() << "%" << endl;

    return 0;
}