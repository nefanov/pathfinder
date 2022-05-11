import os, sys
from preprocessor import *
from editor.grammar_editor import *
import re


def editor_loop(cmd):
    G = Grammar()
    ckpt_storage = Ckpt()
    while cmd != "exit":
        cmd = input("\N{ESC}[32mpathfinder:\N{ESC}[33mgrammar editor\u001b[0m>>")
        parsed = re.split(r'[\s,]+', cmd)
        if parsed in ["help", "h"]:
            print("Grammar editor help: TO DO")
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
        else:
            print(cmd, ": unrecognized command")

def preprocessor_loop(cmd):
    while cmd != "exit":
        cmd = input("\N{ESC}[32mpathfinder:\N{ESC}[34mgraph preprocessor\u001b[0m>>")
        if cmd.startswith("help"):
            print("Graph preprocessor help: TO DO")
        else:
            print(cmd, ": unrecognized command")


def backend_loop(cmd):
    while cmd != "exit":
        cmd = input("\N{ESC}[32mpathfinder:\N{ESC}[35mbackend\u001b[0m>>")
        if cmd.startswith("help"):
            print("Backend help: TO DO")
        else:
            print(cmd, ": unrecognized command")


def postprocessor_loop(cmd):
    while cmd != "exit":
        cmd = input("\N{ESC}[32mpathfinder:\N{ESC}[36mpostprocessor\u001b[0m>>")
        if cmd.startswith("help"):
            print("Postprocessor help: TO DO")
        else:
            print(cmd, ": unrecognized command")


def main_loop(cmd):
    while cmd != "exit":
        cmd = input("\N{ESC}[32mpathfinder\u001b[0m>>")
        if cmd.startswith("config"):
            if "load" in cmd:
                pass
            elif "dump" in cmd:
                print("lol")
        elif cmd in ["editor","e"]:
            editor_loop(cmd)
        elif cmd.startswith("preprocessor"):
            preprocessor_loop(cmd)
        elif cmd.startswith("backend"):
            backend_loop(cmd)
        elif cmd.startswith("postprocessor"):
            postprocessor_loop(cmd)
        else:
            print(cmd,": unrecognized command")


if __name__ == '__main__':
    main_loop("start")