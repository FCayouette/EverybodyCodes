import std;

using u64 = unsigned long long;
using i64 = long long;

[[nodiscard]] std::vector<int> SplitAt(std::string s, char token)
{
	std::vector<int> results;
	size_t tokenStart = 0;
	do
	{
		size_t e = s.find(token, tokenStart);
		if (e == std::string::npos)
		{
			results.emplace_back(std::stoi(s.substr(tokenStart)));
			break;
		}
		results.emplace_back(std::stoi(s.substr(tokenStart, e - tokenStart)));
		tokenStart = e + 1;
	} while (tokenStart < s.size());

	return results;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day16.exe Part1Filename Part2Filename Part3Filename\n";
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
		u64 part1 = 0;
		in1 >> line;
		for (int v : SplitAt(std::move(line), ','))
			part1 += 90 / v;
		
		std::cout << std::format("Part 1: {}\n", part1);
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		u64 part2 = 1;
		in2 >> line;
		std::vector<int> totals = SplitAt(std::move(line), ',');

		for (int i = 0; i < totals.size(); ++i)
			if (totals[i])
			{
				int factor = i + 1, count = totals[i];
				part2 *= factor * count;
				for (int j = i; j < totals.size(); j += factor)
					totals[j] -= count;
			}
		std::cout << std::format("Part 2: {}\n", part2);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		in3 >> line;
		std::vector<int> totals = SplitAt(std::move(line), ','), factors;

		for (int i = 0; i < totals.size(); ++i)
			if (--totals[i] > 0)
			{
				int factor = i + 1, count = totals[i];
				for (int j = 0; j < count; ++j)
					factors.push_back(factor);
				for (int j = i; j < totals.size(); j += factor)
					totals[j] -= count;
			}
		
		i64 target = 202520252025000, low = 1, high = target, part3 = high / 2;
		while (part3 > low)
		{
			i64 used = target-part3, total = 0;
			for (int factor : factors)
				used -= part3 / factor;

			if (used == 0)
				break;
			if (used > 0)
				low = part3;
			else
				high = part3;
			part3 = (low + high) / 2;
		}

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}