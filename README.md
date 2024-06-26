<div align="center">
  
# Raab-bot :rabbit:

</div>

### Status

<div align="center">
  
![GitHub Release](https://img.shields.io/github/v/release/de-Manzanares/Raab-bot?style=for-the-badge&color=290b5e)

![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/de-Manzanares/Raab-bot/cmake-multi-platform.yml)
![Codecov](https://img.shields.io/codecov/c/github/de-Manzanares/Raab-bot)

</div>

### Performance 
###### (games, rating, rating percentile)

<div align="center">

![Dynamic JSON Badge](https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Flichess.org%2Fapi%2Fuser%2FRaab-bot&query=%24.count.rated&logo=lichess&label=games&style=for-the-badge)

![Dynamic JSON Badge](https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Flichess.org%2Fapi%2Fuser%2FRaab-bot&query=perfs.bullet.rating&logo=lichess&label=bullet)
![Dynamic JSON Badge](https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Flichess.org%2Fapi%2Fuser%2FRaab-bot&query=perfs.blitz.rating&logo=lichess&label=blitz)
![Dynamic JSON Badge](https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Flichess.org%2Fapi%2Fuser%2FRaab-bot&query=perfs.rapid.rating&logo=lichess&label=rapid)

![Dynamic JSON Badge](https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Flichess.org%2Fapi%2Fuser%2FRaab-bot%2Fperf%2Fbullet&query=%24.percentile&suffix=%25&logo=lichess&label=bullet)
![Dynamic JSON Badge](https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Flichess.org%2Fapi%2Fuser%2FRaab-bot%2Fperf%2Fblitz&query=%24.percentile&suffix=%25&logo=lichess&label=blitz)
![Dynamic JSON Badge](https://img.shields.io/badge/dynamic/json?url=https%3A%2F%2Flichess.org%2Fapi%2Fuser%2FRaab-bot%2Fperf%2Frapid&query=%24.percentile&suffix=%25&logo=lichess&label=rapid)

</div>

### Play me

<div align="center">
  
[Challenge me to a game!](https://lichess.org/?user=Raab-bot#friend)

###### (I only play the "Standard" variant with "Real time" time controls)

</div>

### Build from source

Because Raab-bot 🐰 is a UCI chess engine, it's pretty much useless without a GUI. Personally, I'd recommend [SCID vs. PC](https://scidvspc.sourceforge.net/).

#### Prerequisites

CMake version 3.20 or greater and GCC (I can't figure out how to get along with clang). 

There is a sprinkling of C++20 language features, so if your compiler isn't puttin' up with it, get a newer version! 
To see if your package manager has one (for Ubuntu-style systems at least) try ```apt search g++```.

#### Linux

The easist way to build the latest release is as follows:

```bash
git clone https://github.com/de-Manzanares/Raab-bot.git
cd Raab-bot
git checkout v1.00
./build.sh
```

#### Windows and Mac 🤷‍♂️ 
I'm sure you can figure it out. CMake is supposed to be cross-platform, right?

The build-test workflow ![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/de-Manzanares/Raab-bot/cmake-multi-platform.yml) verfies that this project can be built on Windows and Mac. 👍

## Acknowledgements

Thank you to the contributors of [Lila](https://github.com/lichess-org/lila) (aka Lichess), the coolest online chess platform! 

Thank you to the contributors of [lichess-bot](https://github.com/lichess-bot-devs/lichess-bot). There's no way I could have figured out the Lichess API on my own. 
Come to think of it, I'm not sure what API stands for... 

Most of all, a huge thank you to my CS professors and the CS department as a whole. Your well structured courses and welcoming community sparked a deep interest in C++. 😁

Thank you! 🙏
