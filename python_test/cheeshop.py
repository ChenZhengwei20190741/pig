def cheeseshop(kind, *arguments, **keywords):
	print "--Do you have any",kind, "?"
	print("--I'm sorry ,we 're all out off",kind)
	for arg in arguments:
		print arg
	print arg
	print "-"*40
	keys = sorted(keywords.keys())
	for kw in keys:
		print kw, ":", keywords[kw]

cheeseshop("limburger", "It's very runny, sir.", "It's really very, VERY runny,sir",shopkeeper = "Michael,Palin", client="John cleese", sketch="Cheese shop Sketch")
