Internals
=========

We make a thing called `screen`, which is a buffer (`char*`) of what the final screen should
be. It would of course be composed of a section of `data` (also a `char*`).