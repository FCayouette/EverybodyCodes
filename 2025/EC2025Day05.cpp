import std.core;

#define ALL(x) (x).begin(),(x).end()
using u64 = unsigned long long;

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
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

struct FishBone
{
	int left = {}, center = {}, right = {};
};

constexpr std::vector<FishBone> GetFishBone(std::vector<std::string> values)
{
	std::vector<FishBone> fish;
	for (const std::string& s : values)
	{
		int v = std::stoi(s);
		bool found = false;
		for (int j = 0; j < fish.size() && !found; ++j)
		{
			FishBone& f = fish[j];
			if (v < f.center && !f.left)
			{
				f.left = v;
				found = true;
			}
			else if (v > f.center && !f.right)
			{
				f.right = v;
				found = true;
			}
		}
		if (!found)
			fish.emplace_back(0, v, 0);
	}
	return fish;
}

constexpr u64 Concat(u64 to, u64 a)
{
	while (a > 0)
	{
		to = to * 10 + a % 10;
		a /= 10;
	}
	return to;
}

constexpr u64 GetQuality(const std::vector<FishBone>& fish)
{
	u64 val = 0;
	for (auto [l, c, r] : fish)
		val = Concat(val, c);
	return val;
}

constexpr u64 GetBoneValue(FishBone f)
{
	return Concat(Concat(Concat(0, f.left), f.center), f.right);
}

struct FishValue
{
	u64 quality = {};
	std::vector<FishBone> fish;
	u64 id;

	constexpr bool operator<(const FishValue& fv) const
	{
		if (quality != fv.quality)
			return quality < fv.quality;

		for (size_t i = 0; i < fv.fish.size() && i < fish.size(); ++i)
			if (u64 left = GetBoneValue(fish[i]), right = GetBoneValue(fv.fish[i]); left != right)
				return left < right;
		return id < fv.id;
	}
};

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025S1QX.exe Part1Filename Part2Filename Part3Filename\n";
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
		std::string part1 = "";
		in1 >> line;

		std::cout << std::format("Part 1: {}\n", GetQuality(GetFishBone(SplitAt(SplitAtFirst(std::move(line), ":").second, ","))));
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	{
		u64 min = 10000000000, max = 0;

		while (in2 >> line)
		{
			u64 val = GetQuality(GetFishBone(SplitAt(SplitAtFirst(std::move(line), ":").second, ",")));
			min = std::min(val, min);
			max = std::max(val, max);
		}
		std::cout << std::format("Part 2: {}\n", max-min);
	}

	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	{
		u64 part3 = 0;
		std::vector<FishValue> fishValues;
		while (in3 >> line)
		{
			auto sword = SplitAtFirst(std::move(line), ":");
			FishValue fv;
			fv.id = std::stoull(sword.first);
			fv.fish = GetFishBone(SplitAt(std::move(sword.second), ","));
			fv.quality = GetQuality(fv.fish);
			fishValues.emplace_back(std::move(fv));
		}

		std::sort(ALL(fishValues));
		for (size_t i = fishValues.size()-1, index = 1; index <= fishValues.size(); ++index, --i)
			part3 += index * fishValues[i].id;

		std::cout << std::format("Part 3: {}\n", part3);
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}