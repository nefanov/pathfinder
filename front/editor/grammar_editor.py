from tabulate import tabulate
import copy

TOKEN_LIST = ["any any","any assign_const"]

index = 0;
def inc_unique_suffix():
  global index
  index += 1
  return str(index)

class Ckpt:
  def __init__(self, stack_limit=5):
    self.stack=[]
    self.limit=stack_limit

  def push(self, item):
    self.stack.append(copy.deepcopy(item))
    if len(self.stack) >= self.limit:
      self.stack = self.stack[1:]

  def pop(self):
    if len(self.stack) > 0:
      return self.stack.pop()
    return None

  def set_limit(self, l):
    self.limit=l


class Term: # for terminal and non-terminal symbols
  def __init__(self, N="A", repeatable=False, optional=False, repeat_limit=-1):
    self.name = N
    self.repeatable = repeatable
    self.repeat_limit = repeat_limit # -1 means "inf", or any positive number
    self.optional = optional
    return


  def __repr__(self):
      return "Term: " + self.name
  
  def repeatable(self, cond=True, limit="inf"):
    self.repeatable = cond
    self.repeat_limit = limit
    return

  def optional(self, cond=True):
    self.optional = cond


class Rule:
  '''
  L: left side
  R: right side
  tag: default | manual (written by programmer) | auto (generated)
  '''
  def __init__(self, L, R, tag="default"):
    self.lhs = L
    self.rhs = R
    self.tag = tag
    
  def __repr__(self):
    return "(" + self.lhs.name + " --> " + " ".join([r.name for r in self.rhs]) + ")"
    
  def make_repeatable(self, optional=True):
    '''
    properties: idempotency
    usage: make once before grammar finalizing
    '''
    rl = [self]
    last_initial_rule_idx = 0
    for idx, term in enumerate(self.rhs):
      if term.repeatable:
        orig_name = term.name
        new_term_name = term.name + '_' + inc_unique_suffix()
        t = Term(N=new_term_name)
        
        for i, rule in enumerate(rl):
          try:
            if rule.rhs[idx].name == orig_name and last_initial_rule_idx >= i:
              rule.rhs[idx].name = new_term_name
              if term.repeat_limit == -1:
                if optional and term.optional:
                  rl.append(Rule(t,[t,Term(orig_name)]))
                  rl.append(Rule(t,[Term("any any")]))
                else:
                  no_opt_name = term.name + '_' + inc_unique_suffix()
                  no_opt_name_2 = term.name + '_' + inc_unique_suffix()
                  t2 = Term(N=no_opt_name)
                  t3 = Term(N=no_opt_name_2)
                  rl.append(Rule(t,[t2,Term(orig_name)]))
                  rl.append(Rule(t2,[t,t3]))
                  rl.append(Rule(t2,[Term("any any")]))
                  rl.append(Rule(t3,[Term("any any")]))
              else:
                if term.optional:
                  rl.append(Rule(Term(new_term_name),[Term("any any")]))
                last_term = rule.rhs[idx]
                for i in range(term.repeat_limit):
                  temp_name = term.name + '_' + inc_unique_suffix()
                  temp_t = Term(temp_name)
                  rl.append(Rule(last_term, [temp_t, Term(orig_name)]))
                  last_term = temp_t
                rl.append(Rule(last_term, [Term("any any")]))

          except Exception as e:
            pass
        term.repeatable = False
    return rl
    
    def make_repeatable_and_non_optional(self):
        return self.make_repeatable(self, optional=False)


class Grammar:
  def __init__(self, from_list=[]):
    self.P = list() + from_list
    pass

  def save(self, path, replace_spaces=False):
    with open(path, 'w+') as f:
      strings = []
      for r in self.P:
        strings.append(r.lhs.name + " " + " ".join([rs.name.replace(" ", "_") if replace_spaces else rs.name for
                                                    rs in r.rhs ])+"\n")
      f.writelines(strings)
  
  def new_sync_term_rule(self, left_part=Term("A"), lbr=Term("a"), rbr=Term("b"),
                         interrelation="==", optional=True, between=[
                                                                    Term("any any"),
                                                                    Term("any any"),
                                                                    Term("any any")
          ],
                                                            facings=None):
    rl = []
    if interrelation == "==":
      if between:
        new_term_l = Term(left_part.name + '_' + inc_unique_suffix())
        new_term_l2 = Term(left_part.name + '_' + inc_unique_suffix())
        new_term_r = Term(left_part.name + '_' + inc_unique_suffix())
        new_term_r2 = Term(left_part.name + '_' + inc_unique_suffix())
        new_term_r3 = Term(left_part.name + '_' + inc_unique_suffix())
        lt = Term("Term__" + lbr.name)
        rl.append(Rule(left_part, [new_term_l, new_term_r]))
        rl.append(Rule(new_term_l, [new_term_l2, left_part]))
        rl.append(Rule(new_term_l2, [lt, between[0]]))
        if facings:
          lt2 = Term("Term__" + lbr.name + "2")
          rl.append(Rule(lt, [facings[0], lt2]))
          rl.append(Rule(lt2, [lbr]))
        else:
          rl.append(Rule(lt, [lbr]))
        rl.append(Rule(left_part, [between[1], new_term_r3]))

        rl.append(Rule(new_term_r, [between[2], new_term_r2]))
        if facings:
          rt2 = Term("Term__" + rbr.name + "2")
          rl.append(Rule(new_term_r2, [rt2, facings[1]]))
          rl.append(Rule(rt2, [rbr]))
        else:
          rl.append(Rule(new_term_r2, [rbr]))

        rl.append(Rule(new_term_r3, [Term("any any")]))
      else:
        new_term_l = Term(left_part.name + '_' + inc_unique_suffix())
        new_term_l2 = Term(left_part.name + '_' + inc_unique_suffix())
        new_term_r = Term(left_part.name + '_' + inc_unique_suffix())
        new_term_r2 = Term(left_part.name + '_' + inc_unique_suffix())
        new_term_r3 = Term(left_part.name + '_' + inc_unique_suffix())
        rl.append(Rule(left_part, [new_term_l, new_term_r]))
        rl.append(Rule(new_term_l, [new_term_l2, left_part]))
        rl.append(Rule(new_term_l2, [lbr]))
        rl.append(Rule(new_term_r, [new_term_r2, new_term_r3]))
        rl.append(Rule(new_term_r2, [rbr]))
        rl.append(Rule(new_term_r3, [Term("any any")]))
    if optional:
      rl.append(Rule(left_part, [Term("any any")]))
      
    self.P += rl  
    return rl
   
  def print(self, table=True, tablefmt='orgtbl', tab_filter=[]):
    if table: # required: tabulate
      content = [[i, p.lhs.name, ",".join([z.name for z in p.rhs]), p.tag]  for i, p in enumerate(self.P) if p.tag not in tab_filter]
      header = ["No", "left side", "right side", "tag"]
      print(tabulate(content, headers=header, tablefmt=tablefmt))
    else:
      for idx, p in enumerate(self.P):
        print(idx, p)
    return


  def finalize(self, flat=True, sequence=[]):
    '''
    :param flat: is this rule set represents only one-level sentential form in non-terminals ? If no, there may be some cross-level relations
    :return: full set of grammar rules
    '''
    if len(sequence) == 1:
      return self.P
    rhs = []
    while(len(sequence)>1):
      next_sequence = []
      for i, x in enumerate(sequence):
        rhs.append(x) # rules, which left sides are positions in right side of the new rule
        if len(rhs) == 2:
          found = [item for item in self.P if (len(item.rhs) == 2 and
                                               item.rhs[0].name == rhs[0].lhs.name and
                                               item.rhs[1].name == rhs[1].lhs.name)]
          if len(found) != 0:
            temp_t = found[0].lhs
          else:
            temp_t = Term("N_" + inc_unique_suffix())
          r = Rule(temp_t, [r.lhs for r in rhs], tag="auto")
          self.P += [r]
          rhs.clear()
          next_sequence.append(r)
      if len(rhs) == 1:
        next_sequence.append(sequence[-1])
        rhs.clear()
      sequence = next_sequence
      next_sequence = []






class Editor:
  def newGrammar(self, rules=[], ):
    self.G(rules)

  def add_rule_from_config(self, config):
    rl = [Rule(Term(config['L']['name']), [Term(cont['name'],
                                                       repeatable=cont['repeatable'],
                                                       optional=cont['optional'] ) for cont in config['R']])]
    if (config['make_repeatable']):
      rl = rl[0].make_repeatable(True if config['make_optional'] else False)

  def saveGrammar(self):
    pass

  def printGrammar(self):
    pass
