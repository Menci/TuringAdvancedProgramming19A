# A self implemented IEEE-754 Floating Point datatype
Initially, I implemented a extendable version of this to support multi-precision, but it's WAY toooooooo slow.
And I didn't implement the extendable division function, so I simply gave up and created a new version.

`main.c` is the new version which supports double precision.

`expandable-version.c` is the original version which can be easily modified to support multi-precision.

`test.sh` uses the checker to test `main.c`.
