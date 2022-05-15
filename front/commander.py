import sys
import subprocess
from preprocessor import *
from editor.grammar_editor import *
import re

config = {}


def editor_loop(cmd, ex_list=None):
    G = Grammar()
    ckpt_storage = Ckpt()
    while cmd != "exit":
        if not ex_list:
            cmd = input("\N{ESC}[32mpathfinder:\N{ESC}[33mgrammar editor\u001b[0m>>")
        else:
            cmd = ex_list.pop(0)
        parsed = re.split(r'[\s,]+', cmd)
        if parsed[0] in ["help", "h"]:
            print("Grammar editor help: InProgress")
            print("Commands:")
            print("\t(ar)\tadd rule LEFTSIDE -> RIGHTSIDE -- add rule")
            print("\t(rm)\tremove N -- remove grammar rule number N")
            print("\t(u)\tundo -- backtrack to previous grammar snapshot")
            print("\t(dcs)\t -- dump grammar checkpoints")
            print("\t(ckpt)\tcheckpoint -- forcely make grammar checkpoint")
            print("\t(rstr)\trestore -- forcely pop grammar checkpoint")
            print("\t(f,fin)\tfinalize RULENUM1 RULENUM2 .. RULENUMK -- finalize grammar as grammar for derivation of"
                  " sentencial form 'RULENUM1,RULENUM2,...,RULENUMK'")
            print("\t(q)\texit -- exit")

        elif parsed[0] == "ar" or (parsed[0] == "add" and parsed[1] == "rule"):
            try:
                arrow = parsed.index("->")
            except:
                print(cmd, "cmd wrong format")
                continue
            left, right = (parsed[arrow-1], parsed[arrow+1:])
            rargs = []
            for i, r in enumerate(right):
                rargs += [{'repeatable': False, 'optional': False, 'repeat_limit': -1}]
                r_pedantic = re.split(r'\.|=', r)
                if 'repeatable' in r_pedantic:
                    rargs[i]['repeatable'] = True
                if 'optional' in r_pedantic:
                    rargs[i]['optional'] = True
                try:
                    rargs[i]['repeat_limit'] = int(r_pedantic[r_pedantic.index("repeat_limit")+1])
                except:
                    pass

                right[i] = r_pedantic[0]
            rl = Rule(Term(left),
                         [Term(x, repeatable=rargs[i]['repeatable'],
                               optional=rargs[i]['optional'],
                               repeat_limit=rargs[i]['repeat_limit']) for i, x in enumerate(right)],
                      tag="manual").make_repeatable()
            G.P += rl
            G.print()
            ckpt_storage.push(G)
        elif parsed[0] == "pr" or (parsed[0] == "print"):
            G.print()
        elif parsed[0] in ["rm", "remove"]:
            G.P.pop(int(parsed[1]))
            G.print()
            ckpt_storage.push(G)
        elif parsed[0] in ["finalize", "fin", "f"]:
            bottom = [G.P[(int)(p)] for p in parsed[1:]]
            G.finalize(sequence=bottom)
            print("------- Finalized Grammar for bottom-up from", " ".join([str(x) for x in bottom]) + " -------")
            G.print()
        elif parsed[0] in ["exit","q"]:
            return
        elif parsed[0] in ['ckpt', 'checkpoint']:
            ckpt_storage.push(G)
        elif parsed[0] in ['rstr', 'restore', 'undo', 'u']:
            gt = ckpt_storage.pop()
            if gt:
                G = gt
                G.print()
        elif parsed[0] in ['u', 'undo']:
            _ = ckpt_storage.pop()
            gt = ckpt_storage.pop()
            if gt:
                G = gt
                G.print()
        elif parsed[0] in ['dcs']:
            for i, item in enumerate(ckpt_storage.stack):
                print("----------- Grammar checkpoint", i, "-----------")
                item.print()
            print("------ Grammar checkpoint stack bottom -------")
        elif parsed[0] in ['pg', 'print']:
            G.print()
        elif parsed[0] in ['sg', 'save']:
            replace_spaces = False
            try:
                replace_spaces = True if parsed[2] == "spaces-fix" else False
            except:
                pass
            G.save(parsed[1], replace_spaces=replace_spaces)

        else:
            print(cmd, ": unrecognized command")


def preprocessor_loop(cmd, ex_list=None):
    while cmd != "exit":
        cmd = input("\N{ESC}[32mpathfinder:\N{ESC}[34mgraph preprocessor\u001b[0m>>")
        if cmd.startswith("help"):
            print("Graph preprocessor help: TO DO")
        else:
            print(cmd, ": unrecognized command")


def backend_loop(cmd, ex_list=None):
    while cmd != "exit":
        if not ex_list:
            cmd = input("\N{ESC}[32mpathfinder:\N{ESC}[35mbackend\u001b[0m>>")
        else:
            cmd = ex_list.pop(0)
        parsed = re.split(r'[\s,]+', cmd)
        if cmd.startswith("help"):
            print("Backend help: TO DO")
        if parsed[0] in ["run","r"]:
            exec_seq = ["build/core"]
            f_name = ""
            cmd_arg = ["-spaced_rhs"]
            if parsed[1] in ["-fast", "fast", "-fst", "fst"]:
                f_name = parsed[2]
                cmd_arg += ["-fast"]
            elif parsed[1] in ["slow", "slw", "-slow", "-slw"]:
                f_name = parsed[2]
                cmd_arg += ["-slow"]
            exec_seq +=[f_name] + cmd_arg
            result = subprocess.run(
                exec_seq, capture_output=True, text=True)
            res_stdout = result.stdout
            res_stderr = result.stderr
            print(res_stdout, res_stderr)

        else:
            print(cmd, ": unrecognized command")




def postprocessor_loop(cmd):
    while cmd != "exit":
        cmd = input("\N{ESC}[32mpathfinder:\N{ESC}[36mpostprocessor\u001b[0m>>")
        if cmd.startswith("help"):
            print("Postprocessor help: TO DO")
        else:
            print(cmd, ": unrecognized command")


def main_loop(cmd, ex_list=None):
    while cmd != "exit":
        if not ex_list:
            cmd = input("\N{ESC}[32mpathfinder\u001b[0m>>")
        else:
            cmd = ex_list.pop(0)

        if cmd.startswith("config"):
            parsed = re.split(r'[\s,]+', cmd)
            print(parsed)
            if "load" in parsed:
                pass
            elif "dump" in parsed:
                print("------- Config ------\n", config)
            elif parsed[1] == "var":
                spl_parsed = parsed[2].split('=')
                config[spl_parsed[0]] = spl_parsed[1]
        elif cmd in ["editor","e"]:
            editor_loop(cmd, ex_list)
        elif cmd.startswith("preprocessor"):
            preprocessor_loop(cmd)
        elif cmd in ["backend","be","b"]:
            backend_loop(cmd, ex_list)
        elif cmd.startswith("postprocessor"):
            postprocessor_loop(cmd)
        elif cmd.startswith("exit"):
            sys.exit(0)
        else:
            print(cmd, ": unrecognized command")


if __name__ == '__main__':
    if len(sys.argv) > 2:
        if "-ex" in sys.argv:
            try:
                tokens = sys.argv[sys.argv.index("-ex")+1:]
                #tokens = re.split(r'[\s,]+', tok_string[1:-1])
                main_loop("start", ex_list=tokens)
            except Exception as e:
                print("Error on -ex option parsing:", e, "\nRunning in interactive mode...")

    main_loop("start")