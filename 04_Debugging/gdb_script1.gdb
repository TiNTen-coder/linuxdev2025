set pagination off

b range.c:28 if current % 5 == 0
command 1
    silent
    printf "@@@ %d %d %d %d\n", start, stop, step, current
    c
end

run 1 12
q
