//
//    Copyright 2021 Christopher D. McMurrough
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

/*******************************************************************************************************************//**
 * @file program2.cpp
 * @brief Coin sorter program with crude classification of US coins Quarter, Dime, Nickel, and Penny
 * @author Edrik Aguilera
 **********************************************************************************************************************/

// include necessary dependencies
#include <iostream>
#include <cstdio>
#include "coinsortmachine.h"

// configuration parameters
#define NUM_COMNMAND_LINE_ARGUMENTS 1

/*******************************************************************************************************************//**
 * @brief program entry point
 * @param[in] argc number of command line arguments
 * @param[in] argv string array of command line arguments
 * @return return code (0 for normal termination)
 * @author Christoper D. McMurrough
 **********************************************************************************************************************/
int main(int argc, char **argv)
{
    // validate and parse the command line arguments
    if(argc != NUM_COMNMAND_LINE_ARGUMENTS + 1)
    {
        std::printf("USAGE: %s <image_path> \n", argv[0]);
        return 0;
    }
    else
    {
		// Read the file into the original imageIn
		cv::Mat imageIn = cv::imread(argv[1], cv::IMREAD_COLOR);

		// check for file error
		if(!imageIn.data)
		{
			std::cout << "Error while opening file " << argv[1] << std::endl;
			return 0;
		}
		else
		{
			// Pass the input image
			CoinSortMachine sorter{imageIn};

			// Use canny edge detection
			sorter.FindImageEdges();

			// Find contour edges
			sorter.FindImageContours();

			// Create ellipses off of those contours
			sorter.FitEllipsesToCoins();

			// Categorize the ellipses by drawing them on the original image
			// Display to console the monetary total
			sorter.DisplayResults();
		}
    }
}
