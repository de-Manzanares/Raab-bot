Rewriting my first and very terrible chess engine to be my second and hopefully not so terrible chess engine.

Current Engine Features

- 0x88 board
- search
    - alpha-beta
    - quiescence search
    - transposition table
    - repetition detection
    - iterative deepening
- move ordering
    - transposition table
    - MVV/LVA
    - promotion material
- evaluation
    - material
    - mobility
    - king safety
- minimal UCI capabilities

Principles of Development:

    Modularity
        Clear separation of responsibilities
        Clean interfaces between modules
    Clarity
        Readable code
        As simple as possible, but no simpler
        Sufficient documentation at all levels of abstraction
    Informed
        Don't just wing it
        Actually read:
            The chess programming wiki
            The source code of other engines
        (it's okay to wing it a little just for fun, but go back and improve it with conventional wisdom after)
    Sustainable Development
        Clear roadmap for high-level goals
        Concise and measurable low-level goals

// TODO 50 move rule

position startpos moves e2e4 c7c5 g1f3 b8c6 d2d4 c5d4 f3d4 g8f6 b1c3 e7e5 d4b5 d7d6 c3d5 f6d5 e4d5 c6b8 a2a4 f8e7 f1e2 e8g8 e1g1 f7f5 f2f4 b8d7 g1h1 a7a6 b5a3 e5f4 c1f4 d7e5 d1d4 e7f6 d4b4 d8e8 f1e1 a6a5 b4d6 e5f7 d6c5 e8a4 c5c4 c8d7 c4a4 d7a4 a1b1 g7g5 f4e3 f8d8 b2b3 a4e8 b1d1 g5g4 e1f1 a5a4 a3c4 a4b3 c2b3 f7d6 c4d6 d8d6 f1f5 e8g6 f5f1 g6c2 d1c1 a8a2 e3c5 d6d5 e2c4
go wtime 69689 btime 70427 winc 1000 binc 1000
