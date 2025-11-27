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

struct Plant
{
	Plant() = default;
	Plant(i64 t) : thickness(t), energy(0ll) {}
	i64 thickness, energy;
	std::vector<std::pair<size_t, i64>> connections;
	void Validate() { if (energy < thickness) energy = 0; }
};

std::vector<Plant> plants;

i64 Simulate(std::vector<Plant>& plants, std::vector<std::string> init)
{
	for (int i = 0; i < plants.size(); ++i)
		if (i < init.size())
			plants[i].energy = init[i][0] - '0';
		else
		{
			plants[i].energy = 0;
			for (auto [to, thick] : plants[i].connections)
				plants[i].energy += thick * plants[to].energy;
			plants[i].Validate();
		}

	return plants.back().energy;
};

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day18.exe Part1Filename Part2Filename Part3Filename\n";
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
		std::vector<int> plantThickness = { 0 }, plantEnergy = { 0 };
		while (std::getline(in1, line))
		{
			if (line.empty())
			{
				if (plantEnergy.back() < plantThickness.back())
					plantEnergy.back() = 0;
			}			
			else if (line[0] == 'P')
			{
				auto pos = line.rfind(' ');
				plantThickness.push_back(std::stoi(line.substr(pos+1, line.size() - pos-2)));
				plantEnergy.push_back(0);
			}
			else if (line[0] == '-')
			{
				if (auto tokens = SplitAt(std::move(line), " "); 
					tokens[1][0] == 'f')
					plantEnergy.back() += 1;
				else
					plantEnergy.back() += plantEnergy[std::stoi(tokens[4])] * std::stoi(tokens.back());
			}
		}
		// Sanity check on last
		if (plantEnergy.back() < plantThickness.back())
			plantEnergy.back() = 0;

		std::cout << std::format("Part 1: {}\n", plantEnergy.back());
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		u64 part2 = 0;
		std::vector<Plant> plants;
		
		while (std::getline(in2, line))
		{
			if (line.empty())
			{
				plants.back().Validate();
				continue;
			}

			if (char c = line[0]; c == 'P')
				plants.emplace_back(std::stoi(line.substr(line.rfind(' ')+1)));
			else if (c == '-' && line[2] != 'f' )
			{	
				auto tokens = SplitAt(std::move(line), " "); 
				plants.back().connections.emplace_back(std::stoi(tokens[4]) - 1, std::stoll(tokens.back()));
			}
			else if (c == '0' || c == '1')
				part2 += Simulate(plants, SplitAt(std::move(line), " "));
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
		std::vector<Plant> plants;
		std::vector<i64> scores;
		std::set<size_t> pos, neg;
		size_t input = 0, withFreeOnly = 0;
		
		while (std::getline(in3, line))
		{
			if (line.empty())
				plants.back().Validate();
			else if (char c = line[0];
					 c == 'P')
				plants.emplace_back(std::stoi(line.substr(line.rfind(' ') + 1)));
			else if (c == '-')
			{
				if (line[2] != 'f')
				{
					auto tokens = SplitAt(std::move(line), " ");
					size_t source = std::stoi(tokens[4]) - 1;
					plants.back().connections.emplace_back(source, std::stoll(tokens.back()));
					if (!withFreeOnly && source > input)
						withFreeOnly = plants.size() - 1;
				}
				else
					input = plants.size();
			}
			else if (c == '0' || c == '1')
				if (i64 val = Simulate(plants, SplitAt(std::move(line), " "));
					val > 0)
					scores.push_back(val);
		}

		// Search max
		for (int i = input; i < withFreeOnly; ++i)
		{
			i64 total = 0;
			for (auto [_, t] : plants[i].connections)
				total += std::max(t, 0ll);
			if (total >= plants[i].thickness)
				for (auto [c, t] : plants[i].connections)
					if (t > 0) pos.insert(c);
		}
		std::vector<std::string> perfect;
		for (size_t i = 0; i < input; ++i)
			perfect.emplace_back(pos.contains(i) ? "1" : "0");
		i64 best = Simulate(plants, perfect);

		for (i64 s : scores)
			part3 += best - s;

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}