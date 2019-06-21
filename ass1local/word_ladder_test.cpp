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
  and then never run again :S )
}

The degree to which I am certain I have covered all cases {

  - as with testing anything, without proving that you have tested all cases you cannot
  really be sure you have. I am not 100% sure I have tested ALL cases (in general), BUT i
  am confident I have tested the cases that are most common to occur and the most
  meaningful less common cases.

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

}

*/

#include "assignments/wl/lexicon.h"
#include "assignments/wl/word_ladder.h"
#include "catch.h"

SCENARIO("two valid inputs with no solution should return no solutions") {

  GIVEN("two real words (from lexicon) with same length") {

    auto lexicon = GetLexicon("assignments/wl/words.txt");
    std::string guess = "zoysia";
    std::string target = "jogger";

    REQUIRE((lexicon.find(guess) != lexicon.end() &&
            lexicon.find(target) != lexicon.end()));

    WHEN("we run the worladder algorithm on the two inputs") {

      auto solutions = FindWordLadder(lexicon, guess, target);

      THEN("we should get an empty set") {

        REQUIRE(solutions.empty()); {

        }

      }

    }

  }
}

SCENARIO("scenario to test") {

  GIVEN("some setup") {

    REQUIRE("that setup succeeded");

    WHEN("something happens") {

      THEN("this should happen") {

        REQUIRE("that it did happen"); {

        }

      }

    }

  }
}

