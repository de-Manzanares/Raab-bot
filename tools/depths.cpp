
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>

using U8 = std::uint8_t;

template <typename T> class LessThan {
  const T _val;

 public:
  LessThan(const T &val) : _val{val} {}
  bool operator()(const T &val) const { return val < _val; }
};

int main(int argc, char *argv[])
{
  constexpr U8 max_depth = -1;
  using DepthArr         = std::array<std::size_t, max_depth>;

  std::ifstream ifile(argv[1]);
  if (!ifile.is_open()) {
    std::cerr << "Can't open file" << argv[1] << '\n';
    return 1;
  }

  std::string line;
  DepthArr    all_depths{};
  std::string sub_s = "info depth";

  while (std::getline(ifile, line)) {
    if (const auto it = line.find(sub_s); it != std::string::npos) {
      ++all_depths[std::stoi(line.substr(it + sub_s.size() + 1))];
    }
  }

  std::cout << argv[1] << '\n';
  std::size_t        fmc = all_depths[1];
  DepthArr::iterator lb;
  DepthArr::iterator ub;
  lb = std::prev(std::find_if(std::next(all_depths.begin(), 2),
                              all_depths.end(), LessThan(fmc)));

  // weird assignment stuff for my logging quirks
  ub = (std::find(lb, all_depths.end(), 1));
  if (ub == all_depths.end()) {
    ub = std::find(lb, all_depths.end(), 0);
  }
  else {
    ub = std::next(ub);
  }

  for (auto it = lb; it < std::prev(ub); ++it) {
    *it -= *std::next(it);
  }
  assert(fmc == std::accumulate(lb, ub, 0UL));
  for (auto it = lb; it < ub; ++it) {
    std::cout << std::distance(all_depths.begin(), it) << " : " << *it << '\n';
  }
  double avg_depth{};
  for (auto it = lb; it < ub; ++it) {
    avg_depth += std::distance(all_depths.begin(), it) * *it;
  }
  avg_depth /= fmc;

  std::cout << "avg depth: " << avg_depth << '\n';
}
