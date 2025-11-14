import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos) [[unlikely]]
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

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
		std::cout << "Usage: EC2025Day09.exe Part1Filename Part2Filename Part3Filename\n";
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

		std::vector<std::string> individuals;
		while (in1 >> line)
			individuals.push_back(line.substr(2));
		
		for (int x = 0, y = 1, z = 2; x < 3 && !part1; ++x, y = (y+1)%3, z = (z+1)%3)
		{
			bool valid = true;
			u64 a = 0, b = 0;
			for (int i = 0; i < individuals.front().size() && valid; ++i)
			{
				if (char c = individuals[y][i], d = individuals[z][i], e = individuals[x][i];
					c != e && d != e)
					valid = false;
				else
				{
					if (c == e) ++a;
					if (d == e) ++b;
				}
			}
			if (valid)
				part1 = a * b;
		}

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

		std::vector<std::string> individuals;
		while (in2 >> line)
		{
			auto pos = line.find(":");
			individuals.push_back(line.substr(pos + 1));
		}

		const int dnaSize = individuals.front().size();

		for (int i = 0; i < individuals.size(); ++i)
		{
			int count = 0;
			bool found = false;
			for (int j = 0; j < individuals.size() && !found; ++j)
			{
				if (i== j) continue;
				for (int k = 0; k < individuals.size(); ++k)
				{
					bool valid = true;
					if (k== i || k == j) continue;
					int a = 0, b = 0;
					for (int x = 0; x < dnaSize; ++x)
					{
						char toMatch = individuals[i][x];
						if (toMatch != individuals[j][x] && toMatch != individuals[k][x])
						{
							valid = false;
							break;
						}
						if (toMatch == individuals[j][x]) ++a;
						if (toMatch == individuals[k][x]) ++b;
					}
					if (valid)
					{
						part2 += a * b;
						found = true;
						break;
					}
				}
			}
			

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
		u64 part3 = 0;

		std::vector<std::string> individuals;
		while (in3 >> line)
		{
			auto pos = line.find(":");
			individuals.push_back(line.substr(pos + 1));
		}

		const int dnaSize = individuals.front().size();

		std::vector<std::pair<int, int>> lineage;
		std::vector<std::unordered_set<int>> families;
		for (int i = 0; i < individuals.size(); ++i)
		{
			int count = 0;
			bool found = false;
			for (int j = 0; j < individuals.size() && !found; ++j)
			{
				if (i == j) continue;
				for (int k = 0; k < individuals.size(); ++k)
				{
					bool valid = true;
					if (k == i || k == j) continue;
					for (int x = 0; x < dnaSize; ++x)
						if (char toMatch = individuals[i][x]; toMatch != individuals[j][x] && toMatch != individuals[k][x])
						{
							valid = false;
							break;
						}
					if (valid)
					{
						lineage.emplace_back(i, k);
						lineage.emplace_back(j, k);
						found = true;
						break;
					}
				}
			}
		}

		for (auto [a, b] : lineage)
		{
			int indexA = -1;
			int indexB = -1;
			for (int i = 0; i < families.size(); ++i)
			{
				if (families[i].contains(a))
					indexA = i;
				if (families[i].contains(b))
					indexB = i;
			}
			if (indexA == indexB)
			{
				if (indexA == -1)
					// new family
					families.emplace_back(std::unordered_set({a, b}));
				// else do nothing, already known
			}
			else if (indexA == -1)
				families[indexB].insert(a);
			else if (indexB == -1)
				families[indexA].insert(b);
			else
			{
				// merge
				families[indexA].insert(ALLc(families[indexB]));
				families[indexB].clear();
				if (indexB != families.size()-1)
					std::swap(families[indexB], families[families.size()-1]);
				families.pop_back();
			}
		}
		int maxSize = 0;
		for (const auto& fam : families)
			if (fam.size() > maxSize)
			{
				maxSize = fam.size();
				part3 = std::accumulate(ALLc(fam), fam.size());
			}
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}