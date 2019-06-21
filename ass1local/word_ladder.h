#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


/* just as a rung represents one step on a normal ladder,
 * a word represents one step on the word-ladder. */
struct rung {
  std::string word; // the "rung" itself
  int distance; // the number of steps to get to this rung from the starting rung (guess)
};

// i.e. parent x found at distance y
using parent_dists = std::unordered_map<std::string, int>;

/* map of words seen, the keys are the words seen, the values are the words they were seen from
 * map(word -> {parent1, distance}, {parent2, distance}, ..., {parentn, distance}) */
using seen_map = std::unordered_map<std::string, parent_dists>;

/*
 * given a lexicon, a guess word, and a target word, this function finds and
 * returns all best word-ladders (or no solution otherwise). Uses BFS.
 */
std::set<std::string> FindWordLadder(const std::unordered_set<std::string>&,
                                        const std::string&, const std::string&);

/*
 * print the set of solutions retrieved from the FindWordLadder function
 */
void PrintSolutions(const std::set<std::string>& solutions);


#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_

