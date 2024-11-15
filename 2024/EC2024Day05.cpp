import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;
using i64 = long long;

using Dance = std::vector<std::list<int>>;

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day05.exe Part1Filename Part2Filename Part3Filename\n";
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
		std::string s;
		std::getline(in1, s);
		std::stringstream ss(s);
		Dance dance;
		int v;
		while (ss >> v)
			dance.push_back(std::list<int>{v});

		while (std::getline(in1, s))
		{
			std::stringstream ss(s);
			for (int i = 0; i < dance.size(); ++i)
			{
				ss >> v;
				dance[i].push_back(v);
			}
		}

		for (int step = 0; step < 10; ++step)
		{
			int v = dance[step%dance.size()].front();
			dance[step%dance.size()].pop_front();

			int next = (step+1) % dance.size();
			int work = v % (2*dance[next].size());
			if (work < dance[next].size())
			{
				auto it = dance[next].begin();
				for (int i = 1; i < work; ++i)
					++it;
				dance[next].insert(it, v);
			}
			else
			{
				auto it = dance[next].end();
				for (int i = dance[next].size()+1; i < work; ++i)
					--it;
				dance[next].insert(it, v);
			}
		}

		for (const auto& l : dance)
		{
			part1 = 10*part1 + l.front();
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
		std::map<i64, int> shoutCounts;
		std::string s;
		std::getline(in2, s);
		std::stringstream ss(s);
		Dance dance;
		int v;
		while (ss >> v)
			dance.push_back(std::list<int>{v});

		while (std::getline(in2, s))
		{
			std::stringstream ss(s);
			for (int i = 0; i < dance.size(); ++i)
			{
				ss >> v;
				dance[i].push_back(v);
			}
		}
		i64 round = 0;
		while (true)
		{
			int v = dance[round % dance.size()].front(), next = (round + 1) % dance.size(), index = 0;
			dance[round % dance.size()].pop_front();
			bool up = true;
			auto iter = dance[next].begin();
			
			if (dance[next].size() > 1)
			{
				for (int i = 1; i < v; ++i)
				{
					if (up)
					{
						if (++iter == dance[next].end())
							up = false;
					}
					else if (--iter == dance[next].begin())
						up = true;
				}
				dance[next].insert(iter, v);
			}
			else
				dance[next].insert(v % 2 == 1 ? dance[next].begin() : dance[next].end(), v);

			i64 result = 0;
			for (const auto& l : dance)
				result = 100 * result + l.front();
			++round;

			if (++shoutCounts[result] == 2024)
			{
				part2 = round * result;
				break;
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
		std::map<i64, int> shoutCounts;
		std::string s;
		std::getline(in3, s);
		std::stringstream ss(s);
		Dance dance;
		int v;
		while (ss >> v)
			dance.push_back(std::list<int>{v});

		while (std::getline(in3, s))
		{
			std::stringstream ss(s);
			for (int i = 0; i < dance.size(); ++i)
			{
				ss >> v;
				dance[i].push_back(v);
			}
		}
		i64 round = 0;
		while (true)
		{
			int v = dance[round % dance.size()].front(), next = (round + 1) % dance.size(), index = 0;
			dance[round % dance.size()].pop_front();
			bool up = true;
			auto iter = dance[next].begin();
			if (dance[next].size() > 1)
			{
				for (int i = 1; i < v; ++i)
				{
					if (up)
					{
						if (++iter == dance[next].end())
							up = false;
					}
					else if (--iter == dance[next].begin())
						up = true;
				}
				dance[next].insert(iter, v);
			}
			else
				dance[next].insert(v % 2 == 1 ? dance[next].begin() : dance[next].end(), v);
			
			i64 result = 0;
			for (const auto& l : dance)
				result = 10000 * result + l.front();
			++round;
			if (++shoutCounts[result] == 100)
				break;
		}

		std::cout << std::format("Part 3: {}\n", (--shoutCounts.end())->first);
	}

}