def parrot(voltage, state='a stiff', action='voom', type='Norwegian Blue'):
	print "-- this parrot wouldn't", action
	print "if you put", voltage, "volts through it."
	print "-- Lovely plumage, the", type
	print "-- it's", state, "!"

parrot(1000)
parrot(voltage = 1000)
parrot(voltage = 1000000, action='VOOOOOM')
