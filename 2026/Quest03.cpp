import std;

using i64 = long long;

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

i64 POW(i64 v, i64 power)
{
	i64 result = 1, i = 0;
	while (i++ < power)
		result *= v;

	return result;
}

i64 Convert(const std::string& base, const std::string& val)
{
	std::string iso = base;
	if (val.size() < base.size())
		iso = base.substr(base.size() - val.size());
	if (base.size() < val.size())
		for (size_t i = 0; i < val.size() - base.size(); ++i)
			iso = base[base.size() - 1 - (i % base.size())] + iso;
	i64 value = 0;
	for (i64 index = iso.size() - 1, i = 0; index >= 0; --index, ++i)
		value += POW(iso[index] - '0', i) * (val[index] - '0');
	return value;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: Quest03.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::string line;

	{
		std::ifstream in(argv[1]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
			return -1;
		}
		i64 part1 = 0;
		while (std::getline(in, line))
		{
			auto [iso, val] = SplitAtFirst(line.substr(4), " ");
			i64 value = 0;
			for (i64 index = iso.size() - 1, i = 0; index >= 0; --index, ++i)
				value += POW(iso[index] - '0', i) * (val[index] - '0');
			part1 += value;
		}
		std::cout << std::format("Part 1: {}\n", part1);
	}

	{
		std::ifstream in(argv[2]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
			return -1;
		}
		i64 part2 = 0;
		while (std::getline(in, line))
		{
			auto [base, val] = SplitAtFirst(line.substr(4), " ");
			part2 += Convert(base, val);
		}
		std::cout << std::format("Part 2: {}\n", part2);
	}

	{
		std::ifstream in(argv[3]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
			return -1;
		}
		i64 part3 = 0;
		while (std::getline(in, line))
		{
			auto [base, val] = SplitAtFirst(line.substr(5), " ");
			i64 value = Convert(base, val);
			bool found = false;
			for (int i = 0, max = stoi(val); i < max && !found; i += 10)
				if (i64 diff = value - Convert(base, std::to_string(i)); diff >= 0 && diff <= 9)
				{
					found = std::to_string(i + diff) != val;
					break;
				}
			if (!found)
				part3 += value;
		}
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}