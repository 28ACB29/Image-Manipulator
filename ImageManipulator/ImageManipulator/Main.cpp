#include <fstream>
#include <iostream>
#include <math.h>
#include "PortableGrayMap.h"
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char *argv[])
{

	//Check argument length
	if (argc != 7)
	{
		std::cout << "wrong number of arguments";
		std::cout << std::endl;
	}
	else
	{
		std::stringstream stringStream;

		//Read command line arguments
		std::string inputName(argv[1]);
		std::string varianceThresholdString(argv[2]);
		std::string intervalString(argv[3]);
		std::string compressedImageName(argv[4]);
		std::string quantizedImageName(argv[5]);
		std::string errorImageName(argv[6]);

		//Create file input from string
		std::ifstream inputFile;
		inputFile.open(inputName.data(), std::ifstream::in);

		//Create variance threshold from string
		int varianceThreshold;
		stringStream.str(varianceThresholdString);
		stringStream >> varianceThreshold;
		stringStream.clear();
		stringStream.str("");

		//Create intervals from string
		int intervals;
		stringStream.str(intervalString);
		stringStream >> intervals;
		stringStream.clear();
		stringStream.str("");

		//Read data from file
		std::string line;
		std::string temp;
		std::vector<std::string> fileData;
		char character;
		bool isDigit;

		//Check if file is not empty
		while (!inputFile.eof())
		{

			//Get each line
			std::getline(inputFile, line, '\n');

			//If line is not a comment and not a start of a magic symbol
			if (line[0] != '#' && line[0] != 'P')
			{
				unsigned int lastIndex = line.length() - 1;

				//Split it up and add the data to the list of tokens
				for (unsigned int index = 0; index < line.length(); ++index)
				{
					character = line[index];

					//Check if character is not whitespace
					if (character != ' ' && character != '\t')
					{

						//It is a digit and is added to the temporary string
						temp.push_back(character);
						isDigit = true;

						//Check if it is the last digit
						if (index == lastIndex)
						{

							//Add it to the list
							fileData.push_back(temp);

							//Clear the string
							temp.clear();

							//Reset the digit indicator
							isDigit = false;
						}
					}

					//Check if the previous character was a digit
					else if (isDigit == true)
					{

						//Add it to the list
						fileData.push_back(temp);

						//Clear the string
						temp.clear();

						//Reset the digit indicator
						isDigit = false;
					}
				}
			}
		}

		//Close input file
		inputFile.close();

		//Create width from string
		int width;
		stringStream.str(fileData[0]);
		stringStream >> width;
		stringStream.clear();
		stringStream.str("");

		//Create height from string
		int height;
		stringStream.str(fileData[1]);
		stringStream >> height;
		stringStream.clear();
		stringStream.str("");

		//Create max value from string
		int maxValue;
		stringStream.str(fileData[2]);
		stringStream >> maxValue;
		stringStream.clear();
		stringStream.str("");

		//Create data from strings
		int dataStart = 3;
		int** data;
		data = new int*[width];
		for (int i = 0; i < width; ++i)
		{
			data[i] = new int[height];
			for (int j = 0; j < height; ++j)
			{
				stringStream.str(fileData[dataStart + i * height + j]);
				stringStream >> data[i][j];
				stringStream.clear();
				stringStream.str("");
			}
		}

		//Create image object from arguments
		ImageManipulator::PortableGrayMap originalImage = ImageManipulator::PortableGrayMap(data, width, height, maxValue);

		//Compress image
		originalImage.compressImage(varianceThreshold);

		//Quantize image
		ImageManipulator::PortableGrayMap quantizedImage = originalImage.quantizeImage(intervals);

		//Generate error image
		ImageManipulator::PortableGrayMap errorImage = originalImage.imageError(quantizedImage);

		//Write to output files
		std::ofstream compressedImageFile;
		compressedImageFile.open(compressedImageName.data(), std::ofstream::out);
		compressedImageFile << originalImage.toString();
		std::ofstream quantizedImageFile;
		quantizedImageFile.open(quantizedImageName.data(), std::ofstream::out);
		quantizedImageFile << quantizedImage.toString();
		std::ofstream errorImageFile;
		errorImageFile.open(errorImageName.data(), std::ofstream::out);
		errorImageFile << errorImage.toString();

		//Close output files
		compressedImageFile.close();
		quantizedImageFile.close();
		errorImageFile.close();
		std::cout << "Quantized Image Distortion: " << originalImage.imageDistortion(quantizedImage) << std::endl;
		std::cout << "Quantized Image Bit Rate: " << quantizedImage.imageBitRate(intervals) << std::endl;
		return (0);
}