#include "Learning.hpp"

Learning::Learning(float defStepSize) {
	stepSize = sSize;
}


//Adjust connections to this node to get its value closer to target
//Simple dummy algorithm to get somehow going
void Learning::adjustConnectionSimple(int layer, int index, float targetValue, NeuralNetwork& nn) {

	auto node = nn.nodes[layer][index];
	float oldValue = node->calcValue();
	//float difference = targetValue - currentValue;
	//float smallestDifference = difference;
	
	
}

float Learning::getStepSize() {
	return defaultStepSize;
}

void Learning::adjustNetwork(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget, int mode) {
	if (mode == 2) {
		throw "Learning mode 2 is not yet supported. Be back soon!";
		backPropagationLearning(controller, nn, qvalue, qtarget);
	} else if (mode == 1) {
		racistNetworkLearning(controller, nn, qvalue, qtarget);
	} else if (mode == 0) {
		// do nothing
	} else {
		throw "Learning.cpp: No learning algorithm with mode number " + mode + " found.";
	}
}

void Learning::racistNetworkLearning(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget) {
	int layers = nn.nodes.size();
	racistNodeAdjustment(nn.nodes[layerCount - 1][0], nn, error, controller->prevWeightCoefficient, layers - 1);
}

// Recursively adjust weights
void racistNodeAdjustment(Node& n, NeuralNetwork& nn, float target, float prevWeightCoefficient, int currentLayer) {
	if (currentLayer > 0) {
		float inputWeightTotal = 0.0f;
		for (unsigned int i = 0; i < nn.nodes[currentLayer - 1].size(); i++) {
			inputWeightTotal += n->getWeight(i);
		}

		float error = n->getValue() - target;
		for (unsigned int i = 0; i < nn.nodes[currentLayer - 1].size(); i++) {
			Node& input = std::get<0>(n.getConnectionsIn()[index]);
			int inputValueSign = (input.getValue() > 0) - (input.getValue() < 0);
			// Magic happens. Pseudocode and then real code. Painstakingly implemented a pseudo-backpropagation from scratch. Because I can.
			// prev_weight + stepsize * (sign(in_i.value) * error * (1 + abs(prev_weight) * prevWeightCoefficient))
			float newWeight = n->getWeight(i) + stepSize * (inputValueSign * error * (1 + abs(n->getWeight(i)) * prevWeightCoefficient));
			// prev_value * prevValueCoefficient + (1 - prevValueCoefficient) * (target / prev_weight) / nn.nodes[currentLayer - 1].size()
			float nodeTarget = input.getValue() * prevValueCoefficient + (1 - prevValueCoefficient) * (target / n->getWeight(i)) / nn.nodes[currentLayer - 1].size();
			racistNodeAdjustment(nn.nodes[currentLayer - 1][i], nn, nodeTarget, prevWeightCoefficient, currentLayer - 1);
			n->setWeight(i, newWeight); // TODO: currently no correction for first layers receiving multiple adjustments
		}
	}
}

void backPropagationLearning(Controller& controller, NeuralNetwork& nn, float qvalue, float qtarget) {
	// TODO: Implement more typical BP algorithm if there is extra time.
}