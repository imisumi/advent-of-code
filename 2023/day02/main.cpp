#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<std::string> readFileLines(const std::string& filename)
{
	std::vector<std::string> lines;
	std::ifstream file(filename);

	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			lines.push_back(line);
		}
		file.close();
	}
	else
	{
		std::cerr << "Unable to open file: " << filename << std::endl;
	}

	return lines;
}

std::vector<std::string> splitString(const std::string &s, const std::string &delimiters)
{
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;

	while ((end = s.find_first_of(delimiters, start)) != std::string::npos)
	{
		if (end != start)
		{
			tokens.push_back(s.substr(start, end - start));
		}
		start = end + 1;
	}
	if (start < s.length()){
		tokens.push_back(s.substr(start));
	}

	return tokens;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "give input file as argument" << std::endl;
		return EXIT_FAILURE;
	}
	std::vector<std::string> fileLines = readFileLines(argv[1]);
	int red = 12;
	int green = 13;
	int blue = 14;

	int part_one = 0;
	int part_two = 0;
	int invalid_game = 0;
	for (int i = 0; i < fileLines.size(); i++)
	{
		int j = 0;
		while (fileLines[i][j] != ':')
			j++;
		fileLines[i].erase(0, j + 2);
		
		int lowest_r = 99999;
		int lowest_g = 99999;
		int lowest_b = 99999;

		int p2_r = 0;
		int p2_g = 0;
		int p2_b = 0;

		invalid_game = 0;
		std::vector<std::string> tokens = splitString(fileLines[i], ";");
		for (int l = 0; l < tokens.size(); l++)
		{
			std::vector<std::string> tokens_2 = splitString(tokens[l], ",");
			lowest_r = 0;
			lowest_g = 0;
			lowest_b = 0;

			for (int k = 0; k < tokens_2.size(); k++)
			{
				int p = 0;
				while (tokens_2[k][p] != 'r' && tokens_2[k][p] != 'g' && tokens_2[k][p] != 'b')
					p++;
				int convert = std::stoi(tokens_2[k]);
				if (tokens_2[k][p] == 'r')
				{
					if (convert > red)
						invalid_game = 1;
					if (convert > lowest_r)
						lowest_r = convert;
				}
				else if (tokens_2[k][p] == 'g')
				{
					if (convert > green)
						invalid_game = 1;
					if (convert > lowest_g)
						lowest_g = convert;
				}
				else if (tokens_2[k][p] == 'b')
				{
					if (convert > blue)
						invalid_game = 1;
					if (convert > lowest_b)
						lowest_b = convert;
				}
			}
			if (lowest_r > p2_r)
				p2_r = lowest_r;
			if (lowest_g > p2_g)
				p2_g = lowest_g;
			if (lowest_b > p2_b)
				p2_b = lowest_b;
		}
		part_two += p2_r * p2_g * p2_b;
		if (invalid_game == 1)
			continue;
		part_one += i + 1;
	}
	std::cout << std::endl << part_one << std::endl;
	std::cout << part_two << std::endl;
	return EXIT_SUCCESS;
}

