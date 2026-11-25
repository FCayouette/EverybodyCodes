import std;

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

template <typename T>
class y_combinator {
	T lambda;
public:
	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}
	template <typename...Args>
	constexpr decltype(auto) operator()(Args&&...args) const { return lambda(std::move(*this), std::forward<Args>(args)...); }
};

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 4)
	{
		std::cout << "Usage: Quest06.exe Part1Filename Part2Filename Part3Filename\n";
		return -1;
	}
	std::string line;
	std::unordered_map<std::string, size_t> nameToCode = { { "NULL", 0 }, { "NULLv1", 1 }, { "NULLv2", 2 }, { "NULLv3", 3 } };

	auto GetCode = [&nameToCode](const std::string&& s)
		{
			if (auto iter = nameToCode.find(s); iter != nameToCode.cend())
				return iter->second;
			size_t result = nameToCode.size();
			nameToCode[s] = result;
			return result;
		};

	{
		std::ifstream in(argv[1]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
			return -1;
		}
		std::unordered_map<size_t, std::vector<size_t>> recipes;
		std::vector<size_t> toProcess;
		while (in >> line)
		{
			const auto [prod, temp] = SplitAtFirst(std::move(line), ":");
			std::vector<size_t> elems;
			for (const std::string& s : SplitAt(std::move(temp), ","))
				elems.push_back(GetCode(std::move(s)));
			size_t prodCode = GetCode(std::move(prod));
			if (!prodCode)
				toProcess = elems;
			recipes[prodCode] = std::move(elems);
		}
		
		std::unordered_set<size_t> inTheQueue{ toProcess.cbegin(), toProcess.cend() };
		for (size_t i = 0; i < toProcess.size(); ++i)
		{
			const std::vector<size_t>& check = recipes[toProcess[i]];
			for (size_t e: check)
				if (e != 0 && inTheQueue.insert(e).second)
					toProcess.push_back(e);
		}

		std::cout << std::format("Part 1: {}\n", toProcess.size());
	}

	{
		std::ifstream in(argv[2]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[2]);
			return -1;
		}
		std::unordered_map<size_t, std::vector<size_t>> usedBy;
		while (in >> line)
		{
			const auto [prod, temp] = SplitAtFirst(std::move(line), ":");
			size_t prodCode = GetCode(std::move(prod));
			for (const std::string& s : SplitAt(std::move(temp), ","))
				usedBy[GetCode(std::move(s))].push_back(prodCode);
		}

		std::unordered_set<size_t> processed;
		std::vector toProcess = usedBy[0];
		for (size_t i = 0; i < toProcess.size(); ++i)
		{
			size_t cur = toProcess[i];
			processed.insert(cur);
			for (size_t elem : usedBy[cur])
				if (!processed.contains(elem) && std::find(toProcess.cbegin() + i, toProcess.cend(), elem) == toProcess.cend())
					toProcess.push_back(elem);
		}
		std::cout << std::format("Part 2: {}\n", processed.size());
	}

	{
		std::ifstream in(argv[3]);
		if (!in)
		{
			std::cout << std::format("Could not open file {}.\nAborting!\n", argv[3]);
			return -1;
		}
		
		std::unordered_map<size_t, std::vector<size_t>> recipes;
		
		
		while (in >> line)
		{
			const auto [prod, temp] = SplitAtFirst(std::move(line), ":");
			size_t prodCode = GetCode(std::move(prod));
			std::vector<size_t> elems;
			for (const std::string& s : SplitAt(std::move(temp), ","))
				elems.push_back(GetCode(std::move(s)));
			recipes[prodCode] = std::move(elems);
		}
		std::vector<size_t> nullUsage(nameToCode.size(), 0);

		for (const auto& [recipe, ingredients] : recipes)
		{
			unsigned char mask = 0;
			std::unordered_set<size_t> checked;

			auto NullCheck = y_combinator([&checked, &nullUsage, &recipes](auto&& NullCheck, size_t s) -> unsigned char
				{
					if (auto val = nullUsage[s]; val)
						return val;
					unsigned char result = 0;
					if (checked.contains(s)) return result;

					checked.insert(s);
					if (!recipes.contains(s)) return result;

					for (size_t e : recipes[s])
						result |= e <= 3 ? 1 << (e - 1) : NullCheck(e);
					if (std::popcount(result) > 1)
						nullUsage[s] = result;
					return result;
				});
			
			
			nullUsage[recipe] = NullCheck(recipe);
		}
		std::cout << std::format("Part 3: {}\n", std::count_if(nullUsage.cbegin(), nullUsage.cend(), [](size_t mask) { return std::popcount(mask) > 1; }));
	}
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}