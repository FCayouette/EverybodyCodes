import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()
using u64 = unsigned long long;


int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cout << "Usage: EC2024Day01.exe Part1Filename Part2Filename Part3Filename\n";
        return -1;
    }
    std::ifstream in(argv[1]);
    if (!in)
    {
        std::format("Could not open file {}.\nAborting!\n", argv[1]);
        return -1;
    }

    std::string s;
    in >> s;

	int part1 = std::count(ALLc(s), 'B') + 3 * std::count(ALLc(s), 'C'), part2 = 0, part3 = 0;
    std::cout << std::format("Part 1: {}\n", part1);

    std::ifstream in2(argv[2]);

	if (!in2)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
		return -1;
	}

    auto Valid = [](char c) {return c >='A' && c <='D';};
    auto Value = [](char c) {if (c == 'B') return 1; if (c=='C') return 3; if (c=='D') return 5; return 0;};

    char c1, c2;
    while (in2 >> c1 >> c2)
    {
        int v = Valid(c1) && Valid(c2) ? 2 : 0;
        part2 += v + Value(c1) + Value(c2);
    }
    std::cout << std::format("Part 2: {}\n", part2);
	std::ifstream in3(argv[3]);

	if (!in3)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
		return -1;
	}

    char c3;
    while (in3 >> c1 >> c2 >> c3)
    {
        int v = Valid(c1) + Valid(c2) + Valid(c3);
        if (v == 1) v = 0;
        else if (v == 3) v = 6;
        part3 += v + Value(c1) + Value(c2) + Value(c3);
    }

    std::cout << std::format("Part 3: {}\n", part3);
    return 0;
}
