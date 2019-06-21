/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
  to which you're certain you have covered all possibilities,
  and why you think your tests are that thorough.

How I am approaching testing {

  - my approach to testing is to try provide a test for each control flow of the program
  and for each theoretically different "input" i.e. an input that should succeed with a
  single solution vs an input that should succeed with multiple solutions vs an input that
  will provide no solutions etc. I will not however be providing tests for those types of
  input that will never be run (those that were guaranteed not to be given in the auto tests
  by the spec and by the staff).

  - thinking of different types of input: when calling the word ladder algorithm, there is
  either no solution, one solution, or many solutions. When there are solutions, the only
  thing we need to do is check/ensure they are optimal. This leads to the following tests;

    1. valid (same length and real words) inputs, but no solutions
    2. valid inputs, one and only one optimal solution (*doesn't print the un-optimal ones)
    3. valid inputs, multiple optimal solutions and multiple un-optimal solutions (*)
    4. valid inputs that could cause cycles doesn't cause cycles
    5. i've been thinking for quite a while now and i'm honestly beat, can't think
    of anything else to test, so i'll test these ^. If my thinking is correct then they
    also implicity test things like "make sure invalid solutions don't exist" e.g.
    can't do super -> duper -> doper -> dooer -> doorr -> doors (1 implicitly tests this).

  - justification for omitting some tests: I am aware that one should always test for all
  types of input to your program and shouldn't trust that users will adhere to the API and
  always provide valid/sensible inputs. However, I had already written some code and tested
  in the command line that these other types of invalid inputs were caught and the appropriate
  no solution message printed. Furthermore, the spec and the reference solution do not even
  agree with one another about these invalid inputs i.e. the spec says that auto word ladder
  is a path from one word to another in which you must change one letter per step and each
  intermediate word must be a real word, however, if you put "test" as the start and end word,
  then the reference solution returns "test test" as the word ladder. This seems inconsistent
  with the definition of a word ladder (one letter change was not enforced upon stepping) and
  it would probably make more sense to output there's no solution. Also, i feel it would
  be counter-productive for me to spend time testing for test cases that will never run.
  (also other subjects exist, and also trimesters (LOL), so I don't really want to spend
  my time writing tests for auto program that will only run once more on autotests after this
  and then never run again :S I must say this is the best/easiest testing framework i have
  ever used though :P)
}

The degree to which I am certain I have covered all cases {

  - as with testing anything, without proving that you have tested all cases you cannot
  really be sure you have. I am not 100% sure I have tested ALL cases (in general), BUT i
  am confident I have tested the cases that are most common to occur and the most
  meaningful less common cases.

  - I think it is worth specifically noting that I am not testing whether output strings
  are outputted alphabetically for one very good reason. I use a std::set of std::string's
  to store my solutions strings and this is guaranteed by the c++ standard to be sorted
  alphabetically (and if we cant trust the c++ standard then we cannot expect our
  programs to behave correctly for everything). Basically, by testing alphabetical order
  i would actually be testing the c++ stl and not really my own code

}

What makes my testing thorough {

  - what makes my testing thorough is the methodical approach I took to testing;
  1. I went through my implementation and at each different control flow I examined
  what caused each of the different paths to trigger and then provided a test case/input
  type that corresponds to that control flow running and checked to see the behaviour of
  that control flow was as expected/correct
  2. I re-read the spec and each time it provided some fact about the behaviour of the
  program I checked that I either had a test that already tests that behaviour or
  added a new one accordingly.

  note: lots of the implementation could not explicity be tested because it was not
  part of the public API but was instead tested as a result of testing the
  public FindWordLadder function.

}

*/

#include "assignments/wl/lexicon.h"
#include "assignments/wl/word_ladder.h"
#include "catch.h"

SCENARIO("two valid inputs with no solution should return no solutions") {
  GIVEN("two real words (from lexicon) with same length with no possible word-ladder") {
    auto lexicon = GetLexicon("assignments/wl/words.txt");
    std::string guess = "zoysia";
    std::string target = "jogger";

    REQUIRE((lexicon.find(guess) != lexicon.end() && lexicon.find(target) != lexicon.end()));
    REQUIRE(guess.size() == target.size());

    WHEN("we run the word-ladder algorithm on the two inputs") {
      auto solutions = FindWordLadder(lexicon, guess, target);

      THEN("we should get an empty set") { REQUIRE(solutions.empty()); }
    }
  }
}

SCENARIO("two valid inputs with a single optimal solution but many un-optimal solutions"
         "should only return the optimal solution") {
  GIVEN("two real words (in lexicon) with same length and multiple possible word-ladders") {
    auto lexicon = GetLexicon("assignments/wl/words.txt");
    std::string guess = "simple";
    std::string target = "dimple";

    REQUIRE((lexicon.find(guess) != lexicon.end() && lexicon.find(target) != lexicon.end()));
    REQUIRE(guess.size() == target.size());

    WHEN("we call the word-ladder function on these inputs") {
      auto solutions = FindWordLadder(lexicon, guess, target);
      THEN("we should only get one solution, simple->dimple, even though other longer "
           "word ladders are possible e.g. simple->pimple->dimple") {
        REQUIRE(solutions.size() == 1);
        REQUIRE(*solutions.begin() == "simple dimple ");  // allowed trailing white space
      }
    }
  }
}

SCENARIO("valid inputs with multiple optimal solutions should return all optimal solutions "
         "but no un-optimal solutions i.e. testing equal length optimal solutions") {
  GIVEN("two valid words with multiple optimal and non-optimal possible word-ladders") {
    auto lexicon = GetLexicon("assignments/wl/words.txt");
    std::string guess = "bat";
    std::string target = "ear";

    REQUIRE((lexicon.find(guess) != lexicon.end() && lexicon.find(target) != lexicon.end()));
    REQUIRE(guess.size() == target.size());

    WHEN("we call the word-ladder function on these inputs") {
      auto solutions = FindWordLadder(lexicon, guess, target);

      THEN("we should only get multiple optimal solutions and but no longer solutions") {
        /*
         * i.e. the fastest way to get a word ladder is to change directly to the
         * target word (so long as the intermediate words are valid). Hence changing the
         * b->e first gives bat->eat->ear which is one optimal solution, whilst changing
         * the r->t first gives bat->bar->ear which is the other optimal solution. It
         * is clear no other solutions can beat this, but it is clear that there are other
         * non-optimal solutions possible i.e. bat->cat->eat->ear
         */
        REQUIRE(solutions.size() == 2);
        auto it = solutions.begin();
        REQUIRE(*it == "bat bar ear ");
        it++;
        REQUIRE(*it == "bat eat ear ");
      }
    }
  }
}

SCENARIO("two valid words with the ability to create a cycle in their word-ladder"
         "solution should not create a cycle in their solution") {
  GIVEN("two valid words with the possibility to cycle") {
    auto lexicon = GetLexicon("assignments/wl/words.txt");
    std::string guess = "cat";
    std::string target = "bog";

    REQUIRE((lexicon.find(guess) != lexicon.end() && lexicon.find(target) != lexicon.end()));
    REQUIRE(guess.size() == target.size());

    WHEN("we run the word-ladder algorithm on the two inputs") {
      auto solutions = FindWordLadder(lexicon, guess, target);

      THEN("cycles should have been ignored") {
        // i.e. all solutions for this word-ladder have the ability
        // to cycle and give the impression of a larger optimal
        // solution than what there really is. i.e. because there
        // is two intermediate words, it is possible for the
        // second intermediate word to go back to the first intermediate
        // word e.g. cat -> cot -> cat -> ..
        std::string solution1 = *solutions.begin();
        REQUIRE(solution1.size() == 16);  // 4*3 letter words + 4 spaces
        // note: don't need to test that the rest conform because of previous test passing
      }
    }
  }
}
