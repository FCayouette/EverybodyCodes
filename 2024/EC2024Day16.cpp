import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()

using i64 = long long;

template <typename T>
class y_combinator {
	T lambda;
public:
	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}
	template <typename...Args>
	constexpr decltype(auto) operator()(Args&&...args) const { return lambda(std::move(*this), std::forward<Args>(args)...); }
};

std::vector<std::string> SplitAt(std::string s, std::string token)
{
	std::vector<std::string> results;
	auto pos = s.find(token), tSize = token.size();
	while (pos != std::string::npos)
	{
		results.push_back(s.substr(0, pos));
		s = s.substr(std::min(pos + tSize, s.length()));
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
		std::cout << "Usage: EC2024Day16.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		std::vector<int> pulls, indices;

		std::string line, part1;
		in1 >> line;
		auto tokens = SplitAt(line, ",");
		for (const auto& t : tokens)
			pulls.push_back(std::stoi(t));
		indices.resize(pulls.size());
		std::vector<std::vector<std::string>> rollers(pulls.size());

		while (std::getline(in1, line))
		{
			if (line.empty())
				continue;
			int pos = 0, index = 0;
			do 
			{
				if (line[pos] != ' ')
					rollers[index].push_back(line.substr(pos, 3));
				pos += 4;
				++index;
			} while (pos < line.size());
		}

		for (int i = 0; i < pulls.size(); ++i)
		{
			part1 += rollers[i][(100 * pulls[i]) % rollers[i].size()];
			part1 += ' ';
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
		std::vector<i64> pulls;
		std::vector<std::vector<std::pair<char, char>>> rollers, reordered;

		std::string line;
		in2 >> line;
		auto tokens = SplitAt(line, ",");
		for (const auto& t : tokens)
			pulls.push_back(std::stoi(t));
		rollers.resize(pulls.size());

		while (std::getline(in2, line))
		{
			if (line.empty())
				continue;
			int pos = 0, index = 0;
			do
			{
				if (line[pos] != ' ')
					rollers[index].emplace_back(line[pos], line[pos+2]);
				pos += 4;
				++index;
			} while (pos < line.size());
		}

		for (int i = 0; i < pulls.size(); ++i)
		{
			std::vector<std::pair<char, char>> work;
			int index = 0;
			do 
			{
				work.push_back(rollers[i][index]);
				index = (index + pulls[i]) % rollers[i].size();
			} while (index != 0);
			reordered.push_back(std::move(work));
		}
		i64 cycle = 1;
		for (const auto& r : reordered)
			cycle = std::lcm(r.size(), cycle);
		
		std::vector<i64> spinValue;
		spinValue.resize(cycle);
		for (int i = 0; i < cycle; ++i)
		{
			std::map<char, i64> count;
			for (int j = 0; j < reordered.size(); ++j)
			{
				auto [a, b] = reordered[j][i % reordered[j].size()];
				++count[a];
				++count[b];
			}
			for (auto iter = count.cbegin(); iter != count.cend(); ++iter)
				spinValue[i] += std::max(0LL, iter->second - 2ll);
		}

		i64 cumulative = std::accumulate(ALLc(spinValue), 0LL), total = 202420242024 / cycle, rem = 202420242024 % cycle;
		std::cout << std::format("Part 2: {}\n", std::accumulate(spinValue.cbegin() + 1, spinValue.cbegin() + rem + 1, cumulative * total));
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		std::vector<int> pulls;
		std::string line;
		in3 >> line;
		for (const auto& t : SplitAt(line, ","))
			pulls.push_back(std::stoi(t));
		
		std::vector<std::vector<std::pair<char, char>>> rollers(pulls.size());

		while (std::getline(in3, line))
		{
			if (line.empty())
				continue;
			int pos = 0, index = 0;
			do
			{
				if (line[pos] != ' ')
					rollers[index].push_back({line[pos], line[pos+2]});
				pos += 4;
				++index;
			} while (pos < line.size());
		}

		std::map<i64, std::pair<int, int>> doneSearches;
		auto part3 = y_combinator([&](auto&& Search, const std::vector<int>& indices, int toGo) -> std::pair<int, int>
			{
				if (!toGo--)
					return { 0, 0 };
				
				i64 searchIndex = 0;
				for (i64 i : indices)
					searchIndex = (searchIndex << 7) | (i & 0xff);
				searchIndex = searchIndex << 10 | toGo;
				if (auto iter = doneSearches.find(searchIndex); iter != doneSearches.cend())
					return iter->second;

				std::pair<int, int> result = {0, 1'000'000};
				std::vector<int> next;
				for (int i = 0; i < indices.size(); ++i)
					next.push_back((indices[i] + pulls[i] - 1) % rollers[i].size());

				for (int i = 0; i < 3; ++i)
				{
					std::map<char, int> count;
					for (int i = 0; i < rollers.size(); ++i)
					{
						auto [a,b] = rollers[i][next[i]];
						++count[a];
						++count[b];
					}
					int cur = 0;
					for (auto iter = count.cbegin(); iter != count.cend(); ++iter)
						cur += std::max(0, iter->second - 2);

					auto best = Search(next, toGo);
					result.first = std::max(result.first, cur + best.first);
					result.second = std::min(result.second, cur + best.second);
					for (int j = 0; j < next.size(); ++j)
						next[j] = (next[j] + 1) % rollers[j].size();
				}
				doneSearches[searchIndex] = result;
				return result;
			})(std::vector<int>(pulls.size(), 0), 256);

		std::cout << std::format("Part 3: {} {}\n", part3.first, part3.second);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}