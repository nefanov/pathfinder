import re

pattern=r'^([a-zA-Z_*$][a-zA-Z_*$0-9]*\s+)+([a-zA-Z_*$][a-zA-Z_*$0-9]*)\s*\((.*)\)'

def search_pattern(pattern, lines, fname):
	res = list()
	for loc, string in enumerate(lines):
		r = re.findall(pattern, string)
		if r != []:
			res.append((fname, loc, r))
	return res


def extract_func_def_list(fname):
	with open(fname) as f:
		lines = f.readlines()
		res = search_pattern(pattern, lines, fname)
		return res


if __name__ == '__main__':
	"""
	test
	"""
	import sys
	import pprint
	pprint.pprint(extract_func_def_list(sys.argv[1]))
