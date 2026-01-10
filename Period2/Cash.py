from cs50 import get_float
#divmod
cents = get_float("Change Owed: ")
coins = 0
while (cents < 0):
    coins = (int(cents))

cents = round(cents * 100)
cents = int(cents)


#Quarters
coins += cents // 25
cents %= 25

#Dimes
coins += cents // 10
cents %= 10

#Nickels
coins += cents // 5
cents %= 5

#Pennies
coins += cents

print(int(coins))
