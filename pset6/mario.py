import cs50

while True:
    print("Please give me a number of floors between 0 and 23 inclusive: ", end="")
    n = cs50.get_int()
    if n < 0 or n > 23:
        print("Error in floor number.")
    else:
        break
for i in range(n):
    print(" " * (n - i - 1), end="")
    print("#" * (i + 2))