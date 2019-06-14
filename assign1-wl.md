# word-ladder

## BFS
* given a word in the lexicon, e.g. dog, we need to get every intermediate word that is in the lexicon e.g;
  1. first letter perms: aog, bog, cog, dog, eog, fog, gog, hog, iog, jog, kog, log, ...
  2. second letter perms: dag, dbg, dcg, ddg, deg, dfg, dgg, ...
  3. third letter perms: doa, dob, doc, dod, doe, dof, ...
  * obviously adding to the BFS queue if they are real words (i.e. in the lexicon)
* now, does this have any problems? and can it be optimised?
  1. it has a cycling problem : we see when permuting the first letter, we find dog but this was our starting word! the same happens for the second and third letters (when we get o and g perms respecitvely)
    * this needs to be dealt with, how?
  2. this is brute force : we are trying all 26 letters for each letter in the word i.e. 26*strlen() permutations to check
    * can we narrow this down?

## ideas
* **priority BFS?** : prioritising specific search paths i.e. if we want dog -> cot then for permutations of the first word, even though bog is a legit word, we want to explore cog first because it has a 2/3 match with the result word. Priority queue?
  * we use a scoring function, the more same-position, same-letter matches the current word and the target word have, the better
  * put these initially into pools of their score before throwing them onto the BFS queue?
  * not every word has a vowel, but almost all of them do. maybe this can be leveraged? this is more later level optimisations though.
* **for checking re-encounters** : make an unordered_set of pairs {word, bool}. Then just need to throw a word into the set and check whether the bool is set or not, if it is, we have seen it before, if not, we haven't so we can use it, but we must set it now because we are going to use it. use an unordered set because it has O(1) access.
