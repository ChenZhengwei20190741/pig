#while True:
#	pass

def inilog(*args):
	pass

def fib(n):
	a,b = 0, 1
	while a < n:
		print a,
		a,b = b, a+b

f = fib
f(100)

def fib2(n):
	result = []
	a,b = 0, 1
	while a < n:
		result.append(a)
		a,b = b, a+b
	return result

f100 = fib2(100)
print f100
print f(0),'\n'
print fib2(0)
