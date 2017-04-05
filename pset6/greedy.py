import cs50

while True:
    print("Please give a positive cash amount (eg. 2.50): ", end="")
    n = cs50.get_float()
    if n < 0:
        print("Positive amounts only.")
    else:
        break

cents = round(n*100)
quarters = int(cents/25)
change = cents % 25

dimes = int(change/10)
change = change % 10

nickels = int(change/5)
pennies = change % 5

coins = quarters + dimes + nickels + pennies

print(coins)