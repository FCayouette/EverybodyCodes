import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()
using u64 = unsigned long long;


int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::cout << "Usage: EC2024Day02.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}

	std::string s;
	std::getline(in, s);
	s = s.substr(6);
	std::vector<std::string> words;
	auto pos = s.find(',');
	while (pos != std::string::npos)
	{
		words.emplace_back(s.substr(0, pos));
		s = s.substr(pos+1);

		pos = s.find(',');
	}
	words.emplace_back(std::move(s));

	std::getline(in, s);
	std::getline(in, s);
	int part1 = 0, part2 = 0, part3 = 0;

	for (const std::string& w : words)
	{
		auto pos = s.find(w, 0);
		while (pos != std::string::npos)
		{
			++part1;
			pos = s.find(w, pos+1);
		}
	}

	std::cout << std::format("Part 1: {}\n", part1);
	
	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	words.clear();
	std::getline(in2, s);
	s = s.substr(6);
	pos = s.find(',');
	while (pos != std::string::npos)
	{
		words.push_back(s.substr(0, pos));
		s = s.substr(pos + 1);

		pos = s.find(',');
	}
	words.emplace_back(std::move(s));

	std::getline(in2, s);
	while (std::getline(in2, s))
	{
		std::set<int> indices;
		for (const std::string& w : words)
		{
			auto pos = s.find(w, 0);
			while (pos != std::string::npos)
			{
				for (int i = 0; i < w.size(); ++i)
					indices.insert(pos+i);
				pos = s.find(w, pos + 1);
			}
			std::string r = w;
			std::reverse(ALL(r));
			pos = s.find(r, 0);
			while (pos != std::string::npos)
			{
				for (int i = 0; i < r.size(); ++i)
					indices.insert(pos + i);
				pos = s.find(r, pos + 1);
			}
		}

		part2 += indices.size();
	}

	std::cout << std::format("Part 2: {}\n", part2);

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	words.clear();
	std::getline(in3, s);
	s = s.substr(6);
	pos = s.find(',');
	while (pos != std::string::npos)
	{
		words.push_back(s.substr(0, pos));
		s = s.substr(pos + 1);

		pos = s.find(',');
	}
	words.emplace_back(std::move(s));

	std::vector<std::string> armour;
	std::set<std::pair<int, int>> scales;
	while (in3 >> s)
		armour.emplace_back(std::move(s));

	int maxX = armour.size(), maxY = armour.front().size();
	for (int x = 0; x < maxX; ++x)
		for (int y = 0; y < maxY; ++y)
			for (const std::string& w : words)
				if (armour[x][y] == w[0])
				{
					int count = 1;
					for (int i = 1; i < w.size(); ++i)
						count += armour[x][(y + i) % maxY] == w[i];
					if (count == w.size())
						for (int i= 0; i < w.size(); ++i)
							scales.emplace(x, (y + i) % maxY);
					if (x + w.size() <= maxX)
					{
						count = 1;
						for (int i = 1; i < w.size(); ++i)
							count += armour[x + i][y] == w[i];
						if (count == w.size())
							for (int i = 0; i < w.size(); ++i)
								scales.emplace(x + i, y);
					}
					count = 1;
					for (int i = 1; i < w.size(); ++i)
						count += armour[x][(y - i + maxY) % maxY] == w[i];
					if (count == w.size())
						for (int i = 0; i < w.size(); ++i)
							scales.emplace(x, (y - i + maxY) % maxY);

					if (x >= w.size()-1)
					{
						count = 1;
						for (int i = 1; i < w.size() && w.size() < maxX; ++i)
							count += armour[x - i][y] == w[i];
						if (count == w.size())
							for (int i = 0; i < w.size(); ++i)
								scales.emplace(x - i, y);
					}
				}

	std::cout << std::format("Part 3: {}\n", scales.size());
	return 0;
}
