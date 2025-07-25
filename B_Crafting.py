def solve():
    n = int(input())
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))

    for i in range(n):
        if b[i] > a[i]:
            needed = b[i] - a[i]
            for j in range(n):
                if i != j:
                    if a[j] - b[j] < needed:
                        print("NO")
                        return
    print("YES")

t = int(input())
for _ in range(t):
    solve()