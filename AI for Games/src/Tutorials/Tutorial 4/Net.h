#pragma once
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cmath>

class Neuron;
typedef std::vector<Neuron> Layer;

#pragma region Neuron

struct Connection {
	double weight;
	double deltWeight;
};

class Neuron {
public:
	Neuron(unsigned numOutputs, unsigned myIndex);
	void setOutputValue(double value) { m_outputValue = value; }
	double getOutputValue() const { return m_outputValue; }
	void feedForward(const Layer &previousLayer);

	void calcOutputGradients(double targetValue);
	void calcHiddenGradients(const Layer &nextLayer);
	void UpdateInputWeights(Layer &previousLayer);
private:
	static double eta;//[0..1] overall net training rate
	static double alpha;//[0..n] multiplier of last weight change (momentum)
	static double transferFunction(double x);
	static double transferFunctionDerivative(double x);
	static double randomWeight() { return rand() / double(RAND_MAX); }
	double sumDOW(const Layer &nextLayer) const;
	double m_outputValue;
	std::vector <Connection> m_outputWeights;
	unsigned m_myIndex;
	double m_gradient;
};
double Neuron::eta = 0.15;
double Neuron::alpha = 0.5;

Neuron::Neuron(unsigned numOutputs, unsigned myIndex) {
	for (unsigned c = 0; c < numOutputs; ++c) {
		m_outputWeights.push_back(Connection());
		m_outputWeights.back().weight = randomWeight();
	}
	m_myIndex = myIndex;
}
double Neuron::transferFunction(double x) {
	//tanh - output range [-1...1]
	return tanh(x);
}
double Neuron::transferFunctionDerivative(double x) {
	//tanh Derivative
	return (1-(x*x));
}
double Neuron::sumDOW(const Layer &nextLayer) const {
	double sum = 0.0;
	for (unsigned n = 0; n < nextLayer.size()-1; ++n) {
		sum += (m_outputWeights[n].weight * nextLayer[n].m_gradient);
	}
	return sum;
}

void Neuron::calcOutputGradients(double targetValue) {
	double delta = targetValue - m_outputValue;
	m_gradient = (delta*Neuron::transferFunctionDerivative(m_outputValue));
}
void Neuron::calcHiddenGradients(const Layer &nextLayer) {
	double dow = sumDOW(nextLayer);
	m_gradient = (dow*Neuron::transferFunctionDerivative(m_outputValue));
}
void Neuron::UpdateInputWeights(Layer &previousLayer) {
	for (unsigned n = 0; n < previousLayer.size() - 1; ++n) {
		Neuron &neuron = previousLayer[n];
		double oldDeltWeight = neuron.m_outputWeights[m_myIndex].deltWeight;
		double newDeltWeight = (eta *neuron.getOutputValue()*m_gradient+alpha*oldDeltWeight);
		neuron.m_outputWeights[m_myIndex].deltWeight = newDeltWeight;
		neuron.m_outputWeights[m_myIndex].weight += newDeltWeight;
	}
}

void Neuron::feedForward(const Layer &previousLayer) {
	double sum = 0.0;
	for (unsigned n = 0; n < previousLayer.size(); ++n) {
		sum += (previousLayer[n].getOutputValue() * previousLayer[n].m_outputWeights[m_myIndex].weight);
	}
	m_outputValue = Neuron::transferFunction(sum);
}

//====================================================================================================//
#pragma endregion

#pragma region Net

class Net {
public:
	Net(const std::vector<unsigned> &topology);
	void feedForward(const std::vector<double> &inputValues);
	void backPropagation(const std::vector<double> &targetValues);
	void getResults(std::vector<double> &resultValues) const;
private:
	std::vector<Layer> m_layers;
	double m_error;
	double m_recentAverageError;
	double m_recentAverageSmoothingFactor;
};

Net::Net(const std::vector<unsigned> &topology) {
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {
		m_layers.push_back(Layer());
		unsigned numOutputs = (layerNum == (topology.size() - 1) ? 0 : topology[layerNum]-1);

		for (unsigned neuronNum = 0; neuronNum < topology[layerNum]; ++neuronNum) {
			m_layers.back().push_back(Neuron(numOutputs, neuronNum));
		}
		m_layers.back().back().setOutputValue(1.0);
	}
}

void Net::feedForward(const std::vector<double> &inputValues) {
	inputValues;
	assert(inputValues.size() == m_layers[0].size()-1);
	//Assign input values into input neurons
	for (unsigned i = 0; i < inputValues.size(); ++i) {
		m_layers[0][i].setOutputValue(inputValues[i]);
	}
	//Forward Propagation
	for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum) {
		//Pointer
		Layer &previousLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size()-1; ++n) {
			m_layers[layerNum][n].feedForward(previousLayer);
		}
	}
}

void Net::backPropagation(const std::vector<double> &targetValues) {
	Layer &outputLayer = m_layers.back();
	m_error = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
		double delta = targetValues[n] - outputLayer[n].getOutputValue();
		m_error += (delta * delta);
	}
	m_error /= (outputLayer.size() - 1);
	m_error = sqrt(m_error);

	m_recentAverageError = ((m_recentAverageError*m_recentAverageSmoothingFactor+ m_error)/ m_recentAverageSmoothingFactor+1.0);


	for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
		outputLayer[n].calcOutputGradients(targetValues[n]);
	}

	for (unsigned layerNum = m_layers.size()-2; layerNum > 0; --layerNum) {
		Layer &hiddenLayer = m_layers[layerNum];
		Layer &nextLayer = m_layers[layerNum + 1];

		for (unsigned n = 0; n < hiddenLayer.size(); ++n) {
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
	}

	for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum) {
		Layer &layer = m_layers[layerNum];
		Layer &previousLayer = m_layers[layerNum - 1];

		for (unsigned n = 0; n < layer.size() - 1; ++n) {
			layer[n].UpdateInputWeights(previousLayer);
		}
	}
}


void Net::getResults(std::vector<double> &resultValues) const {
	resultValues.clear();
	for (unsigned n = 0; n < m_layers.back().size()-1; ++n) {
		resultValues.push_back(m_layers.back()[n].getOutputValue());
	}
}

//====================================================================================================//
#pragma endregion