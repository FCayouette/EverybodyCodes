import std;

using u64 = unsigned long long;

struct Range
{
	int start = {}, end = {};
	constexpr int GetSize() const { return end - start + 1; }
};

int GetNumber(std::ifstream& in, u64 turns)
{
	u64 size = 1, count = 0;
	std::string line;

	bool clockwise = true;
	std::vector<Range> cw = { Range(1,1) }, ccw;
	while (in >> line)
	{
		auto pos = line.find('-');
		Range r(std::stoi(line.substr(0, pos)), std::stoi(line.substr(pos + 1)));
		size += r.GetSize();
		if (clockwise) cw.emplace_back(std::move(r));
		else ccw.emplace_back(std::move(r));
		clockwise = !clockwise;
	}
	bool reverseAt = cw.size();
	cw.insert(cw.end(), ccw.crbegin(), ccw.crend());
	u64 remainder = turns % size;

	auto iter = cw.cbegin();
	while (iter->GetSize() <= remainder)
	{
		remainder -= iter++->GetSize();
		++count;
	}

	return count < reverseAt ? iter->start + remainder : iter->end - remainder;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: EC2025Day13.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::ifstream in1(argv[1]);
	
	if (!in1)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	{
		int val;
		bool clockwise = true;
		std::vector<int> cw = {1}, ccw;
		while (in1 >> val)
		{
			if (clockwise) cw.push_back(val);
			else ccw.push_back(val);
			clockwise = !clockwise;
		}
		
		if (int index = 2025 % (cw.size() + ccw.size()); index < cw.size())
			val = cw[index];
		else
			val = *(ccw.crbegin() + (index-cw.size()));

		std::cout << std::format("Part 1: {}\n", val);
	}

	std::ifstream in2(argv[2]);
	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}
	std::cout << std::format("Part 2: {}\n", GetNumber(in2, 20252025));
	
	std::ifstream in3(argv[3]);
	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}
	std::cout << std::format("Part 3: {}\n",  GetNumber(in3, 202520252025));
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}