import os
import sys
import subprocess
from networkx.generators.social import les_miserables_graph
import pydot

import extra_py_utils
from extra_py_utils import *

current_path = os.path.dirname(os.path.abspath("."))
sys.path.append(os.path.join(current_path, "preprocessor"))
sys.path.append(os.path.join(current_path, (".." + os.sep)*2 + "third-party"))

import mem_spec
import lexer.lexer as lexer


def prepare_custom_markup(scenario=None,
                          spec=mem_spec.specialize_prev_mem_dep,
                          need_interproc_pass=True,
                          config=None,
                          custom_tokens=None,
                          front_dir="../../front"):
    if config:
        working_dir = config['working_dir']
        src = config['src']
        ir_dotfile = config['ir_dotfile']
        ir_pic = config['ir_pic']
        ir_markup = config['ir_markup']
    else:
        working_dir = os.path.join(current_path, "../front")
        src = "1.c"
        ir_dotfile = "m.dot"
        ir_pic = "pic25.png"
        ir_markup = 'p_markup.png'

    print(working_dir)
    wdir = working_dir
    os.chdir(wdir)
    GCC_CMD = "-fdump-tree-cfg-graph"
    FRONT_CMD_SUFFIX=""
    if "SSA_MODE" in config.keys():
        GCC_CMD ="-fdump-tree-ssa-graph"
        FRONT_CMD_SUFFIX = "-SSA"

    os.system("gcc-9 " + GCC_CMD + " " + src + " && sync " + src +".*t.*.dot")
    os.system("echo "+src+" | "+front_dir+"/build/code2graph -cmd "+src+" -front-only -thin " + FRONT_CMD_SUFFIX +" > " +src + "_gcc_frontend.output")
    os.system("sync "+src+"_gcc_frontend.output")
    os.system("csplit " + src + "_gcc_frontend.output " +"'/digraph /'" + " > /dev/null")
    os.system("rm "+src+"_gcc_frontend.output && sync")
    os.system("cat xx01 > " + ir_dotfile + " && sync "+ ir_dotfile)
    os.system("rm xx*")
    os.system("sed -i '$ d' " + ir_dotfile +" && sync "+ ir_dotfile)
    os.system("rm "+ src +".*t.*")
    os.system("echo Picture generating from dotfile " + ir_dotfile + ", saving to: " +ir_pic)

    #os.system('./get_thin_graph.sh -i ' + os.path.join(wdir, src) + ' -s '+ os.path.join(wdir, ir_dotfile) + ' -p '+os.path.join(wdir,ir_pic))
    os.system("dot -Tpng " + ir_dotfile + " -o " + ir_pic)
    # os.system('mv ' + 'front' + os.sep + src + '.\\* '+ wdir)
    in_graph = os.path.join(wdir, ir_dotfile)
    if need_interproc_pass:
        g = pydot.graph_from_dot_file(in_graph)
        ft  = extra_py_utils.func_table(os.path.join(wdir, src)) #"../front/1.c")
        g[0] = extra_py_utils.prepare_interproc_graph_var_trans(g[0], ft)
        g[0].write_raw(in_graph)

    out_f = os.path.join(wdir, 'processed.dot')
    out_pic = os.path.join(wdir, ir_markup)
    os.chdir(current_path)
    if custom_tokens:
        for t in custom_tokens:
            pass
    return lexer.prepare_graph(in_graph, out_f, out_pic, need_graph_save=True, need_plot=True,
                            pattern_composer=mem_spec.compose_pattern,
                            specializer=spec,
                            scenario=scenario), working_dir


def preprocess_test1():
    print("Preprocessor test #1: default markup")
    conf = {
            "working_dir" : os.path.join(current_path, ".." + os.sep + "front" + os.sep + "tmp"),
            "src" : "1.c",
            "ir_dotfile" : "m.dot",
            "ir_pic" : "pic_ir_test_1.png",
            "ir_markup" : 'p_markup.png',
            "SSA_MODE" : "Y",
        }
    if not os.path.exists(conf["working_dir"]):
        os.makedirs(conf["working_dir"])
        f = open(conf["working_dir"]+ os.sep + "1.c","w+")
        f.write("int main(){return 0;}")
        f.close()

    (graph, mapping), working_dir = prepare_custom_markup(config=conf)
    print(mapping, working_dir)


if __name__ == '__main__':
    preprocess_test1()
