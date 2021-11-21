import re
import os, sys
import pydot

import lexer

def highlight_node_seq(G, seq, color='black'):
	for idx, n in enumerate(seq[:-1]):
		N1 = G.get_node(n)
		N1[0].set_style('bold')
		N1[0].set_color(color)
		N2 = G.get_node(seq[idx + 1])
		N2[0].set_style('bold')
		N2[0].set_color(color)
		E = G.get_edge(n, seq[idx + 1])
		for e in E:
			e.set_style('bold')
			e.set_color(color)

		#G.set_edge_style(n, seq[idx + 1], style='bold')
		#G.set_node_style(N1[0], style='bold')
		#G.set_node_style(N2[0], style='bold')
	return G

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
	table = dict()
	fn = [f[0] for f in func_names]
	for i, name in enumerate(fn):
		table[name] = arg_names[i]

	return table


def extract_func_def_list(fname, verbose=False):
	with open(fname) as f:
		if verbose:
			print("Extracting functions list from", fname)
		lines = f.readlines()
		res = search_pattern(pattern, lines, fname)
		return res


def func_call_var_remap(func_name, func_arg_table, caller):
	try:
		callee_args = func_arg_table[func_name]
	except KeyError:
		print(func_arg_table)
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


def get_interfunc_remap(label, def_table, l, r, verbose=False):
	res = re.search(r['exp'], label)
	if not res:
		# not matched
		return {}

	r['format'].update({
                        'left': res.group(2),
                        'func_name': res.group(4),
                        'arguments': res.group(5)
                    })
	f_name = r['format']['func_name']
	caller_args = func_call_var_remap(f_name, def_table, r)
	switched_caller_args = dict(zip(caller_args.values(), caller_args.keys()))
	if verbose:
		print("Assignment:", caller_args)
		print("Mnemonic '-->'", switched_caller_args)
	return caller_args


def append_var_chain(g, src, dst, caller_args, verbose=False):
	# add and specify the chain of variables assignment: var=arg
	last = dst
	current = src
	for k,v in caller_args.items():
		next = pydot.Node(label=k +" = "+v+";")
		next.set_name(k + " = "+v+";")
		g.add_node(next)
		if verbose:
			print("Node added:", next)
		ed = pydot.Edge(current,
                                   next,
                                   color="maroon",
                                   style ='solid',
                                   label="argpass")
						
		g.add_edge(ed)
		if verbose:
			print("Edge added:", ed)

		current = next

	g.add_edge(pydot.Edge(current,
                                   last,
                                   color="maroon",
                                   style ='solid',
                                   label="call"))

	return g

#===================================================================================
def prepare_interproc_graph_var_trans(g, def_table, verbose=False, rm_direct_calledge=True):
	for e in g.get_edges():
		src_n = e.get_source()
		dst_n = e.get_destination()
		src = g.get_node(src_n)[0]
		dst = g.get_node(dst_n)[0]
		src_label = src.get_attributes()['label'].replace("\\", "")[2:]
		dst_label = dst.get_attributes()['label'].replace("\\", "")[2:]
		src_match = re.search(lexer.glex.assign_function_call, src_label)
		dst_match = re.search(r'.*NTRY.*', dst_label)


		if verbose:
			print(src_n, src_match, src.get_attributes()['label'])
			print("|\nV")
			print(dst_n, dst_match, dst.get_attributes()['label'])
			print("----------")

		l = 'assign_function_call'
		r = {
			'exp': lexer.glex.assign_function_call,'format': {
				'left': "",
				'func_name': "",
				'arguments': "",
			}
		}
		
		if src_match is not None and dst_match is not None:
			if verbose:
				print(src_label,"-->", dst_label)
			caller_args = get_interfunc_remap(src_label, def_table, l, r)
			if verbose:
				print(caller_args)
			if len(caller_args) > 0:
				append_var_chain(g, src, dst, caller_args)
				if rm_direct_calledge:
					g.del_edge(e)
	return g


#===================================================================================

if __name__ == '__main__':
	"""
	test
	"""
	current_path = os.path.dirname(os.path.abspath("."))
	sys.path.append(os.path.join(current_path, "./preprocessor/lexer"))

	import lexer
	import pydot

	def_table = func_table(sys.argv[1])


	g=pydot.Dot()
	g = prepare_interproc_graph_var_trans(g, def_table)

	g.write_png('output.png')