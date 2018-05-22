/**
* @class Structs
* @brief File containing all structs
* @author Jack Matters
*
* @date 22/05/2017
* @version 1.0	Initial start.
*/

#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <vector>

/// Struct to hold all of a model types data (positions, scales, filePath to load)
struct ModelsData
{
	std::string filePath;
	std::vector<std::vector<float>> modelPositions;
	std::vector<std::vector<float>> modelScales;
};

/// Struct to hold both vertex and fragment shaders (needs to be moved)
struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

#endif