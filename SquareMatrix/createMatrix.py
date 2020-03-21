from random import randint
import sys
F = open("input.txt","w")
if len(sys.argv) > 1  and sys.argv[1].isnumeric():
	N = int(sys.argv[1])
else:
	N = 4

F.write(f'{N}\n')

for _ in range(N):
	for _ in range(N):
		F.write(f'{randint(0,9)} ')
	F.write('\n')
F.write('\n')
for _ in range(N):
	for _ in range(N):
		F.write(f'{randint(0,9)} ')
	F.write('\n')
F.close()
