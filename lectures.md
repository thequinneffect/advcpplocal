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
* read more here and add to this: https://stackoverflow.com/questions/3413470/what-is-stdmove-and-when-should-it-be-used

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

# week 4

## friend? (read and add https://www.quora.com/What-are-the-differences-between-a-member-operator-function-and-a-friend-operator-function)
* how do you know whether to make a function a member function or just a friend function?
  * use members when the operation is called in the context of a particular instance
  * use friends when the operation is called without any particular instance (even if they don't require access to private details i.e. they can get all they need though public access) TODO: why make them friends if they don't have to be? isn't this bad encapsulation?

# week 5 (wed: skip to 8:00, fri: start at start but skip  0:39 to 1:15:30)

## Ownership
* **ownership** : you can think of ownership as a responsibility for correct cleanup of a resource. You own it, you perform the correct clean up procedure for the current situation.
  * the term clean-up changes depending on context : delete for new, close for open etc.
  * just because you own something doesn't mean you will actually clean it up (sometimes it does, single ownership), it just means you are responsible for the cleanup IF it needs to be done (sometimes only a ref count needs to be updated, shared ownership)
* **single/unique ownership** : the resource is owned by a single object. Once this owning object no longer exists then the resource is released.
* **shared ownership** : the resource is shared between many objects and is only released once all objects no longer exist. This requires reference counts and is important to prevent a double/multiple free on the same pointer (which causes undefined behaviour). This is useful if you have a very expensive to copy resource.
* it is easy to picture the difference of unique/single ownership and shared ownership as just 1 pointer pointing to something (unqiue/single) vs many pointers pointing to something (shared). 
* **Pointer Ownership**
* determining whether a pointer (of any kind) has ownership, and whether that ownership is shared, is easy as long as you understand what ownership is (to have ownership over a resource means to have the responsibility to clean it up when required) 
  1. unique ptr : has ownership because it is responsible for freeing the un-named data. As the name suggests, this ownership is not shared
  2. shared ptr : has ownership because it is responsible for possibly having to free the un-named data. As the name suggests, this ownership can be shared and uses reference counts to do so
  3. weak ptr : a shared pointer that does not increment the reference count. Because it doesn't increment the reference count, it has no responsibility to free and thus is non-owning (shared ownership therefore cannot exist)
  4. raw c ptr/observer ptr : no ownership (reason stated many times), and hence non-sharing also
  

## Object lifetimes
* we attach the lifetime of an object to something else to create safe object lifetimes i.e. to ensure that an object will be destroyed once it is no longer needed
* remember that everything is an object
* **named objects (stack objects);**
  * variables are tied to their scope
  * a data member (of a class) is tied to the lifetime of that class instance (unless static)
  * an element in a std::vector is tied to the lifetime of the vector
* **un-named objects (heap objects);**
  * heap objects should be tied to their owner
    * an owning raw pointer (raw pointer that points to the heap data) is tied to its scope/owning class instance, but the difference is that when it is destroyed (goes out of scope, owning class instance is destructed), the data that it points to is not cleaned up. 
    * a c-style array is exactly the same as the above (array variable is just a pointer) except that you don't clean up not only 1 element, but n.
* remember that using the heap for an object is essentially saying "this object needs its own space AND its OWN LIFETIME"
* in c++ we tend to want to favour named/stack objects because that memory is managed for us (the combination of the stack frame protocol popping things, and c++ calling objects destructors when they are popped from the stack handles this)

## Smart Pointers (i.e. favouring named/stack objects over un-named heap objects)
* the rationale is that you can choose between;
  * raw pointers that point to "new" memory on the heap which is not freed unless "delete" is explicitly called. The heap memory is not freed when the raw pointer is popped from the stack frame.
    * yes a raw pointer is technically a named/stack object too, but it isn't owning and so doesn't hold-up the clean-up contract that owning demands
  * smart pointers (classes, and therefore named/stack objects) that abstract away the new into their constructor and the delete into their destructor. Hence when the smart pointer class instance is popped from the stack frame, the destructor runs which has been implemented to free the allocated heap memory! Clearly this is better.
* **common combinations**  
  1. unique ptr + raw ptrs (observers)
  2. shared ptr + weak ptr/raw ptrs (observers)  
* use shared pointers when you need multiple pointers to a resource AND MORE IMPORTANTLY you don't know which one will live the longest (if you know which one will live the longest, you just make it a unique ptr and the rest observers/raw ptrs)
**avoid using new and delete** 
* "unique_ptr<LongTypeName> up{new LongTypeName(args)}" must mention LongTypeName twice, while "auto up = make_unique<LongTypeName>(args)" mentions it once - this is one very small benefit for using make_* instead of new
* compilers evaluate function arguments in different orders, some left to right, others right to left. Therefore if you call a function foo(new int, new double) then if one of these succeeds but the second one fails, the first one will be leaked (not free'd). You can fix this with smart pointers, foo(std::make_unique<int>(), std::make_unique<double>())
* int* i = new int; std::unique_ptr<int> up1{i}, std::unique_ptr<int> up2{i}, ... , std::unique_ptr<int> upN{i}. As you can see, if you use new and have a raw pointer to it then you can have it owned by many unique pointers which doesn't make sense and will cause a crash on the second attempt to free (smart pointers aren't smart enought to get around this). By using auto up = std::make_unique<T>(val), you encapsulate the allocation within the make_unique function and so the only way to copy it would be to copy construct or copy assign it (and smart pointers ARE smart enough to defend against this!)
* technically std::unique_ptr<T> up{new T} encapsulates the allocation as well (meaning the resource could not be re-used in any other way except for copy construction/copy assignment as well) but the problem is that this is bad for exception safety/stack unwinding (TODO: add once i know this)
**TODO: when you should/have to use new**
**release vs reset (AKA: WHY THE FUCK DIDN'T THEY CHOOSE BETTER NAMES FOR THESE**
* release : releases ownership and returns pointer to resource (reLease, L for leakable)
* reset : deletes the currently owned resource

## Exceptions
* allows us to recover from critical but recoverable errors, instead of just terminating the program
* exceptions are classes (objects) (std::exception is the parent class of all exceptions)
* The reason we have exceptions as classes is it provides a generic and consistent interface to handle exceptions thrown by the standard library. All the exceptions generated by the standard library are inherited from std::exception.
  * also, this allows you to make your own sub-class of std::exception for making custom exception handlers. They can then handle both std library exceptions as well as your own custom exceptions
* throw by value, not by pointer i.e. throw Exception() NOT throw new Exception()
* catch by const reference i.e. catch(const ExceptionType& e)
  * const : because there is no reason to change an exception object that is caught
  * reference : avoids copy so it is faster AND prevents slicing (say you throw a sub-class but you catch statement has a parent-class, then because you don't have a reference you are passing by value and hence it will do a copy. Because the type you specified to create was the parent class, it is going to use its copy constructor and this will obviously throw away any extra parts the sub-class had i.e. slices them off)

# week 6 - templates (fri: skip to 3:50, skip 51:30 - 1:00:53 | wed: skip to

## Polymorphism in C++
* Polymorphism is when you have the same single interface (API) being implemented with multiple different implementations (say version A vs version B etc.). Then, depending on whether you have an A or B, you will use the A or B version/implementation of the API.
* C++ has two types of polymorphism, static and dynamic
* **static polymorphism** : as the name suggests, this is compile-time polymorphism. This involves both function overloading (multiple function prototypes with the same name but different parameters) and templates (implementation of function logic without specifying actual types)
* **dynamic polymorphism** : as the name suggests, this is run-time polymorphism (BUT it can also be compile-time depending on the variable type). This is all quite complicated and is explored later (week 8).

## Generic Programming
* generic programming is programming where you write software components (methods, classes etc.) that are independant of types
  * it is the implementation of the logic i.e. the logic for a min algorithm doesn't change just because the data type being compared changes (the only thing that changes when the data type changes is how the comparison of the two variables of said data type are compared e.g. for an int you just see which one is lesser, for a string it is alphabetic, for a student class it could be comparing by zid etc.)
  * because the logic is the same regardless of data type, as long as the data type being passed in has correctly implemented the operations that the generic algorithm needs, then it will work (like above for min the data type must have operator< defined)

## Why Templates?
* without templates, we cannot program generically - that is, we cannot create a function which accepts and applies the same logic to two very different data types. Instead, we must create two versions of the same function i.e. function overloading (see slide 3.1 of lecture 6.2)

## Templates
**Function Templates**  
* first of all, function templates are NOT CODE, they are instructions to generate code
* secondly, the generation of a function (from a function template) for a particular type only happens when a call to the templated function is seen during compile time i.e. if you never parameterize a templated function then no actual functions will be generated (the compiler only generates what you parameterize)
* you can think of templates as making the compiler generate function overloads for you
  * this means that compiling takes longer (because the compiler has to compile each templated function n times - where n is the number of unique ways you parameterized that specific function template). This also mean that run-time is faster because we have the instructions specific to each type ready to go (no overhead at runtime, just call the matching function). 
  * this is an example of how C++ shifts more work to compile time to have a faster run time (same motivation as constexpr)
* you make a templated function by using the keyword template. Then you put however many **template type parameters** you need inside the **template paramter list**
~~~
template <typename T> // this is the template parameter list. T itself is a template type parameter
T min(T a, T b) {
 return a < b ? a : b;
}
~~~
* confusingly (and somewhat backwards), template type parameters (i.e. placeholders for types that themselves don't have a concrete type) are called **type parameters**. **Non-type parameters** are any actual concrete types (but they have no default value)
  * it's easier to think of these as "typename parameters" and "non-typename parameters" becuase you use the keyword typename when there is no concrete type and the concrete type otherwise.
  * another way to remember it is that you parameterize type parameters with types (like char, int, std::string etc.), whilst you parameterize non-type parameters with values (1, 3.0, 'a', "hello" etc.)
~~~
template<typename T, int size, double scaler>
foo(T lhs, T rhs) { ... } 
~~~
* T is a type parameter, size and scaler are both non-type parameters. Remember you can figure this out because T uses typename and so must be a type parameter but size and scaler use int and double and so are non-type paramters (they don't use typename). The easiest way to figure it out is to imagine parameterizing it : foo<char, 5, 100>('a', 'b'), as you can see, the T's are parameterized with a type (char). making them type parameters, yet scaler and size are parameterized with values (making them non-type parameters)
* non-type parameters are often used for passing things like container size e.g. "int array_size or int queue_size". The advantage of this is improved performance - the size of the container is determined at compile time requiring less to be done/processed at runtime. The disadvantages are 1. code explosion - instantiation created for a queue of every size and 2. you are unable to copy construct easily (for templated classes)  

**Class Templates**  

## Template Declarations and Definitions
* when you declare a template component (function or class), you must write the template parameter list (i.e. template <...>) above the class name or function name (except for when a function declaration is made inside a templated class, then it doesn't require it)
  * you can think of it like scopes. If you write the template paramter list above the class, then everything inside the class knows what the template parameters are
  * but that doesn't mean a method defined outside the class knows about them, so you must add the template paramter list above the method
  * tldr: the compiler won't know what T means if you don't have the template paramter list including T in it for each scope it is used in
~~~
template <typename T>
class MyStack
{
 void Push(T&);
 std::vector stack_;
}
~~~
* also, when you define a templated class's member function otuside the class, not only do you have to add the template parameter list as described above, but you must parameterize the class name with the appropriate template parameter
~~~
// this is below ^ in the file
template <typename T>
void MyStack<T>::Push(T& element) { stack_.push_back(element); }
~~~

# misc

## defaulting functions and the rule of five (or 6?)/special member functions
* C++ has 6 special member functions: ctor, dtor, copy ctor, move ctor, copy assign, move assign
* if you do not explicitly 

## terminology
