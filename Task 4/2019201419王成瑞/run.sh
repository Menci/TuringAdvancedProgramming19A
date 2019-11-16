rm /tmp/main
gcc *.c -o /tmp/main
cd ..
python3 assembly-checker/ /tmp/main