gcc zip.c -o zip && ./zip
gcc hfm.c -o hfm && ./hfm
rm data.in
gcc rehfm.c -o rehfm && ./rehfm
gcc rezip.c -o rezip && ./rezip
rm me.out
gcc std.c -o std && ./std
diff test.out std.out
rm std.out test.out
rm zip hfm rehfm rezip std