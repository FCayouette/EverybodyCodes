import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()

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

std::pair<std::vector<std::string>, std::unordered_map<char, std::vector<char>>> ReadData(std::ifstream& in)
{
	std::string line;
	std::getline(in, line);
	std::vector<std::string> names = SplitAt(std::move(line), ",");
	std::getline(in, line);
	std::unordered_map<char, std::vector<char>> rules;
	while (std::getline(in, line))
		for (size_t i = 4; i < line.size(); i +=2)
			rules[line[0]].push_back(line[i]);
	
	return {std::move(names), std::move(rules)};
}

bool Valid(const std::string& n, const std::unordered_map<char, std::vector<char>>& rules)
{
	for (int i = 0; i < n.size() - 1; ++i)
		if (const auto& r = rules.at(n[i]); std::find(ALLc(r), n[i + 1]) == r.cend())
			return false;
	return true;
}

template <typename T>
class y_combinator {
	T lambda;
public:
	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}
	template <typename...Args>
	constexpr decltype(auto) operator()(Args&&...args) const { return lambda(std::move(*this), std::forward<Args>(args)...); }
};

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day07.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		auto [names, rules] = ReadData(in1);

		for (const std::string& n : names)
		{
			if (Valid(n, rules))
			{
				std::cout << std::format("Part 1: {}\n", n);
				break;
			}
		}		
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		int part2 = 0;
		auto [names, rules] = ReadData(in2);
		for (int i = 0; i < names.size(); ++i)
			if (Valid(names[i], rules))
				part2 += i+1;
			
		std::cout << std::format("Part 2: {}\n", part2);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		int part3 = 0;
		auto [names, rules] = ReadData(in3);

		std::string work;
		work.reserve(12);
		std::unordered_set<std::string> uniques;

		auto GetPrefixes = y_combinator([&rules, &work, &uniques](auto&& GetPrefixes, char start) -> int
		{
			int result = 0;
			if (work.size() < 11)
				for (char c : rules[start])
				{
					work.push_back(c);
					if (uniques.insert(work).second)
					{
						if (work.size() >= 7)
							++result;
						result += GetPrefixes(c);
					}
					work.pop_back();
				}
			return result;
		});

		for (int i = 0; i < names.size(); ++i)
			if (const std::string& n = names[i]; Valid(n, rules))
			{
				work = n;
				part3 += GetPrefixes(n[n.size()-1]);
			}
		
		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}