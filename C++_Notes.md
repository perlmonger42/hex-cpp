
It is intended that quadsets be constexpr constructible, so that methods like
left\_column() can be evaluated at compile time.

The constexpr feature was added in C++11.

CONSTEXPR
=========

For a function to be constexpr, the following requirements must be met:
1.  The function must have non-void return type. This requirement is
    trivially met in the case of a constructor.
2.  The function body canot declare variables or define new types.
3.  The body may only contain declarations, null statements, and a single
    return statement.
4a. [non-constructor] There must exist argument values such that, after
    argument substitution, the expression in the return statement produces
    a constant expression.
4b. [constructor] There must exist argument values such that, after
    argument substitution, it initializes the class's members with
    constant expressions.
5.  [constructor] The destructors for the types must be trivial.
Constexpr member functions are implicitly const (relative to ‹this›).

C++14 relaxed some of the constexpr constraints.
1.  Declarations are now permitted, except for ‹static› or ‹thread\_local›
    variables, and variable declarations without intializers.
2.  ‹goto› statements are forbidden.
3.  ‹if› and ‹switch› are permitted.
3.  All looping statements are permitted, including range-based ‹for›.
4.  Expressions may change the value of an object if the lifetime of that
    object began within the constant expression function. This includes
    calls to any non-‹const› ‹constexpr›-declared non-static member
    functions.
In C++14, constexpr member functions can be non-‹const› (relative to ‹this›).
However, a non-‹const› ‹constexpr› member function can only modify a class
member if that object's lifetime began within the constant expression
evaluation.


Despite all the inlining possible with a good C++ compiler, constexpr
doesn't seem to do as well as POD at speeding up my code. When I created
a version of these classes that had constexpr constructors and methods, but
were not POD, Benchmark\_SimpleVirtualWinner showed a slowdown from 1336ns/op
to 2264ns/op.


POD (PLAIN OLD DATA)
====================

A class is POD if it is trival, standard-layout, and all of its non-static
data members and base classes are POD.

A trivial class or struct is one that:
1.  Has a trivial default constructor. In other words, either it has no
    constructor, or uses the default constructor syntax:
      SomeConstructor() = default;
2.  Has trivial copy and move constructors, which may use the default
    syntax.
3.  Has trivial copy and move assignment operators, which may use the
    default syntax.
4.  Has a trivial destructor, which must not be virtual.

A standard-layout class or struct is one that:
1.  Has no virtual functions.
2.  Has no virtual base classes.
3.  All of its non-static data members have the same access control
    (public, private, protected).
4.  All its non-static data members, including any in its base classes,
    are in the same one class in the hierarchy. In other words, only
    one class in the inheritance chain may contribute instance data
    members.
5.  These rules also apply to all the base classes and to all non-static
    data members in the class hierarchy.
6.  Has no base classes of the same type as the first defined non-static
    data member.


AGGREGATE INITIALIZATION
========================

A class type may be aggregate initialized only if it has
  1. No private or protected members.
  2. No user-provided constructors.
  3. No base classes.
  4. No virtual members.
  5. No brace-or-equal initializers for non-static members (until C++14).

<!-- Vim: set ft=markdown: -->
