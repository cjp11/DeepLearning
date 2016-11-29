#include <vector>
#include "Node.hpp"
#include <tuple>

class NeuralNetwork {
	friend class Learning;
public:
	NeuralNetwork();
	const int getInputSize() const;
	const int getOutputSize() const;
	const int getLayerSize(int& const layer) const;
	const std::vector<double>& getOutputs() const;
	const std::vector<double>& getOutputsFromInputs(std::vector<double>& const values);


	void setInput(int& const index, double& const value);
	void setInput(std::vector<double>& const values);
	void addNode(Node& const node, int& const type);
	void addNode(std::vector<Node>& const nodes, int& const type);

	void connectAll();
	void calcAll();

protected:
	std::vector<std::vector<Node>> nodes;
	std::vector<int> sizes; //size of each Node-vector

private:
};
