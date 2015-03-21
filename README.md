# MonteCarlo
Monte Carlo Integration with an Arduino Due

Both versions use the Stacklist library created by Efstathios Chatzikyriakidis (contact@efxa.org), which provides a simple stack data structure, used for a (kind of hacky) post-fix expression evaluator.

The monte_carlo_integration_using_entropy_library version uses, as the name implies, the Entropy library created by Walter Anderson. This, in theory, provides a better source of entropy for the PRNG, although in testing results were not always so clear-cut.

The expression evaluator is a little kludgy, but it works well enough (although it is not particularly efficient). It can handle addition, subtraction, multiplication, division, and exponentiation; this is enough to work with simple polynomials at least. For the sake of simplicity, all letters are treated as representing the same variable.

I have found good results running the integration over about 100k to 200k iterations, with sharply decreasing returns after that, in addition to the amount of time that is required to do additional calculations.

I wouldn't bother trying to run this on anything less powerful than the Due for fairly obvious reasons.
