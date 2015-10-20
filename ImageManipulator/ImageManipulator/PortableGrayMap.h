#include <math.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>

#ifndef PORTABLEGRAYMAP_H
#define PORTABLEGRAYMAP_H

namespace ImageManipulator
{
	class PortableGrayMap
	{
	private:
		int** data;
		int height;
		int maxValue;
		int width;
		int decodePixel(int min, int max, int intervals, int value);
		int encodePixel(int min, int max, int intervals, int value);
		int quantizePixel(int min, int max, int intervals, int value);
	public:

		//Constructors
		//Default Constructor
		PortableGrayMap();

		//Copy Constructor
		PortableGrayMap(const PortableGrayMap& image);

		//Regular Constructor
		PortableGrayMap(int** data, int width, int height, int maxValue);
		//End Constructors

		//Destructor
		virtual ~PortableGrayMap();

		//Getters
		int** getData();
		int getHeight();
		int getMaxValue();
		int getWidth();
		//End Getters

		//Equals Operator
		PortableGrayMap& operator=(const PortableGrayMap& rhs);

		//Image Statistics
		double imageBitRate(int intervals);
		double imageDistortion(PortableGrayMap image);
		PortableGrayMap imageError(PortableGrayMap image);
		double imageMean();
		double imageVariance();
		//End Image Statistics

		//Image Combiners
		//Combine two images, left and right, horizontally and return the result
		PortableGrayMap combineImageHorizontally(PortableGrayMap left, PortableGrayMap right);

		//Combine two images, top and bottom, vertically and return the result
		PortableGrayMap combineImageVertically(PortableGrayMap top, PortableGrayMap bottom);
		//End Image Combiners

		//Image Filler
		//Fills the image and set the max value with fillValue
		void fillImage(int fillValue);

		//Image Splitters
		//Split image horizontally into two images, left and right
		std::vector<PortableGrayMap> splitImageHorizontally();

		//Split image vertically into two images, top and bottom
		std::vector<PortableGrayMap> splitImageVertically();
		//End Image Splitters

		//Image Compressors
		////Compress image given a sub-image variance threshold
		//void compressImage(int varianceThreshold);

		//Compress and return image given a sub-image variance threshold
		PortableGrayMap compressImage(int varianceThreshold);

		//Compress image given a sub-image variance threshold and a start and end column index
		void compressImage(int varianceThreshold, int start, int end);

		//Compress image given a sub-image variance threshold a start and end row index and a start and end column index
		void compressImage(int varianceThreshold, int rStart, int rEnd, int cStart, int cEnd);

		//Compress and return image given an image and a sub-image variance threshold
		PortableGrayMap compressImage(PortableGrayMap image, int varianceThreshold);
		//End Image Compressors

		//Image Quantizer
		PortableGrayMap quantizeImage(int intervals);

		//String Converter
		std::string toString();
	};
}
#endif // !PORTABLEGRAYMAP_H
