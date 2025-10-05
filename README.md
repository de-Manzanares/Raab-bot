Rewriting my first and very terrible chess engine to be my second and hopefully not so terrible chess engine.

Estimated elo (CCRL distribution): 1477 +/- 8

### Engine Features

- 0x88 board
- search
    - alpha-beta with principal variation search
    - futility pruning
    - history heuristic
    - iterative deepening
    - killer move heuristic
    - null move pruning
    - quiescence search with delta pruning
    - razoring
    - transposition table
- evaluation
    - Tomasz Michniewski's simplified evaluation function
        - (a stand-in while other engine features are being developed)
- UCI

### Parametric compilation

(see [config.cppm](modules/config.cppm))

| feature                  | default |
|--------------------------|---------|
| delta pruning in qsearch | ON      |
| futility pruning         | OFF     |
| history heuristic        | ON      |
| killer heuristic         | OFF     |
| null move pruning        | ON      |
| razoring                 | OFF     |

#### Principles of Development:

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

    Debugging is twice as hard as writing the code in the first place. 
    Therefore, if you write the code as cleverly as possible, you are, by definition, not smart enough to debug it.
    -- Brian W. Kernighan
