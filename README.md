# ConsoleChessCPP

![Chessboard](chessboard.png)

ConsoleChessCPP is an efficient chess game designed in C++ that brings the world of chess to your terminal. It is a minimalistic yet powerful implementation that leverages bitboards and bitmasks to maximize efficiency and minimize memory usage.

## Project Files

The project includes the following files:

- `board.cpp`: Unleash the power of chessboard manipulation with functions for initialization, move generation, and more.
- `movement.cpp`: Dive into the depths of chess moves with functions for piece movements, capturing, and special moves.
- `board.hpp`: A handy header file that provides access to the functions used in `board.cpp` and `movement.cpp`.

## How to Play

1. Clone the repository:

   ```bash
   git clone https://github.com/your-username/ConsoleChessCPP.git
   ```

2. Build the project using your preferred C++ compiler. For example:

   ```bash
   g++ board.cpp movement.cpp -o ConsoleChessCPP
   ```

3. Run the executable:

   ```bash
   ./ConsoleChessCPP
   ```

4. Get ready to unleash your strategic prowess! The console will display the current board state, and you can enter your moves using standard chess notation.

## Acknowledgements

- [chessprogramming.org](https://www.chessprogramming.org/): A valuable source of knowledge and resources for chess programming enthusiasts.

- [Tinker](https://www.chessprogramming.net/tinker/): The private Chess Engine Communication Protocol used in ConsoleChessCPP.

- [Chess Programming Wiki](https://www.stmintz.com/ccc/index.php?id=487844): A comprehensive wiki dedicated to chess programming, providing insights and ideas for chess engine development.

## Dependencies

The ConsoleChessCPP project does not have any external dependencies. It is implemented purely in C++ using the standard library.
