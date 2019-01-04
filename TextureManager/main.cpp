#include <string>
#include <fstream>

template<typename _charType>
void LoadStringFromFile(std::ifstream & _ifs, std::basic_string<_charType> & _dest)
{
	uint64_t stringSize = 0;
	_ifs.read(reinterpret_cast<char*>(&stringSize), sizeof(uint64_t));
	_dest.resize(stringSize);
	_ifs.read(reinterpret_cast<char*>(&_dest[0]), sizeof(std::basic_string<_charType>::value_type) * stringSize);
}

template<typename _charType>
void SaveStringToFile(std::ofstream & _ofs, const std::basic_string<_charType> & _source)
{
	uint64_t stringSize = _source.size();
	_ofs.write(reinterpret_cast<char*>(&stringSize), sizeof(uint64_t));
	_ofs.write(reinterpret_cast<const char*>(_source.data()), sizeof(std::basic_string<_charType>::value_type) * stringSize);
}

int main()
{
	std::wstring a[] = {
		L"bricks", L"bricks.dds",
		L"bricks_nmap", L"bricks_nmap.dds",
		L"checkboard", L"checkboard.dds",
		L"default_nmap", L"default_nmap.dds"
	};

	uint64_t textureCount = _countof(a) / 2;

	std::ofstream ofs(L"TextureManager.data", std::ios::binary | std::ios::out);

	ofs.write(reinterpret_cast<const char*>(&textureCount), sizeof(uint64_t));

	for (auto _ele : a)
	{
		SaveStringToFile<wchar_t>(ofs, _ele);
	}
}