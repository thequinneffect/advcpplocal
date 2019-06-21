#include <iostream>

#include "assignments/wl/lexicon.h"
#include "assignments/wl/word_ladder.h"

int main() {

  auto lexicon = GetLexicon("assignments/wl/words.txt");

  std::string guess {};
  std::string target {};

  std::cout << "Enter start word (RETURN to quit): ";
  std::getline(std::cin, guess, '\n');

  if (guess.empty()) return 0;

  std::cout << "Enter destination word: ";
  std::getline(std::cin, target, '\n');

  auto solutions = FindWordLadder(lexicon, guess, target);

  PrintSolutions(solutions);

  return 0;
}

