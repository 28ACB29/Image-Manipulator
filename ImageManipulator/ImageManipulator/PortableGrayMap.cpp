#include "PortableGrayMap.h"

namespace ImageManipulator
{
	int PortableGrayMap::decodePixel(int min, int max, int intervals, int bin)
	{

		//Calculate the interval length
		int interval = (max - min) / intervals;

		//Calculate the reconstruction value
		int decodedPixel = min + ((2 * bin - 1) * interval) / 2;
		return (decodedPixel);
	}

	int PortableGrayMap::encodePixel(int min, int max, int intervals, int value)
	{

		//Calculate the interval length
		int interval = (max - min) / intervals;

		//Start at the second lowest interval
		int encodedPixel = 1;

		//Find the lowest upper bound for the high end
		while (min + encodedPixel * interval < value)
		{
			encodedPixel++;
		}
		return (encodedPixel);
	}

	int PortableGrayMap::quantizePixel(int min, int max, int intervals, int value)
	{

		//		//Calculate the interval length
		//		int interval = (max - min) / intervals;
		//
		//		//Start at the high end of the lowest interval
		//		int quantizedPixel = min + interval;
		//
		//		//Find the lowest upper bound for the high end
		//		while(quantizedPixel < value)
		//		{
		//			quantizedPixel = quantizedPixel + interval;
		//		}
		//
		//		//Put the value in the middle of the interval
		//		quantizedPixel = quantizedPixel - interval / 2;
		int quantizedPixel = this->decodePixel(min, max, intervals, this->encodePixel(min, max, intervals, value));
		return (quantizedPixel);
	}

	//Constructors
	//Default Constructor
	PortableGrayMap::PortableGrayMap()
	{

		//Assign a null value to data
		this->data = 0;
		this->width = 0;
		this->height = 0;
		this->maxValue = 0;
	}

	//Copy Constructor
	PortableGrayMap::PortableGrayMap(const PortableGrayMap& image)
	{
		this->width = image.width;
		this->height = image.height;
		this->maxValue = image.maxValue;

		//Create the data as an array of rows
		this->data = new int*[this->height];

		//Copy data element by element
		for (int i = 0; i < this->height; ++i)
		{

			//Create a row
			this->data[i] = new int[this->width];
			for (int j = 0; j < this->width; ++j)
			{
				this->data[i][j] = image.data[i][j];
			}
		}
	}

	//Regular Constructor
	PortableGrayMap::PortableGrayMap(int** data, int width, int height, int maxValue)
	{
		this->width = width;
		this->height = height;
		this->maxValue = maxValue;

		//Create the data as an array of rows
		this->data = new int*[this->height];

		//Copy data element by element
		for (int i = 0; i < this->height; ++i)
		{

			//Create a row
			this->data[i] = new int[this->width];
			for (int j = 0; j < this->width; ++j)
			{
				this->data[i][j] = data[i][j];
			}
		}
	}
	//End Constructors

	//Destructor
	PortableGrayMap::~PortableGrayMap()
	{

		//Delete the rows
		for (int i = 0; i < this->height; ++i)
		{
			delete[] this->data[i];

			//Assign a null value to the row
			this->data[i] = 0;
		}

		//Delete the array of rows
		delete[] this->data;

		//Assign a null value to data
		this->data = 0;
	}

	//Getters
	int** PortableGrayMap::getData()
	{

		//Create the data as an array of rows
		int** data = new int*[this->height];

		//Copy data element by element
		for (int i = 0; i < this->height; ++i)
		{

			//Create a row
			data[i] = new int[this->width];
			for (int j = 0; j < this->width; ++j)
			{
				data[i][j] = this->data[i][j];
			}
		}
		return (data);
	}

	int PortableGrayMap::getHeight()
	{
		return (this->height);
	}

	int PortableGrayMap::getMaxValue()
	{
		return (this->maxValue);
	}

	int PortableGrayMap::getWidth()
	{
		return (this->width);
	}
	//End Getters

	//Equals Operator
	PortableGrayMap& PortableGrayMap::operator=(const PortableGrayMap& rhs)
	{

		//Check  if the dimensions are different
		if (this->height != rhs.height || this->width != rhs.width)
		{

			//Delete the old data
			for (int i = 0; i < this->height; ++i)
			{
				delete[] this->data[i];

				//Assign a null value to the row
				this->data[i] = 0;
			}
			delete[] this->data;

			//Create the data as an array of rows
			this->data = new int*[rhs.height];

			for (int i = 0; i < rhs.height; ++i)
			{

				//Create a row
				data[i] = new int[rhs.width];
			}
		}
		this->height = rhs.height;
		this->maxValue = rhs.maxValue;
		this->width = rhs.width;

		//Copy data element by element
		for (int i = 0; i < this->height; ++i)
		{
			for (int j = 0; j < this->width; ++j)
			{
				this->data[i][j] = rhs.data[i][j];
			}
		}
		return (*this);
	}

	//Image Statistics
	double PortableGrayMap::imageBitRate(int intervals)
	{
		double imageTotalBits = 0.0;
		double imageBitRate = 0.0;

		//Accumulate total bits
		for (int i = 0; i < this->height; ++i)
		{
			for (int j = 0; j < this->width; ++j)
			{
				int encodedValue = this->encodePixel(0, this->maxValue, intervals, this->data[i][j]);
				imageTotalBits +=
					encodedValue != 0 ? ceil(log2(encodedValue)) : 1;
			}
		}

		//Calculate bit rate from total bits, width, and height
		imageBitRate = (double)imageTotalBits / (double)(this->width * this->height);
		return(imageBitRate);
	}

	double PortableGrayMap::imageDistortion(PortableGrayMap image)
	{
		int error = 0;
		double totalDistortion = 0.0;
		double imageDistortion = -1.0;

		//Check  if the dimensions are the same
		if (this->height == image.height || this->width == image.width)
		{
			imageDistortion = 0.0;

			//Accumulate distortion
			for (int i = 0; i < this->height; ++i)
			{
				for (int j = 0; j < this->width; ++j)
				{

					//Calculate error
					error = this->data[i][j] - image.data[i][j];
					totalDistortion += (double)(error * error);
				}
			}

			//Calculate distortion from total, width, and height
			imageDistortion = totalDistortion / (double)(this->width * this->height);
		}
		return (imageDistortion);
	}

	PortableGrayMap PortableGrayMap::imageError(PortableGrayMap image)
	{
		PortableGrayMap errorImage;

		//Check  if the dimensions are the same
		if (this->height == image.height || this->width == image.width)
		{
			errorImage = PortableGrayMap(this->data, this->width, this->height, 0);

			//Accumulate distortion
			for (int i = 0; i < this->height; ++i)
			{
				for (int j = 0; j < this->width; ++j)
				{

					//Calculate absolute error
					errorImage.data[i][j] = abs(this->data[i][j] - image.data[i][j]);

					//Check if this pixel value is the maximum value
					if (errorImage.data[i][j] > errorImage.maxValue)
					{
						errorImage.maxValue = errorImage.data[i][j];
					}
				}
			}
		}
		return (errorImage);
	}
	double PortableGrayMap::imageMean()
	{
		int imageTotal = 0;
		double imageMean = 0.0;

		//Accumulate total
		for (int i = 0; i < this->height; ++i)
		{
			for (int j = 0; j < this->width; ++j)
			{
				imageTotal += this->data[i][j];
			}
		}

		//Calculate mean from total, width, and height
		imageMean = (double)imageTotal / (double)(this->width * this->height);
		return (imageMean);
	}

	double PortableGrayMap::imageVariance()
	{
		double imageMean = this->imageMean();
		double deviation = 0.0;
		double imageMoment = 0.0;
		double imageVariance = 0.0;

		//Accumulate second moment
		for (int i = 0; i < this->height; ++i)
		{
			for (int j = 0; j < this->width; ++j)
			{

				//Calculate deviation
				deviation = ((double) this->data[i][j] - imageMean);
				imageMoment += deviation * deviation;
			}
		}

		//Calculate variance from total, width, and height
		imageVariance = imageMoment / (double)(this->width * this->height);
		return (imageVariance);
	}
	//End Image Statistics

	//Image Combiners
	//Combine two images, left and right, horizontally and return the result
	PortableGrayMap PortableGrayMap::combineImageHorizontally(PortableGrayMap left, PortableGrayMap right)
	{

		//Create a default image
		PortableGrayMap fullImage = PortableGrayMap();

		//Check left and right heights for equality
		if (left.getHeight() == right.getHeight())
		{
			int leftWidth = left.getWidth();
			int rightWidth = right.getWidth();
			int fullWidth = leftWidth + rightWidth;
			int height = left.getHeight();
			int maxValue =
				left.getMaxValue() > right.getMaxValue() ? left.getMaxValue() : right.getMaxValue();

			//Create the combined data as an array of rows
			int** data = new int*[height];
			for (int i = 0; i < height; ++i)
			{

				//Create a row at full width
				data[i] = new int[fullWidth];

				//Copy data element by element from the left image
				for (int j = 0; j < leftWidth; ++j)
				{
					data[i][j] = left.getData()[i][j];
				}

				//Copy data element by element from the right image
				for (int j = 0; j < rightWidth; ++j)
				{
					data[i][j + leftWidth] = right.getData()[i][j];
				}
			}

			//Create a new combined image
			fullImage = PortableGrayMap(data, fullWidth, height, maxValue);
		}
		return (fullImage);
	}

	//Combine two images, top and bottom, vertically and return the result
	PortableGrayMap PortableGrayMap::combineImageVertically(PortableGrayMap top, PortableGrayMap bottom)
	{

		//Create a default image
		PortableGrayMap fullImage = PortableGrayMap();

		//Check top and bottom widths for equality
		if (top.getWidth() == bottom.getWidth())
		{
			int topHeight = top.getHeight();
			int bottomHeight = bottom.getHeight();
			int fullHeight = topHeight + bottomHeight;
			int width = top.getWidth();
			int maxValue =
				top.getMaxValue() > bottom.getMaxValue() ? top.getMaxValue() : bottom.getMaxValue();

			//Create the combined data as an array of rows at full height
			int** data = new int*[fullHeight];
			for (int i = 0; i < topHeight; ++i)
			{

				//Create a row
				data[i] = new int[width];

				//Copy data element by element from the top image
				for (int j = 0; j < topHeight; ++j)
				{
					data[i][j] = top.getData()[i][j];
				}
			}
			for (int i = 0; i < bottomHeight; ++i)
			{

				//Create a row
				data[i] = new int[width];

				//Copy data element by element from the bottom image
				for (int j = 0; j < bottomHeight; ++j)
				{
					data[i + topHeight][j] = bottom.getData()[i][j];
				}
			}

			//Create a new combined image
			fullImage = PortableGrayMap(data, width, fullHeight, maxValue);
		}
		return (fullImage);
	}
	//End Image Combiners

	//Image Filler
	//Fills the image and set the max value with fillValue
	void PortableGrayMap::fillImage(int fillValue)
	{
		for (int i = 0; i < this->height; ++i)
		{
			for (int j = 0; j < this->width; ++j)
			{
				this->data[i][j] = fillValue;
			}
		}
	}

	//Image Splitters
	//Split image horizontally into two images, left and right
	std::vector<PortableGrayMap> PortableGrayMap::splitImageHorizontally()
	{
		int halfWidth = this->width / 2;

		//Create the data as an array of rows
		int** left = new int*[this->height];
		for (int i = 0; i < this->height; ++i)
		{

			//Create a row
			left[i] = new int[halfWidth];

			//Copy data element by element to the left image
			for (int j = 0; j < halfWidth; ++j)
			{
				left[i][j] = data[i][j];
			}
		}

		//Create the data as an array of rows
		int** right = new int*[this->height];
		for (int i = 0; i < this->height; ++i)
		{

			//Create a row
			right[i] = new int[halfWidth];

			//Copy data element by element to the right image
			for (int j = 0; j < halfWidth; ++j)
			{
				right[i][j] = data[i][j + halfWidth];
			}
		}

		//Create a new array of images
		std::vector<PortableGrayMap> halves = std::vector<PortableGrayMap>(2);
		halves[0] = PortableGrayMap(left, halfWidth, this->height, this->maxValue);
		halves[1] = PortableGrayMap(right, halfWidth, this->height, this->maxValue);
		return (halves);
	}

	//Split image vertically into two images, top and bottom
	std::vector<PortableGrayMap> PortableGrayMap::splitImageVertically()
	{
		int halfHeight = this->height / 2;

		//Create the data as an array of rows
		int** top = new int*[halfHeight];
		for (int i = 0; i < halfHeight; ++i)
		{

			//Create a row
			top[i] = new int[this->width];

			//Copy data element by element to the top image
			for (int j = 0; j < this->width; ++j)
			{
				top[i][j] = data[i][j];
			}
		}

		//Create the data as an array of rows
		int** bottom = new int*[halfHeight];
		for (int i = 0; i < halfHeight; ++i)
		{

			//Create a row
			bottom[i] = new int[this->width];

			//Copy data element by element to the bottom image
			for (int j = 0; j < this->width; ++j)
			{
				bottom[i][j] = data[i + halfHeight][j];
			}
		}

		//Create a new array of images
		std::vector<PortableGrayMap> halves = std::vector<PortableGrayMap>(2);
		halves[0] = PortableGrayMap(top, halfHeight, this->height, this->maxValue);
		halves[1] = PortableGrayMap(bottom, halfHeight, this->height, this->maxValue);
		return (halves);
	}
	//End Image Splitters

	//Image Compressors
	//Compress image given a sub-image variance threshold
	void PortableGrayMap::compressImage(int varianceThreshold)
	{

		//Check if the width is greater than 1
		if (this->width > 1)
		{

			//If the image is more uniform the variance threshold it is filled with the average value
			if (this->imageVariance() <= (double)varianceThreshold)
			{
				this->fillImage(this->imageMean());
			}

			//If the image is less uniform the variance threshold it is split horizontally and recursively compressed
			else if (this->imageVariance() > (double)varianceThreshold)
			{
				this->compressImage(varianceThreshold, 0, this->width / 2);
				this->compressImage(varianceThreshold, this->width / 2, this->width);
			}
		}
	}

	//	//Compress and return image given a sub-image variance threshold
	//	PortableGrayMap PortableGrayMap::compressImage(int varianceThreshold)
	//	{
	//
	//		//Create a default image
	//		PortableGrayMap compressedImage = PortableGrayMap(*this);
	//
	//		//Check if the width is greater than 1
	//		if(this->width > 1)
	//		{
	//
	//			//If the image is more uniform the variance threshold it is filled with the average value
	//			if(this->imageVariance() <= (double) varianceThreshold)
	//			{
	//				compressedImage.fillImage(this->imageMean());
	//			}
	//
	//			//If the image is less uniform the variance threshold it is split horizontally and recursively compressed
	//			else if(this->imageVariance() > (double) varianceThreshold)
	//			{
	//				std::vector<PortableGrayMap> halves = this->splitImageHorizontally();
	//				PortableGrayMap left = this->compressImage(halves[0], varianceThreshold);
	//				PortableGrayMap right = this->compressImage(halves[1], varianceThreshold);
	//				compressedImage = this->combineImageHorizontally(left, right);
	//			}
	//		}
	//		return (compressedImage);
	//	}

	//Compress image given a sub-image variance threshold and a start and end column index
	void PortableGrayMap::compressImage(int varianceThreshold, int start, int end)
	{
		int width = end - start;
		int imageTotal = 0;
		double imageMean = 0.0;
		double deviation = 0.0;
		double imageMoment = 0.0;
		double imageVariance = 0.0;
		if (width > 1)
		{

			//Accumulate total
			for (int i = 0; i < this->height; ++i)
			{
				for (int j = start; j < end; ++j)
				{
					imageTotal += this->data[i][j];
				}
			}

			//Calculate mean from total, width, and height
			imageMean = (double)imageTotal / (double)(width * this->height);

			//Accumulate second moment
			for (int i = 0; i < this->height; ++i)
			{
				for (int j = start; j < end; ++j)
				{

					//Calculate deviation
					deviation = ((double) this->data[i][j] - imageMean);
					imageMoment += deviation * deviation;
				}
			}

			//Calculate variance from total, width, and height
			imageVariance = imageMoment / (double)(width * this->height);

			//If the image is more uniform the variance threshold it is filled with the average value
			if (imageVariance <= (double)varianceThreshold)
			{
				for (int i = 0; i < this->height; ++i)
				{
					for (int j = start; j < end; ++j)
					{
						this->data[i][j] = imageMean;
					}
				}
			}

			//If the image is less uniform the variance threshold it is split horizontally and recursively compressed
			else if (imageVariance >(double) varianceThreshold)
			{
				this->compressImage(varianceThreshold, start, start + width / 2);
				this->compressImage(varianceThreshold, start + width / 2, end);
			}
		}
	}

	//Compress image given a sub-image variance threshold and a start and end column index
	void PortableGrayMap::compressImage(int varianceThreshold, int rStart, int rEnd, int cStart, int cEnd)
	{
		int height = rEnd - rStart;
		int width = cEnd - cStart;
		int imageTotal = 0;
		double imageMean = 0.0;
		double deviation = 0.0;
		double imageMoment = 0.0;
		double imageVariance = 0.0;
		if (width > 1 && height > 1)
		{

			//Accumulate total
			for (int i = rStart; i < rEnd; ++i)
			{
				for (int j = cStart; j < cEnd; ++j)
				{
					imageTotal += this->data[i][j];
				}
			}

			//Calculate mean from total, width, and height
			imageMean = (double)imageTotal / (double)(width * height);

			//Accumulate second moment
			for (int i = rStart; i < rEnd; ++i)
			{
				for (int j = cStart; j < cEnd; ++j)
				{

					//Calculate deviation
					deviation = ((double) this->data[i][j] - imageMean);
					imageMoment += deviation * deviation;
				}
			}

			//Calculate variance from total, width, and height
			imageVariance = imageMoment / (double)(width * height - 1);

			//If the image is more uniform the variance threshold it is filled with the average value
			if (imageVariance <= (double)varianceThreshold)
			{
				for (int i = rStart; i < rEnd; ++i)
				{
					for (int j = cStart; j < cEnd; ++j)
					{
						this->data[i][j] = imageMean;
					}
				}
			}

			//If the image is less uniform the variance threshold it is split horizontally and recursively compressed
			else if (imageVariance >(double) varianceThreshold)
			{
				this->compressImage(varianceThreshold, rStart, rStart + height / 2, cStart, cStart + width / 2);
				this->compressImage(varianceThreshold, rStart, rStart + height / 2, cEnd - width / 2, cEnd);
				this->compressImage(varianceThreshold, rEnd - height / 2, rEnd, cStart, cStart + width / 2);
				this->compressImage(varianceThreshold, rEnd - height / 2, rEnd, cEnd - width / 2, cEnd);
			}
		}
	}

	//	//Compress and return image given an image and a sub-image variance threshold
	//	PortableGrayMap PortableGrayMap::compressImage(PortableGrayMap originalImage, int varianceThreshold)
	//	{
	//
	//		//Create a default image
	//		PortableGrayMap compressedImage = PortableGrayMap(originalImage);
	//		if(originalImage.getWidth() > 1)
	//		{
	//
	//			//If the image is more uniform the variance threshold it is filled with the average value
	//			if(originalImage.imageVariance() <= (double) varianceThreshold)
	//			{
	//				compressedImage.fillImage(originalImage.imageMean());
	//			}
	//
	//			//If the image is less uniform the variance threshold it is split horizontally and recursively compressed
	//			else if(originalImage.imageVariance() > (double) varianceThreshold)
	//			{
	//				std::vector<PortableGrayMap> halves = originalImage.splitImageHorizontally();
	//				PortableGrayMap left = originalImage.compressImage(halves[0], varianceThreshold);
	//				PortableGrayMap right = originalImage.compressImage(halves[1], varianceThreshold);
	//				compressedImage = originalImage.combineImageHorizontally(left, right);
	//			}
	//		}
	//		return (compressedImage);
	//	}
	//End Image Compressors

	//Image Quantizer
	PortableGrayMap PortableGrayMap::quantizeImage(int intervals)
	{

		//Create a default image
		PortableGrayMap quantizedImage = PortableGrayMap(*this);
		for (int i = 0; i < this->height; ++i)
		{
			for (int j = 0; j < this->width; ++j)
			{
				quantizedImage.data[i][j] = this->quantizePixel(0, this->maxValue + 1, intervals, this->data[i][j]);
			}
		}
		return (quantizedImage);
	}

	//String Converter
	std::string PortableGrayMap::toString()
	{
		std::stringstream stringStream;

		//Append magic value
		stringStream << "P2" << '\n';

		//Append width
		stringStream << this->width << ' ';

		//Append height
		stringStream << this->height << '\n';

		//Append maximum value
		stringStream << this->maxValue << '\n';

		//Append image data
		int lastColumn = this->width - 1;
		for (int i = 0; i < this->height; ++i)
		{
			for (int j = 0; j < lastColumn; ++j)
			{
				if ((i * this->height + j) % 12 == 11)
				{
					stringStream << this->data[i][j] << '\n';
				}
				else
				{
					stringStream << this->data[i][j] << "  ";
				}
			}
			stringStream << this->data[i][lastColumn] << '\n';
		}
		return (stringStream.str());
	}
}