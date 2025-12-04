with open("input_p1.txt", "r", encoding="utf-8") as file:
    left = []
    right = []
    for line in file:
        leftnum, rightnum = line.split("   ")
        left.append(int(leftnum))
        right.append(int(rightnum))

if len(left) == len(right):
    left.sort()
    right.sort()

    total_diff = 0
    for (
        left_num,
        right_num,
    ) in zip(left, right):
        total_diff += abs(left_num - right_num)

    print(total_diff)
