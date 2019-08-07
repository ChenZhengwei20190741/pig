def ask_ok(prompt, retries = 4, complaint='yes or no, please!'):
	while True:
		ok = raw_input(prompt)
		if ok in ('y', 'ye', 'yes'):
			return True
		if ok in('n', 'no', 'nop', 'nope'):
			return False
		retries = retries - 1
		if retries < 0:
			raise IOError('refusenik user')
		print complaint


ask_ok('Do you really want to quit?')
ask_ok('OK to overwrite the file?', 2)	
ask_ok('Ok to overwrite the file?', 2, 'come on ,only yes or no!')		
