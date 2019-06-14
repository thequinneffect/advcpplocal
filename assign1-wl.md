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
* can we multi-thread to have multiple things running? a thread working on each n'th letter perms?
