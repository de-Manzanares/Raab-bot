<div align="center">

# Raab-bot :rabbit:

</div>

### Status

<div align="center">

![GitHub Tag](https://img.shields.io/github/v/tag/de-Manzanares/Raab-bot?style=for-the-badge&label=latest&color=290b5e)

![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/de-Manzanares/Raab-bot/.github%2Fworkflows%2Fcmake-macos-latest.yml?logo=apple&label=macos)
![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/de-Manzanares/Raab-bot/.github%2Fworkflows%2Fcmake-ubuntu-latest.yml?logo=ubuntu&label=ubuntu)
![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/de-Manzanares/Raab-bot/.github%2Fworkflows%2Fcmake-windows-latest.yml?logo=github&label=windows)
![Codecov](https://img.shields.io/codecov/c/github/de-Manzanares/Raab-bot?logo=Codecov)

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

Because Raab-bot 🐰 is
a [UCI](https://en.wikipedia.org/wiki/Universal_Chess_Interface) chess engine,
it's pretty much useless without a GUI.
Personally, I'd recommend [SCID vs. PC](https://scidvspc.sourceforge.net/).

#### Prerequisites

CMake: Version 3.20 or higher.

Compilers: The project has been successfully built and tested with Clang,
AppleClang, GCC, and MSVC.

Language features: This project uses some C++20 language features. Any
reasonably recent version of the above compilers should be fine.

#### Linux

The easiest way to build the latest version is as follows:

```bash
git clone https://github.com/de-Manzanares/Raab-bot.git
cd Raab-bot
git checkout $(git describe --tags `git rev-list --tags --max-count=1`)
./build.sh
```

## Acknowledgements

The ASCII file headers were generated using
[patorjk](https://github.com/patorjk)'s
[Text to ASCII Art Generator](https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20).

Thank you to the contributors
of [lichess-bot](https://github.com/lichess-bot-devs/lichess-bot).

Thank you to the contributors of [Lila](https://github.com/lichess-org/lila)
(aka Lichess),
the coolest online chess platform!

Most of all, a huge thank you to my CS professors and the CS department as a
whole.
Your well-structured courses and welcoming community sparked a deep
interest in C++.
😁

Thank you! 🙏
