# week 1

# week 2

# week 3

## scope
* the scope of a variable is the part of the program where it is accessible
* scope starts at variable definition (you obviously can't use something that is undefined because, well, it doesn't exist before it is defined)
  * remember that declaration does not allocate any memory, definition does, hence definition is the actual point at which the declared thing exists
* scope usually ends at the next closing brace "}" (but it won't in some cases e.g. global variable, static variable)
* define variables as close to first use as possible (WOO! finally some sense! 1511 "all vars at top of file" style sucks ass)

## l-values and r-values
* r-value : in simple terms, an r-value is a piece of un-named memory.
* reference : a reference is just a new name for an existing piece of memory.  

* l-value reference : a reference (new name) to an l-value (piece of already named memory). Denoted by T&
* r-value reference : a reference (new name) to an r-value (piece of un-named memory). Denoted by T&&  

**differences in passing**  
* pass by copy : argument object remains un-modified on the calling side because it is a copy that is passed to the function. This is sometimes the desired outcome but can be slow for large types. The called function can obviously do what it likes with its copy. TLDR: un-modified original, possibly slow copy, called function can do what it likes with copy
* pass by l-value reference : the called function operates on the actual argument object via its own new name for that object. Const-ness can be used to enforce immutability of the object if needed. It is slower to pass a reference than to copy for primitive types but much faster to call by reference for large types (no copying done, only overhead is making a new reference/name). TLDR: un-modified or modified original depending on const-ness, same overhead regardless of type, called function can do what it likes with object with respect to const-ness of the reference.
* pass by r-value reference :
  * thus it is clear that the point of r-value references is to get copy behaviour without having to make a copy (you can think of this as object recycling, we repurpose an existing object that we know is no longer needed instead of making a new one)
* if you pass an argument to a reference type parameter foo (whether l-value or r-value reference), the object will not be copied as that is the whole point of references (instead access already existing piece of memory through the new name, foo).
* the first two points reveal why we use r-value references (T&&) for accessing r-values. To explain, your r-value is just a piece of un-named memory, and so you pass it to an r-value reference e.g. T&& foo, to bind foo as a new name to that piece of memory. This makes it an l-value from the perspective of the scope who did the binding of foo to the piece of un-named memory (the r-value) i.e. a T&& parameter is an l-value inside that function
  * but why not use a l-value reference (T& bar) to add the new name bar to the piece of memory? hmm
* If you are passing an 
* if you want to pass an argument to a r-value reference parameter (T&&) then that argument must either already be an r-value or you must explicitly cast it to an r-value with std::move().
* std::move : this can be thought of as removing the names from an l-value piece of memory to make it into an r-value. 


# misc

## terminology
