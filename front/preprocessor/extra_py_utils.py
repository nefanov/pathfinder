import re
import os, sys

def get_function_args(arg_str):
    spl = arg_str.split(",")
    return spl


def parse_function_call_args(item):
    if item['exp'] == lexer.glex.assign_function_call:
        arg_list = get_function_args(item['format']['arguments'])
        return arg_list
    return None


def parse_function_def_args(st):
    arg_list = get_function_args(st)
    if len(arg_list[0]) == 0:
        return arg_list
    return [pos.split()[-1][1:]
            if pos.split()[-1][0] == '*'
            else pos.split()[-1]
            for pos in arg_list] # maybe should be rewritten -- pointer matters...

#===================================================================================

pattern=r'^([a-zA-Z_*$][a-zA-Z_*$0-9]*\s+)+([a-zA-Z_*$][a-zA-Z_*$0-9]*)\s*\((.*)\)'

def search_pattern(pattern, lines, fname):
	res = list()
	for loc, string in enumerate(lines):
		r = re.findall(pattern, string)
		if r != []:
			res.append((fname, loc, r))
	return res


def make_func_def_table(func_names, arg_names):
	table=dict()
	fn = [f[0] for f in func_names]
	for i, name in enumerate(fn):
		table[name] = arg_names[i]

	return table



def extract_func_def_list(fname):
	with open(fname) as f:
		lines = f.readlines()
		res = search_pattern(pattern, lines, fname)
		return res


def func_call_var_remap(func_name, func_arg_table, caller):
	try:
		callee_args = func_arg_table[func_name]
	except KeyError:
		print(func_name, ": function name not defined in sources")
		sys.exit(0)
		return {}

	remap = dict()
	
	try:	
		for i,item in enumerate(parse_function_call_args(caller)):
			remap[callee_args[i]] = item
	except Exception as e:
		print("func call args parsing exception",e)
	return remap

#===================================================================================
def func_table(fn):
		fl = extract_func_def_list(fn)	
		arg_names = [parse_function_def_args(it[-1]) for it in [el[-1] for el in [item[-1] for item in fl]]]
		func_names =[parse_function_def_args(it[-2]) for it in [el[-1] for el in [item[-1] for item in fl]]]
		def_table = make_func_def_table(func_names, arg_names)
		return def_table


def get_interfunc_remap(label, def_table, l, r):
	res = re.search(r['exp'], label)
	if not res:
		print("Not matched")

	r['format'].update({
                        'left': res.group(2),
                        'func_name': res.group(3),
                        'arguments': res.group(4)
                    })

	f_name = r['format']['func_name']
	caller_args = func_call_var_remap(f_name, def_table, r)
	switched_caller_args = dict(zip(caller_args.values(), caller_args.keys()))
	print("Assignment:", caller_args)
	print("Mnemonic '-->'", switched_caller_args)
#===================================================================================

if __name__ == '__main__':
	"""
	test
	"""
	current_path = os.path.dirname(os.path.abspath("."))
	sys.path.append(os.path.join(current_path, "./preprocessor/lexer"))
	import pprint
	import lexer

	def_table = func_table(sys.argv[1])

	label = r'D.1 = m(1,2);'
	l = 'assign_function_call'
	r = {
		'exp': lexer.glex.assign_function_call,'format': {
			'left': "",
			'func_name': "",
			'arguments': "",
		}
	}
	get_interfunc_remap(label, def_table, l, r )
	
	
