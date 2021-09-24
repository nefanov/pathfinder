def predicate_any(self, l,r):
    return True


def predicate_is_depointer(self, l,r):
    try:
        return l['l'].startswith(r'\*') or l['l'].startswith("*")

    except Exception as e:
        print("depointer:", e, "Exception in predicate check: probably wrong lexem, node its format")
        return False


def predicate_malloc_memset(self, l, r):
    larg = [x.strip() for x in l['arguments'].split(',')]
    rarg = [x.strip() for x in r['arguments'].split(',')]
    try:
        return (l['func_name'] == 'malloc') and\
           (r['func_name'] == 'memset') and\
           (rarg[0] == l['left']) and \
           (rarg[2] == larg[0])

    except Exception as e:
        print("malloc memset:", e, "Exception in predicate check: probably wrong lexem, node its format",l,r)
        return False


def predicate_malloc_getptr(self, l,r):
    return (l['func_name'] == 'malloc') and\
           (r['r_operand1'].startswith(l['left']))


def predicate_getptr_write(self, l, r):
    return r['left'].startswith('*') and r['left'][1:] == l['left']

