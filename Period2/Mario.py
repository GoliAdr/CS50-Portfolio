while True:
    n = int(input("Height: "))
    if n > 0 and n < 9:
        break

for i in range(0, n, 1):
    for j in range(0, n, 1):
        if (i + j < n - 1):
            print(" ", end="")
        else:
            print("#", end="")
    print()



