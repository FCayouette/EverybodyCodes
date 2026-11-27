import std;

struct Cloud
{
	int energy = {};
	std::vector<int> connections;
	bool exploded = false;
};

std::vector<Cloud> GenerateClouds(std::istream& in)
{
	std::string line;
	std::vector<Cloud> clouds;
	while (in >> line)
	{
		clouds.emplace_back();
		in >> line;
		size_t pos = line.find('=');
		clouds.back().energy = std::atoi(line.substr(pos + 1, line.size() - pos - 2).c_str());
		in >> line;
		pos = line.find('=');
		size_t tokenStart = pos + 1;
		do
		{
			size_t e = line.find(',', tokenStart);
			if (e == std::string::npos)
			{
				clouds.back().connections.emplace_back(std::stoi(line.substr(tokenStart).c_str()) - 1);
				break;
			}
			clouds.back().connections.emplace_back(std::stoi(line.substr(tokenStart, e - tokenStart).c_str()) - 1);
			tokenStart = e + 1;
		} while (tokenStart < line.size());
	}

	return clouds;
}

int SpreadImpulse(std::vector<Cloud>&& clouds, int impulse)
{
	int start = 0;
	for (int i = 1; i < clouds.size(); ++i)
		if (clouds[i].connections.size() > clouds[start].connections.size())
			start = i;

	std::unordered_map<int, int> addTimes, next;
	addTimes[start] = 1;
	do
	{
		for (const auto [id, times] : addTimes)
		{
			clouds[id].energy += times * impulse;
			for (int c : clouds[id].connections)
				next[c] += times;
		}
		std::swap(addTimes, next);
		next.clear();
	} while (--impulse > 0);
	return std::accumulate(clouds.cbegin(), clouds.cend(), 0, [](int total, const Cloud& c) { return total + c.energy; });
}

int GetMaxCloud(const std::vector<Cloud>& init, size_t start, size_t end)
{
	int best = 0;
	std::vector<Cloud> clouds = init;
	std::array<std::unordered_map<int, int>, 11> addTimes, next;
	for (size_t i = start; i < std::min(clouds.size(), end); ++i)
	{
		for (size_t j = 0; j < clouds.size(); ++j)
			clouds[j].exploded = false, clouds[j].energy = init[j].energy;

		for (int j = 0; j < addTimes.size(); ++j)
		{
			addTimes[j].clear();
			next[j].clear();
		}
		addTimes[10][static_cast<int>(i)] = 1;
		bool goOn = false;
		do {
			goOn = false;
			for (int strength = 1; strength < addTimes.size(); ++strength)
				for (const auto [id, times] : addTimes[strength])
					if (!clouds[id].exploded)
					{
						clouds[id].energy += times * strength;
						goOn = true;
						for (int c : clouds[id].connections)
							next[strength][c] += times;
					}
			if (goOn)
			{
				for (size_t j = 0; j < 10; ++j)
				{
					next[j].clear();
					std::swap(addTimes[j], next[j + 1]);
				}
				addTimes[0].clear();
				addTimes[10].clear();
				next[10].clear();

				for (Cloud& c : clouds)
					if (c.energy >= 1000000)
					{
						c.energy = 0;
						c.exploded = true;
						for (int connection : c.connections)
							addTimes[10][connection] += 1;
					}
			}
		} while (goOn);

		best = std::max(best, std::accumulate(clouds.cbegin(), clouds.cend(), 0, [](int total, const Cloud& c) { return total + c.energy; }));
	}
	return best;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: Quest07.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	{
		std::ifstream in(argv[1]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
			return -1;
		}
		std::cout << std::format("Part 1: {}\n", SpreadImpulse(GenerateClouds(in), 5));
	}

	{
		std::ifstream in(argv[2]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
			return -1;
		}
		std::cout << std::format("Part 2: {}\n", SpreadImpulse(GenerateClouds(in), 10));
	}

	{
		std::ifstream in(argv[3]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
			return -1;
		}
		std::vector<Cloud> clouds = GenerateClouds(in);
		std::vector<std::future<int>> tasks;
		for (size_t i = 0; i < clouds.size(); i += 20)
			tasks.emplace_back(std::async(GetMaxCloud, clouds, i, i + 20));

		std::cout << std::format("Part 3: {}\n", std::reduce(tasks.begin(), tasks.end(), 0, [](int best, auto& task) { return std::max(best, task.get()); }));
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}