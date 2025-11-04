import std.core;

[[nodiscard]] std::vector<std::string> SplitAt(std::string s, std::string token)
{
	std::vector<std::string> results;
	if (s.empty()) [[unlikely]]
		return results;
	
	size_t tokenStart = 0;
	do 
	{
		size_t e = s.find(token, tokenStart);
		if (e == std::string::npos)
		{
			results.emplace_back(s.substr(tokenStart));
			break;
		}
		results.emplace_back(s.substr(tokenStart, e - tokenStart));
		tokenStart = e + token.size();
	} while (tokenStart < s.size());

	return results;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day01.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	std::string line;
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{	
		in1 >> line;
		std::vector<std::string> names = SplitAt(line, ",");
		in1 >> line;
		int index = 0;
		for (const auto& s : SplitAt(line, ","))
		{
			int delta = std::stoi(s.substr(1));
			index = (s[0] == 'L') ? std::max(0, index - delta) : std::min((int)names.size()-1, index + delta);
		}
		std::cout << std::format("Part 1: {}\n", names[index]);
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		in2 >> line;
		std::vector<std::string> names = SplitAt(line, ",");
		in2 >> line;
		int index = 0;
		for (const auto& s : SplitAt(line, ","))
		{
			int delta = std::stoi(s.substr(1));
			index = ((s[0] == 'L') ? (index + (names.size() - delta)) : (index + delta)) % names.size();
		}
		std::cout << std::format("Part 2: {}\n", names[index]);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		in3 >> line;
		std::vector<std::string> names = SplitAt(line, ",");
		in3 >> line;
		for (const auto& s : SplitAt(line, ","))
		{
			int delta = std::stoi(s.substr(1)) % names.size(), index = (s[0] == 'L') ? names.size() - delta : delta;
			std::swap(names[0], names[index]);
		}
		std::cout << std::format("Part 3: {}\n", names[0]);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}
