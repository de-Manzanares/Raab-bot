#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: lo <FILE>\n";
    return 1;
  }

  const std::filesystem::path f(argv[1]);
  std::ifstream               ifs(f);
  if (!ifs) {
    std::cerr << "cannot open " << f << '\n';
    return 1;
  }

  std::string              s;
  std::vector<std::string> lines;

  while (std::getline(ifs, s)) {
    lines.push_back(s);
  }
  std::ranges::sort(lines);

  std::ofstream ofs(f.string() + ".lo");
  if (!ofs) {
    std::cerr << "cannot create output file\n";
  }

  for (const auto &line : lines) {
    ofs << line << '\n';
  }
}
