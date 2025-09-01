import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;
using i64 = long long;

template<typename T>
struct Coord
{
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr bool operator >  (const Coord& p) const { if (x > p.x) return true; else if (p.x > x) return false; else return y > p.y; }
	T x = {}, y = {};
};

template <typename T>
class y_combinator {
	T lambda;
public:
	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}
	template <typename...Args>
	constexpr decltype(auto) operator()(Args&&...args) const { return lambda(std::move(*this), std::forward<Args>(args)...); }
};

int GetScore(const std::vector<std::string>& board, const std::string& inst, int slot)
{
	int x = 2*(slot-1), y = 0, i = 0;
	while (y < board.size())
	{
		if (board[y][x] == '*')
		{
			x += (inst[i++] == 'R' ? 1 : -1);
			if (x < 0)
				x = 1;
			else if (x >= board.front().size())
				x = board.front().size() - 2;
		}
		++y;
	}
	int targetSlot = x / 2 + 1;
	return std::max(0, targetSlot*2 - slot);
}


int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025S2Q1.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::string line;
	std::ifstream in1(argv[1]);
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		u64 part1 = 0;
		std::vector<std::string> board;
		while (std::getline(in1, line))
		{
			if (!line.size())
				break;
			board.emplace_back(std::move(line));
		}

		int slot = 1;
		while (in1 >> line)
			part1 += GetScore(board, line, slot++);

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

		std::vector<std::string> board;
		while (std::getline(in2, line))
		{
			if (!line.size())
				break;
			board.emplace_back(std::move(line));
		}

		while (in2 >> line)
		{
			int best = 0;
			for (int slot = 1; slot * 2 - 2 < board.front().size(); slot += 1)
				best = std::max(best, GetScore(board, line, slot));
			part2 += best;
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
		std::vector<std::string> board;
		while (std::getline(in3, line))
		{
			if (!line.size())
				break;
			board.emplace_back(std::move(line));
		}

		std::vector<std::pair<int, Coord<int>>> allTosses;

		int op = 0, worst = 10000000, score = 0;
		while (in3 >> line)
		{
			for (int slot = 1; slot * 2 - 2 < board.front().size(); slot += 1)
				allTosses.emplace_back(GetScore(board, line, slot), Coord{op, slot});
			
			++op;
		}

		// Optimization, allTosses "coordinates" are all ordered
		auto Search = y_combinator([&allTosses, &worst](auto&& Search, int curScore, int lastOp, std::vector<int>& usedSlot) -> void
		{
			if (usedSlot.size() == 6)
				worst = std::min(worst, curScore);
			
			if (curScore >= worst) return;
			for (const auto& [v, p] : allTosses)
				if (p.x == lastOp + 1 && std::find(ALLc(usedSlot), p.y) == usedSlot.cend())
				{
					usedSlot.push_back(p.y);
					Search(curScore + v, p.x, usedSlot);
					usedSlot.pop_back();
				}
		});

		for (int i = 0; i < allTosses.size(); ++i)
			if (!allTosses[i].second.x)
			{
				int score = allTosses[i].first;
				std::vector<int> usedSlot = { allTosses[i].second.y };
				Search(score, 0, usedSlot);
			}

		// Greedy search
		std::sort(ALL(allTosses), std::greater());
		std::vector<int> usedSlot, usedOps;
		for (const auto [v, p] : allTosses)
		{
			if (std::find(ALLc(usedOps), p.x) == usedOps.cend() && std::find(ALLc(usedSlot), p.y) == usedSlot.cend())
			{
				score += v;
				usedOps.push_back(p.x);
				usedSlot.push_back(p.y);
			}
			if (usedOps.size() == 6)
				break;
		}

		std::cout << std::format("Part 3: {} {}\n", worst, score);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}