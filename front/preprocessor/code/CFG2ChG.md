
# CFG2ChG

The program converts context-free grammar to Chomsky normal form.  
The letters are numerated from 1 to let_quantity, where let_quantity - quantity of letters, epsilon = 0.  
Nonterminal symbols are numerated from let_quantity + 2, where let_quantity + 2 is start symbol.  

## Input

let_quantity - quantity of letters  
N - quantity of rules in the grammar  
Then N lines  
S len_right a<sub>1</sub> ... a<sub>len_right</sub>  
S - nonterminal symbols on the left-hand side, len_right - length of the right-hand side, a<sub>i</sub> - symbol of the right-hand side  

## Output

N<sub>1</sub> - quantity of rules in the new grammar\
Then N<sub>1</sub>lines:  
S len_right a<sub>1</sub> ... a<sub>len_right</sub>  
S - nonterminal symbols on the left-hand side, len_right - length of the right-hand side, a_i - symbol of the right-hand side

# Example

Initial grammar: S -> aS; S -> 0

## Input
    1
    2
    3 2
    1 3
    3 1
    0

## Output

    5
    3 1
    1
    3 2
    1 4
    4 1
    1
    4 2
    1 4
    3 1
    0