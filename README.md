# Tic Tac Toe (Console Game)  

**A simple console-based Tic Tac Toe game with AI powered by the Minimax algorithm.**  

## 🔹 Features  
✅ **Play against AI** with smart moves (Minimax algorithm)  
✅ **Cross-platform** (Windows & Linux support, auto-detects console size)  
✅ **Dynamic UI** (adapts to terminal dimensions)  
✅ **Score tracking** (wins/losses/draws)  
✅ **AI move animation** ("Thinking..." with visual dots)  

## 🔹 Requirements  
- C++17 compiler (GCC, Clang, or MSVC)  

## 🔹 Installation & Running  
1. **Clone the repository:**  
   ```sh
   git clone https://github.com/your-username/tic-tac-toe.git
   cd tic-tac-toe
   ```

2. **Compile and run:**  
   ```sh
   g++ -std=c++17 main.cpp -o tic-tac-toe
   ./tic-tac-toe
   ```
   *On Windows, use MinGW or Visual Studio's compiler.*

## 🔹 Controls  
- Choose **X** or **O** at the start  
- Enter coordinates in `x y` format (e.g., `2 3`)  

## 🔹 How It Works  
- The AI uses **Minimax** for optimal moves.  
- The UI refreshes dynamically using ANSI escape codes.  

## 🔹 Future Improvements  
- [ ] Difficulty levels (easy/medium/hard)  
- [ ] GUI version (SDL/ImGui)  
- [ ] Network multiplayer  
