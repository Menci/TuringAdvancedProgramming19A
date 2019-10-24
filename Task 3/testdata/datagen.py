from random import randint, choice

count_arg, count_var, count_expr = map(int, input().split(' '))

result = ""
for i in range(count_expr):
    var = "v" + str(i + 1)

    left_operand = ""
    if choice(['a', 'v']) == "a" or i < 1:
        left_operand = "a" + str(randint(1, count_arg))
    else:
        left_operand = "v" + str(randint(1, i))

    right_operand = ""
    right_operand_type = choice(['a', 'v', ''])
    if right_operand_type == "a":
        right_operand = "a" + str(randint(1, count_arg))
    elif right_operand_type == "v" and i >= 1:
        right_operand = "v" + str(randint(1, i))
    else:
        right_operand = str(randint(-32768, 32767))


    line = "%s = %s %s %s;" % (var, left_operand, choice('+-*/'), right_operand)
    result += line + "\n"

print(result)
exec('\n'.join(["a" + str(i) + " = " + str(i) for i in range(1, count_arg + 1)]) + "\n" + result)
