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
    - MVV/LVA
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
