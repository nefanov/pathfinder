Описание паттерна состоит из 2 частей:

-- Описание раметки

-- Описание грамматики


### Описание разметки

В данный момент описание разметки происходит программируемым способом через API, предоставляемое python-обвязкой фронтенда (front/).
В качестве точки входа предлагается использовать applications/RD/main.py

Структура задания разметки выглядит примерно так:
```
  # setting the pattern markup:
  scenario = {
                    'type':'flowlists',
                    'data':{'yes_df_list': [],
                            'no_df_list' : 
                            [
                                            [ # relation
                                                lexer.glex.Relation(
                                                left={'type': "assign*"},right={'type': "assign*"}, # src and dst nodes of relation edge
                                                predicate=check_ref_dep_mem, # function-specializer
                                                extra={"disable_labeling":
                                                            "$label $src:left" # extra labeling fmt string
                                                },
                                                label="DF_dep_from", # default label
                                                params={"edge_style": {"color": "#f76d23"}})# parameters for edge visualizing
                                            ] 
                            ],
                            'yes_cf_list': [["any if_cond", "if_cond any"]], #
                            'no_cf_list' : [["return_val exit"]],
                            'rel_kinds'  : set()}
            }
   #  yours extra actions with IR
   #        
   graph, mapping = prepare_custom_markup(scenario) # graph reading, markup
   # next stages
```
Здесь выделяются ребра графа потока управления, соединяющие, соответственно, вершины типов "any", "if_cond" и "if_cond", "any" -- как хорошие;

Ребра между "return_val", "exit" выделяются как нежелательные.

Нежелательный паттерн по потоку данных формируется как отношение "запись в сущность с некоторым идентификатором, а затем его использовании в правой части некоторого присваивания ниже по графу потока управления. Для проверки этого идентификатора используется предикат check_ref_dep_mem, передаваемый в отношение. Отношение будет проверяться только на вершинах типа "assign*", то есть "assign_aryphmetic_op", "assign_const" и т.д.

Метка label может быть опционально дополнена информацией из специализатора, для этого в extra нужно добавить поле labeling (в примере выше оно деактивировано: "disable_labeling").


### Описание грамматики
