import std;

using u64 = unsigned long long;
using i64 = long long;

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

struct Opening
{
	int x;
	std::vector<std::pair<int, int>> range;
};

struct Range
{
	int min, max;
	bool Intersect(const Range& r) const { return !(max < r.min || min > r.max); }
	void Merge(const Range r) { min = std::min(min, r.min); max = std::max(max, r.max); }
};

int Solve(std::ifstream& in)
{
	size_t previous = 0;
	std::vector<Opening> openings;
	std::string line;
	while (in >> line)
	{
		auto vals = SplitAt(std::move(line), ",");
		int x = std::stoi(vals[0]), low = std::stoi(vals[1]), width = std::stoi(vals[2]);
		if (x != previous)
		{
			Opening o;
			o.x = x;
			o.range.emplace_back(low, low + width - 1);
			openings.push_back(o);
		}
		else
			openings.back().range.emplace_back(low, low + width - 1);
		previous = x;
	}

	std::vector<Range> step, potential;
	step.emplace_back(0, 0);
	int x = 0;

	for (Opening o : openings)
	{
		int tx = o.x, delta = tx - x;
		for (auto [l, h] : o.range)
		{
			if (tx % 2 != l % 2) ++l;
			if (tx % 2 != h % 2) --h;

			for (Range r : step)
			{
				int up = r.max + delta, down = r.min - delta;
				if (up < l || down > h) continue;
				std::array<int, 4> values = { up , l , down, h };
				std::sort(values.begin(), values.end());
				Range n{ values[1] , values[2] };
				bool found = false;
				for (Range p : potential)
					if (n.Intersect(p))
					{
						found = true;
						p.Merge(n);
						break;
					}
				if (!found)
					potential.push_back(n);
			}
		}
		x = tx;
		std::swap(step, potential);
		potential.clear();
	}

	return (x + step.front().min + 1) / 2;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day19.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	std::string line;
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	std::cout << std::format("Part 1: {}\n", Solve(in1));

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	std::cout << std::format("Part 2: {}\n", Solve(in2));
	
	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	std::cout << std::format("Part 3: {}\n", Solve(in3));
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}