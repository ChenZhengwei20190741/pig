#!/usr/bin/python
#coding=utf-8

numbers = [12, 37, 5, 42, 8, 3]
even = []
odd = []
while len(numbers) > 0 :
    number = numbers.pop()
    if (number % 2 == 0):
	even.append(number)
    else:
	odd.append(number)
print "质数：",even,'\n'
print "非质数：",odd
