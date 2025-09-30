#ifndef RAAB_BOT_LOG_INL
#define RAAB_BOT_LOG_INL

template <class... Ts> void logln(const Ts &...xs)
{
  ((std::cout << xs << ' '), ...);
  std::cout << std::endl;

  // ((ofile << xs << ' '), ...);
  // ofile << std::endl;
}

#endif // RAAB_BOT_LOG_INL
