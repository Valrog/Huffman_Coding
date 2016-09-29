// Huffman_Coding_v1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

// ------------------DATA STRUCTURES ------------------

struct Node
{
	Node() : frequency(0), symbol('x'), codeWord(""), left(nullptr), right(nullptr) {} // Default constructor, initializer list
	Node(int val, char symbol, Node* left, Node* right); // Parameterized constructor
	//~Node();
	//void remove(Node* temp);

	int frequency; // Frequency for leaves, sum of the subtree for other vertices
	char symbol;
	std::string codeWord;
	Node* left;
	Node* right;
};

// ------------------ NODE METHODS ------------------

Node::Node(int val, char symbol, Node* left, Node* right) // Parameterized constructor
{
	frequency = val;
	this->symbol = symbol;
	codeWord = "";
	this->left = left;
	this->right = right;
}

// ------------------ FUNCTION PROTOTYPES ------------------

void ReadFile(std::string fname, std::vector<Node> &nodeVector); // Reads the file and creates a vector of nodes with their frequencies
void VectorOutput(std::vector<Node> nodeTree); // Prints out the tree vector
void HuffmanCoding(std::vector<Node> &huffTree); // Constructs the Huffman tree
Node min(std::vector<Node> &nodeVector); // Returns the smallest node in the vector and deletes it
void CodeWord(Node *root); // Determines the code word for every leaf in the Huffman tree, In-Order traversal (Left-Parent-Right)
void NodeOutput(const Node *vertex); // Prints out the node
void FindNode(const Node *root, char x, std::string &koda); // Searches for a node with a given char, returns its code word
void encodeFile(Node *root); // Writes encoded characters into a file
void decodeFile(Node *root); // Writes decoded characters into a file


int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Node> table;


	ReadFile("Test_File.txt", table);
	HuffmanCoding(table);

	std::cout << "\n\nFinished.";

	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignores everything until it encounters newline or fills the stream
	std::cin.get();
	return 0;
}


// ----------------- READ FILE -----------------

void ReadFile(std::string fname, std::vector<Node> &nodeVector)
{

	std::ifstream dat(fname); // Puts the file on stream for reading
	char c; // Needed for reading from a file

	if (dat.is_open()) // Checks if a file could be opened
	{
		if (dat.bad())
		{
			std::cout << "\nProblem with reading from file." << std::endl;
			dat.close();
			return;
		}

		bool added = false; // Sets added to false, for character checking

		while (dat.get(c))
		{
			if (dat.good())
			{
				for (size_t i = 0; i < nodeVector.size(); i++) // Goes through the entire vector
				{
					if (nodeVector[i].symbol == c) // Checks if the character is already in the vector
					{
						nodeVector[i].frequency++; // Increments the frequency of the character in a file
						added = true; // Character has been added before, no need for a new node
						break; // No need to check the remaining characters
					}
				}

				if (added == false) // Character c has not yet been added
				{
					Node newNode(1, c, nullptr, nullptr); // Creates a new node with the character, frequency is 1
					nodeVector.push_back(newNode); // Adds the new character to the tree vector
				}
			}

			else
			{
				std::cout << "\nProblem with reading from file." << std::endl;
				dat.close();
				return;
			}

			added = false; // Resets "added" for an examination of the next character
		}
	}

	else
		std::cout << "\nFile could not be opened." << std::endl;

	dat.close();

	std::cout << "\n\tVector output.\n" << std::endl; // Helpful output
	VectorOutput(nodeVector);
}

// ----------------- VECTOR OUTPUT -----------------

void VectorOutput(std::vector<Node> vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		//std::cout << "\n" << i << ". tree:" << std::endl;
		std::cout << "Char: " << vec[i].symbol << " / Frequency: " << vec[i].frequency << std::endl;
	}
}

// ----------------- FUNCTION MIN -----------------

Node min(std::vector<Node> &nodeVector)
{
	int min = INT_MAX;
	std::vector<Node>::iterator it;
	std::vector<Node>::iterator ptr;

	for (it = nodeVector.begin(); it != nodeVector.end(); it++) // Finds a node with the smallest frequency
	{
		if (it->frequency < min)
		{
			ptr = it; // Points to currently the smallest
			min = it->frequency; // Sets a new lower boundary for the smallest
		}
	}

	Node smallest = *ptr;
	nodeVector.erase(ptr); // Removes this node from the vector

	return smallest; // Returns a node with the smallest frequency
}

// ----------------- NODE OUTPUT -----------------

void NodeOutput(const Node *node)
{
	std::cout << "\n\tVertex output." << std::endl;
	std::cout << "Character: " << node->symbol
		<< " / Frequency: " << node->frequency
		<< " / Code word: " << node->codeWord.c_str() << std::endl;
}

// ----------------- FIND NODE -----------------

void FindNode(const Node *root, char x, std::string &code)
{
	if (root == nullptr) // Base case for termination
		return;

	else
	{
		if (root->symbol == x)
		{
			code = root->codeWord;
		}

		FindNode(root->left, x, code);
		FindNode(root->right, x, code);
	}

	return;
}


// ----------------- HUFFMAN'S ALGORITHM -----------------

void HuffmanCoding(std::vector<Node> &nodeVector)
{
	// Establishing a tree from the vector of nodes

	while (nodeVector.size() > 1) // While the vector contains multiple trees
	{

		Node *right = new Node;
		Node *left = new Node;

		*right = min(nodeVector); // right points to the smallest element in the node vector
		*left = min(nodeVector); // left points to the second smallest element in the node vector (min erases an element)

		Node *newNode = new Node(left->frequency + right->frequency, 'x', left, right); // left = left child, right = right child

		nodeVector.push_back(*newNode); // Pushes back a tree to the node vector


		std::cout << "\nCurrent condition of the vector: " << std::endl;
		VectorOutput(nodeVector); // Helpful output
		std::cout << "\n----------------------\n";
	}

	// Calculation of code words for every leaf

	Node *mainNode = new Node;
	mainNode = &nodeVector[0]; // Main node, that points to the beginning of the tree

	CodeWord(mainNode); // Works out a code word for everz leaf in a Huffman tree

	// Encoding into a file

	encodeFile(mainNode);

	// Decoding into a separate file

	decodeFile(mainNode);

	//izpisVektor(vektor); // Helpful output
}

// ----------------- ENCODING INTO A FILE -----------------

void encodeFile(Node* koren)
{
	std::cout << "\n\tEncoding initial file into a decoded one." << std::endl;

	std::ifstream read("Test_file.txt"); // Puts the file on a stream for reading
	std::ofstream write("encoded.txt"); // Puts the file on a stream for writing
	char c; // Needed for reading from a file and node search

	if (read.is_open() && write.is_open()) // Checks if the files could be opened
	{
		if (read.bad())
		{
			std::cout << "\nProblem with reading from a file." << std::endl;
			read.close();
			return;
		}

		std::string code = "";

		while (read.get(c))
		{
			if (read.good())
			{
				FindNode(koren, c, code); // Finds a node that contains the character
				write << code.c_str(); // Writes the character's code into a file
			}

			else
			{
				std::cout << "\nProblem with reading from a file." << std::endl;
				read.close();
				return;
			}
		}
	}

	else
		std::cout << "\nFile could not be opened." << std::endl;

	read.close();
	write.close();
}

// ----------------- DECODING INTO A FILE -----------------

void decodeFile(Node* root)
{
	std::cout << "\n\tDecoding an encoded file into a decoded one." << std::endl;

	std::ifstream read("encoded.txt"); // Puts the file on a stream for reading
	std::ofstream write("decoded.txt"); // Puts the file on a stream for writing
	char c; // Za branje iz datoteke

	if (read.is_open() && write.is_open()) // Checks if the files could be opened
	{
		if (read.bad())
		{
			std::cout << "\nProblem with reading from a file." << std::endl;
			read.close();
			return;
		}

		Node *temp = new Node; // Temporary node for traversal
		temp = root;

		while (read.get(c))
		{
			if (read.good())
			{
				if (root->left != nullptr && root->right != nullptr) // If this isn't a leaf
				{
					if (c == '0') // If the read character is a '0'
					{
						root = root->left; // Visits left child

						if (root->left == nullptr && root->right == nullptr) // If the next step is a leaf
						{
							write << root->symbol; // Writes a symbol into a file
							root = temp; // Returns to the top of the tree
						}
					}

					// Problem arises while it's checking a leaf, he read a new char and we are losing information

					else // If not, he read 1
					{
						root = root->right; // Visits right child

						if (root->left == nullptr && root->right == nullptr) // If the next step is a leaf
						{
							write << root->symbol; // Writes a symbol into a file
							root = temp; // Returns to the top of the tree
						}
					}
				}

				else // We're in a leaf
				{
					write << root->symbol; // Writes a symbol into a file
					root = temp; // Returns to the top of the tree
				}
			}

			else
			{
				std::cout << "\nProblem with reading from a file." << std::endl;
				read.close();
				return;
			}
		}
	}

	else
		std::cout << "\nFile could not be opened." << std::endl;

	read.close();
	write.close();

}

// ----------------- CODE WORD -----------------

void CodeWord(Node *root)
{
	if (root == nullptr) // Base case for termination
		return;
	else
	{
		if (root->left != nullptr)
			root->left->codeWord = root->codeWord + "0"; // If a left subtree exists, it adds '0' to the code word of the left child

		if (root->right != nullptr)
			root->right->codeWord = root->codeWord + "1"; // If a right subtree exists, it adds '1' to the code word of the right child

		if (root->left == nullptr && root->right == nullptr) // If it's in a leaf
			NodeOutput(root); // Prints out the node


		CodeWord(root->left); // Recursion call
		CodeWord(root->right); // Recursion call
	}
}