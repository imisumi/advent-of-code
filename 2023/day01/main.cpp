#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define PART_ONE 0

std::string loadFileToString(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	std::ostringstream oss;
	oss << file.rdbuf();
	return oss.str();
}

std::string	generateNumOnlyString(const std::string& content)
{
	std::string numbers;
	for (int i = 0; i < content.length(); i++)
	{
		if (std::isdigit(content[i]) || content[i] == '\n')
			numbers += content[i];
		if (PART_ONE)
			continue;
		else if (strncmp(&content[i], "one", 3) == 0)
		{
			numbers += '1';
			i++;
		}
		else if (strncmp(&content[i], "two", 3) == 0)
		{
			numbers += '2';
			i++;
		}
		else if (strncmp(&content[i], "three", 5) == 0)
		{
			numbers += '3';
			i++;
		}
		else if (strncmp(&content[i], "four", 4) == 0)
		{
			numbers += '4';
			i++;
		}
		else if (strncmp(&content[i], "five", 4) == 0)
		{
			numbers += '5';
			i++;
		}
		else if (strncmp(&content[i], "six", 3) == 0)
		{
			numbers += '6';
			i++;
		}
		else if (strncmp(&content[i], "seven", 5) == 0)
		{
			numbers += '7';
			i++;
		}
		else if (strncmp(&content[i], "eight", 5) == 0)
		{
			numbers += '8';
			i++;
		}
		else if (strncmp(&content[i], "nine", 4) == 0)
		{
			numbers += '9';
			i++;
		}
		else if (strncmp(&content[i], "zero", 4) == 0)
		{
			numbers += '0';
			i++;
		}
	}
	if (numbers[numbers.length() - 1] != '\n')
		numbers += '\n';
	return numbers;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "give input file as argument" << std::endl;
		return 1;
	}
	const std::string filename = argv[1];

	std::string content = loadFileToString(filename);

	std::string numbers = generateNumOnlyString(content);

	int	first = 0;
	int	last = 0;
	for (int i = 0; i < numbers.length(); i++)
	{
		if (i == 0 || numbers[i - 1] == '\n')
			first += numbers[i] - '0';
		if (numbers[i + 1] == '\n')
			last += numbers[i] - '0';
	}
	int sum = first * 10 + last;
	std::cout << "Sum: " << sum << std::endl;
	return 0;
}
