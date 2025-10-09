set pagination off

set $step_num = 0

b range.c:28
command 1
    silent
    set $step_num = $step_num + 1
    if $step_num >= 28 && $step_num <= 35
        printf "@@@ %d %d %d %d\n", start, stop, step, current
    end
    c
end

run -100 100 3
q
