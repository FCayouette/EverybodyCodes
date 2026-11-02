import std;

#define ALL(x) (x).begin(),(x).end()

using u64 = unsigned long long;
using i64 = long long;

int GetVal(std::string&& s)
{
	return std::stoi(s.substr(s.find('=') +1));
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: Quest04.exe Part1Filename Part2Filename Part3Filename\n";
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
		{
			u64 part1 = 0;

			std::vector<std::array<int,3>> times;
			while (in >> line)
			{
				int id = GetVal(std::move(line));
				in >> line >> line;
				int part = GetVal(std::move(line));
				in >> line;
				int time = GetVal(std::move(line));
				if (times.size() <= id)
					times.resize(id+1);
				times[id][part-1] = time;
			}

			std::vector<int> totals;
			for (int i = 1; i < times.size(); ++i)
				totals.push_back(times[i][0] + times[i][1] + times[i][2]);

			std::sort(ALL(totals));
			for (int i =0; i < totals.size(); ++i)
				part1 += (i+1) * totals[i];

			std::cout << std::format("Part 1: {}\n", part1);
		}
	}

	{
		std::ifstream in(argv[2]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
			return -1;
		}
		{
			u64 part2 = 0;
			std::map<int, std::vector<int>> scores;
			while (in >> line)
			{
				int id = GetVal(std::move(line));
				in >> line >> line >> line;
				int time = GetVal(std::move(line));
				scores[id].push_back(time);
			}

			std::vector<std::pair<int, int>> summary;
			for (auto [_, v] : scores)
				summary.emplace_back((int)v.size(), std::accumulate(v.cbegin(), v.cend(), 0));

			std::sort(ALL(summary), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {if (a.first > b.first) return true; if (a.first < b.first) return false; return a.second < b.second;});
			for (int i = 0; i < summary.size(); ++i)
				part2 += (i+1) * summary[i].second;


			std::cout << std::format("Part 2: {}\n", part2);
		}
	}

	{
		std::ifstream in(argv[3]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
			return -1;
		}
		{
			u64 part3 = 0;

			std::vector<std::map<int, std::array<int,3>>> questScores;
			while (in >> line)
			{
				int id = GetVal(std::move(line));
				in >> line;
				int quest = GetVal(std::move(line))-1;
				if (quest >= questScores.size())
					questScores.resize(quest+1);
				in >> line;
				int part = GetVal(std::move(line))-1;
				in >> line;
				int time = GetVal(std::move(line));
				questScores[quest][id][part] = time;
			}
			std::map<int, int> userScores;
			for (auto& q : questScores)
			{
				std::array<std::vector<std::pair<int, int>>, 3> questTimes;
				for (auto [id, times] : q)
					for (int i = 0; i < 3; ++i)
						if (times[i] != 0)
						{
							if (i != 0)
								times[i] += times[i-1];
							questTimes[i].emplace_back(times[i], id);
						}
				
				for (int i = 0; i < 3; ++i)
				{
					int total = (i+1)*50, lastScore = 0, lastPlace = 1;
					std::sort(ALL(questTimes[i]));
					for (int x = 0; x < questTimes[i].size() && lastPlace <= total; ++x)
					{
						auto [t, user] = questTimes[i][x]; 
						if (t != lastScore)
						{
							lastScore = t;
							lastPlace = x;
						}
						userScores[user] += std::max(0, total-lastPlace);
					}
				}
			}

			std::vector<int> scores;
			for (auto[_, score] : userScores)
				scores.emplace_back(score);
			std::sort(ALL(scores), std::greater());
			int lastScore = 0, lastPlace = 0;
			for (int i = 0; i < scores.size(); ++i)
			{
				int s = scores[i];
				if (s != lastScore)
				{
					lastScore = s;
					lastPlace = i+1;
				}
				part3 += lastPlace*s;
			}
			std::cout << std::format("Part 3: {}\n", part3);
		}
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}