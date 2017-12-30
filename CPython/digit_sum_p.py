def digit_sum(x):
    ans = 0
    while x != 0:
        ans += x % 10
        x /= 10
    return ans


s = 0
for x in range(100000000):
    s += digit_sum(x)

print(s)
