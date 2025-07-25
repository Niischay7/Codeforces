def solve():
    t = int(input())
    for _ in range(t):
        n = int(input())
        a = list(map(int, input().split()))

        ans = 0
        for i in range(n):
            ans += a[i]

        while len(a) > 1:
            temp_list = []

            for i in range(len(a) - 1):
                temp_list.append(a[i + 1] - a[i])

            cnt = 0
            for temp in temp_list:
                cnt += temp

            if cnt < 0:
                if cnt > ans:
                    ans = cnt
                for i in range(len(temp_list)):
                    temp_list[i] = -temp_list[i]

            a = temp_list[::-1]

        if a[0] > ans:
            ans = a[0]

        print(ans)

if __name__ == "__main__":
    solve()
