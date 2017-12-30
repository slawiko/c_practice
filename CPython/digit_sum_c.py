import digit_sum_module

s = 0
for x in range(100000000):
    s += digit_sum_module.DigitSum(x)

print(s)
