#include "assignments/wl/word_ladder.h"

#include <bits/unordered_set.h>
#include <iostream>
#include <queue>
#include <set>
#include <stack>


// default (unrestricted) value for BFS maximum search depth, updated once target is first found
constexpr int NOT_SET = -1;


// prototypes of "private" helper functions
std::vector<std::string> GetNextRungs(const std::unordered_set<std::string>& lexicon,
                                      seen_map& words_seen, const std::string& curr_rung,
                                      const std::string& target, const int& distance);
void BuildSolutionsWithRDFS(seen_map& words_seen, const std::string& curr_word,
                            const std::string& guess, std::string curr_string,
                            std::set<std::string>& solutions);
std::set<std::string> BuildSolutions(seen_map& words_seen,
                                     const std::string& target,
                                     const std::string& guess);


/*
 * given a lexicon, a guess word, and a target word, this function finds and returns all best
 * word-ladders. It finds the word-ladders via a fairly standard BFS. Each section is commented
 * in more detail below.
 */
std::set<std::string> FindWordLadder(const std::unordered_set<std::string>& lexicon,
                                     const std::string& guess,
                                     const std::string& target) {

  /* error checking disabled for performance, spec guarantees valid inputs
  if (lexicon.find(target) == lexicon.end() ||
      guess.size() != target.size()) {
    NoSolution();
    return;
  }
  */

  /* map of words seen, the keys are the words seen, the values are the words they were seen from
   * map(word -> {parent1, distance}, {parent2, distance}, ..., {parentn, distance}) */
  seen_map words_seen(lexicon.size());

  // need to insert no parent for guess and a max distance to get to guess of 0
  words_seen[guess].insert({std::string(), 0});

  // queue of rungs (strings with an associated distance from start)
  std::queue<struct rung> rungs_q;
  rungs_q.push({guess, 0}); // guess is 0 rungs away (distance) from itself
  int max_distance = NOT_SET;

  // standard BFS algorithm with depth restriction for finding multiple optimal solutions
  while (!rungs_q.empty()) {

    // everything left on the queue exceeds the max search distance for valid solutions so break
    if (max_distance != NOT_SET && rungs_q.front().distance > max_distance) break;

    // get all words one letter different from our current word
    auto word = rungs_q.front().word;
    auto next_rungs = GetNextRungs(lexicon, words_seen, word, target,
                                   rungs_q.front().distance + 1);

    // put all these on the queue with +1 distance of their parent
    for (const auto& rung : next_rungs) {
      rungs_q.push({rung, rungs_q.front().distance + 1});
    }

    // if we have found the target, restrict word exploration via setting max distance
    if (max_distance == NOT_SET && words_seen.find(target) != words_seen.end()) {
      max_distance = rungs_q.front().distance;
    }

    rungs_q.pop();
  }

  // BFS is done so build solution strings and print them
  return BuildSolutions(words_seen, target, guess);

}


/*
 * given a word (curr_rung), this function permutes every letter of that word
 * through each letter in the alphabet and tests if the resulting word (adjacent_rung)
 * is a real word (in the lexicon). If the adjacent rung is real then we record that we
 * have seen it from curr_rung. If we see the same word multiple times (i.e. from
 * multiple parents) then we only add the new parent if it was found at a distance
 * <= * the distance it was previously found at - this ensures optimal paths.
 *
 * * : really only == because of how BFS works (encounters distances in >= order) but we
 * keep it as <= in case a different algorithm calls this function.
 */
std::vector<std::string> GetNextRungs(const std::unordered_set<std::string>& lexicon,
                                      seen_map& words_seen, const std::string& curr_rung,
                                      const std::string& target, const int& distance) {

  // all the valid words reached from the current word (i.e. adjacent rungs)
  std::vector<std::string> next_rungs {};
  std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

  std::string adjacent_rung = curr_rung;
  for (auto& ch : adjacent_rung) {

    // reset the adjacent rung to default so we are only permuting one location per loop below
    adjacent_rung = curr_rung;

    for (const auto& letter : alphabet) { // permute the current char to all letters a-z
      ch = letter;
      // if the adjacent rung is actually adjacent and a valid word (in lexicon)
      if (adjacent_rung != curr_rung && lexicon.find(adjacent_rung) != lexicon.end()) {

        // if we haven't previously examined the child rungs of this rung
        if (words_seen.find(adjacent_rung) == words_seen.end()) {

          // add this rung to the vector of rungs so its children are explored
          next_rungs.push_back(adjacent_rung);
          // indicate we have seen this rung from the current rung, with specified distance
          words_seen[adjacent_rung].insert({curr_rung, distance});

          // note: we use [] because we want it to create it if it doesn't yet exist

        } else if (distance <= words_seen[adjacent_rung].begin()->second) {

          // note: use of [] okay as control flow confirms it exists with element by this stage

          /* (1) only need to add curr_rung as parent to adjacent rung
           * if it is <= current parent distance */
          words_seen[adjacent_rung].insert({curr_rung, distance});
        }

        if (adjacent_rung == target) return next_rungs;
        /* We can do this without losing any solutions because
          if we have found the target, then any word made from
          curr_rung doesn't need to be checked because we now know
          curr_rung can be transformed straight into the solution
          hence curr_rung -> other words -> target is superfluous
          if we can just do curr_rung -> target directly
        */

      }
    }
  }

  return next_rungs;
}


/* try build solutions, if there are none indicate there are no solutions,
 * otherwise solutions are in the solution set (implicitly ordered), so print them */
std::set<std::string> BuildSolutions(seen_map& words_seen,
                                     const std::string& target,
                                     const std::string& guess) {

  std::set<std::string> solutions;
  BuildSolutionsWithRDFS(words_seen, target, guess, std::string(), solutions);

  return solutions;

}


/*
 * 2) use a recursive depth first search to recreate strings from walking from target backwards
 * i.e. for each of the current words parents, prepend that parent to a copy of the already
 * existing solution strings. if you reach guess as a parent then that string is completed
 * and can be added to the collection of solutions
 */
void BuildSolutionsWithRDFS(seen_map& words_seen, const std::string& curr_word,
                            const std::string& guess, std::string curr_string,
                            std::set<std::string>& solutions) {

  curr_string.insert(0, curr_word + " "); // prepend the current word to the current solution
  if (curr_word == guess) solutions.insert(curr_string); // finished solution so add to solutions

  for (const auto& word_pars : words_seen[curr_word]) { // [] okay because parents defs exist
    // pass a copy of the current solution path to be extended with this specific parent
    BuildSolutionsWithRDFS(words_seen, word_pars.first, guess, curr_string, solutions);
  }

}


/*
 * I provide a helper function for users to print solutions rather than them having
 * to iterate the set themselves.
 */
void PrintSolutions(const std::set<std::string>& solutions) {

  if (solutions.empty()) {
    std::cout << "No ladder found.\n";
    return;
  }

  std::cout << "Found ladder: ";
  for (const auto& string : solutions) {
    std::cout << string << "\n";
  }
}

/*
 * I use two main ideas to find and build solutions efficiently
 *
 * 1) finding solutions : when considering adjacent rungs of a current word (i.e. words
 * that are one different from the current word) we set a maximum distance and then we
 * no longer need to add parents of this word if they are > that set distance. This is
 * because if a word occurs in an optimal solution (opt) at x distance from the start,
 * then it cannot occur < x distance from the start in any other optimal solution (otherwise
 * we could create a solution shorter than opt by going from start to the given word
 * in < x steps, and then from that word to the destination). i.e. in simple terms, you
 * only want to store the shortest path to any given word (see LOC:141, 145)
 *
 * 2) building solution strings: if the above rule is applied, then we know that every parent
 * of the target string must be part of the solution (because we only stored the optimal
 * ways (i.e. parents) to get to any particular string. This rule applies recursively backwards
 * i.e. parents of target all must be part of at least 1 solution string each, all parents of
 * those parents also must be part of at least 1 solution string each, and so on right back to
 * guess (see the BuildSolutionsWithRDFS function for this logic). This simplifies building
 * solution strings as only valid rungs are stored (no trial and error to backtrace from target
 * to guess)
 *
 * */
