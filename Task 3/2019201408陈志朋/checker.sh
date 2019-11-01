while (true) do
gcc main.c -o main && ./main
gcc std.c -o std && ./std
gcc std_check.c -o std_check && ./std_check
if diff me.out std.out; then 
    echo "AC"
    else 
        echo "WA1"
        break
fi
if diff me.out std1.out; then 
    echo "AC"
    else
        echo "WA2"
        break
fi
done