import std.core;

using u64 = unsigned long long;
using i64 = long long;

std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

std::vector<std::string> SplitAt(std::string s, std::string token)
{
	std::vector<std::string> results;
	auto pos = s.find(token), tSize = token.size();
	while (pos != std::string::npos)
	{
		results.push_back(s.substr(0, pos));
		s = s.substr(std::min(pos+tSize, s.length()));
		pos = s.find(token);
	}
	if (!s.empty())
		results.push_back(std::move(s));
	return results;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();

	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day11.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		u64 part1 = 0;
		std::map<std::string, std::vector<std::string>> rules;
		std::string s;
		while (in1 >> s)
		{
			auto [name, vals] = SplitAtFirst(s, ":");
			rules[name] = SplitAt(vals, ",");
		}

		std::map<std::string, i64> counts, next;
		counts["A"] = 1;

		for (int i = 0; i < 4; ++i)
		{
			for (auto iter : counts)
			{
				for (const auto& to : rules[iter.first])
					next[to] += iter.second;
			}
			std::swap(counts, next);
			next.clear();
		}

		for (const auto& i : counts)
			part1 += i.second;

		std::cout << std::format("Part 1: {}\n", part1);
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		u64 part2 = 0;
		std::map<std::string, std::vector<std::string>> rules;
		std::string s;
		while (in2 >> s)
		{
			auto [name, vals] = SplitAtFirst(s, ":");
			rules[name] = SplitAt(vals, ",");
		}

		std::map<std::string, i64> counts, next;
		counts["Z"] = 1;

		for (int i = 0; i < 10; ++i)
		{
			for (auto iter : counts)
				for (const auto& to : rules[iter.first])
					next[to] += iter.second;
			std::swap(counts, next);
			next.clear();
		}

		for (const auto& i : counts)
			part2 += i.second;

		std::cout << std::format("Part 2: {}\n", part2);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		std::map<std::string, std::vector<std::string>> rules;
		std::string s;
		while (in3 >> s)
		{
			auto [name, vals] = SplitAtFirst(s, ":");
			rules[name] = SplitAt(vals, ",");
		}

		std::map<std::string, i64> counts, next;
		i64 min = std::numeric_limits<long long>::max(), max = 0;

		for (const auto& r : rules)
		{
			counts.clear();
			counts[r.first] = 1;
			for (int i = 0; i < 20; ++i)
			{
				for (auto iter : counts)
					for (const auto& to : rules[iter.first])
						next[to] += iter.second;
				std::swap(counts, next);
				next.clear();
			}
			i64 count = 0;
			for (const auto& i : counts)
				count += i.second;

			min = std::min(min, count);
			max = std::max(max, count);
		}
		std::cout << std::format("Part 3: {}\n", max-min);
	}

	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}