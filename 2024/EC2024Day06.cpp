import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;
using i64 = long long;

struct Node
{
	std::string parent = "";
};

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day06.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	auto Name = [](std::ifstream& in, bool shortName) -> std::string
	{
		std::map<std::string, std::string> tree; // Node to Parent
		std::map<std::string, int> endNodes;

		std::string line;
		while (in >> line)
		{
			auto pos = line.find(':');
			std::string root = line.substr(0, pos);

			if (tree.find(root) == tree.cend())
				tree[root] = "";

			line = line.substr(pos + 1);
			pos = line.find(',');

			while (pos != std::string::npos)
			{
				std::string s = line.substr(0, pos);
				tree[s] = root;
				line = line.substr(pos + 1);
				pos = line.find(',');
			}
			if (line != "@")
				tree[line] = root;
			else
				endNodes[root] = 0;
		}

		std::map<int, int> lengthCount;

		for (auto it = endNodes.begin(); it != endNodes.end(); ++it)
		{
			std::string s = tree[it->first];
			int count = 0;
			while (s != "RR")
			{
				++count;
				s = tree[s];
				if (s == "ANT" || s == "BUG" || s == "")
				{
					count = -1;
					break;
				}
			}
			it->second = count;
			++lengthCount[count];
		}

		int length = 0;
		for (auto it = lengthCount.begin(); it != lengthCount.end(); ++it)
			if (it->second == 1 && it->first >= 0)
			{
				length = it->first;
				break;
			}

		std::string which = "";
		for (auto it = endNodes.begin(); which == ""; ++it)
			if (it->second == length)
				which = it->first;

		std::string result = shortName ? which.substr(0, 1) : which;
		result += '@';
		while (which != "RR")
		{
			which = tree[which];
			result = (shortName ? which.substr(0, 1) : which) + std::move(result);
		}
		return result;
	};
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	std::cout << std::format("Part 1: {}\n", Name(in1, false));
	
	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	std::cout << std::format("Part 2: {}\n", Name(in2, true));

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	std::cout << std::format("Part 3: {}\n", Name(in3, true));
}