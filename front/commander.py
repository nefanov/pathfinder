import os, sys
from preprocessor import *
from editor.grammar_editor import *
import re


def editor_loop(cmd):
    G = Grammar()
    while cmd != "exit":
        cmd = input("\N{ESC}[32mpathfinder:\N{ESC}[33mgrammar editor\u001b[0m>>")
        parsed = re.split(r'[\s,]+', cmd)
        print(parsed)
        if parsed in ["help", "h"]:
            print("Grammar editor help: TO DO")
        elif parsed[0]=="ar" or (parsed[0] == "add" and parsed[1] == "rule"):
            arrow = parsed.index("->")
            left , right = (parsed[arrow-1], parsed[arrow+1:])
            G.P += [Rule(Term(left), [Term(x) for x in right], tag="manual")]
            G.print()
        elif parsed[0] == "pr" or (parsed[0] == "print"):
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
        elif cmd.startswith("editor"):
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