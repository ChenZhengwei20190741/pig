#x = int(raw_input("please enter an integer:"))

#if x < 0:
#	x = 0
#	print "Negative changed to zero"
#elif x == 0:
#	print "Zero"
#elif x == 1:
#	print "single"
#else:
#	print "More"

words = ['cat', 'window', 'defenstrate']

for w in words[:]:
	if len(w) > 4:
		words.insert(0, w)

print words

