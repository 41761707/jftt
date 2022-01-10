
# parsetab.py
# This file is automatically generated. Do not edit.
# pylint: disable=W,C,R
_tabversion = '3.10'

_lr_method = 'LALR'

_lr_signature = "left+-left*/rightPOWrightUMINUSCOMMENT NEGPOW NUMBER POWstatement : expressionexpression : expression '+' expression\n                  | expression '-' expression\n                  | expression '*' expression\n                  | expression '/' expressionexpression : expression POW expressionexpression : '-' expression %prec UMINUSexpression : '(' expression ')'expression : NUMBER"
    
_lr_action_items = {'-':([0,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,],[3,7,3,3,-9,3,3,3,3,3,-7,7,-2,-3,-4,-5,-6,-8,]),'(':([0,3,4,6,7,8,9,10,],[4,4,4,4,4,4,4,4,]),'NUMBER':([0,3,4,6,7,8,9,10,],[5,5,5,5,5,5,5,5,]),'$end':([1,2,5,11,13,14,15,16,17,18,],[0,-1,-9,-7,-2,-3,-4,-5,-6,-8,]),'+':([2,5,11,12,13,14,15,16,17,18,],[6,-9,-7,6,-2,-3,-4,-5,-6,-8,]),'*':([2,5,11,12,13,14,15,16,17,18,],[8,-9,-7,8,8,8,-4,-5,-6,-8,]),'/':([2,5,11,12,13,14,15,16,17,18,],[9,-9,-7,9,9,9,-4,-5,-6,-8,]),'POW':([2,5,11,12,13,14,15,16,17,18,],[10,-9,-7,10,10,10,10,10,10,-8,]),')':([5,11,12,13,14,15,16,17,18,],[-9,-7,18,-2,-3,-4,-5,-6,-8,]),}

_lr_action = {}
for _k, _v in _lr_action_items.items():
   for _x,_y in zip(_v[0],_v[1]):
      if not _x in _lr_action:  _lr_action[_x] = {}
      _lr_action[_x][_k] = _y
del _lr_action_items

_lr_goto_items = {'statement':([0,],[1,]),'expression':([0,3,4,6,7,8,9,10,],[2,11,12,13,14,15,16,17,]),}

_lr_goto = {}
for _k, _v in _lr_goto_items.items():
   for _x, _y in zip(_v[0], _v[1]):
       if not _x in _lr_goto: _lr_goto[_x] = {}
       _lr_goto[_x][_k] = _y
del _lr_goto_items
_lr_productions = [
  ("S' -> statement","S'",1,None,None,None),
  ('statement -> expression','statement',1,'p_statement_expr','test.py',93),
  ('expression -> expression + expression','expression',3,'p_expression_binop','test.py',99),
  ('expression -> expression - expression','expression',3,'p_expression_binop','test.py',100),
  ('expression -> expression * expression','expression',3,'p_expression_binop','test.py',101),
  ('expression -> expression / expression','expression',3,'p_expression_binop','test.py',102),
  ('expression -> expression POW expression','expression',3,'p_expression_pow','test.py',128),
  ('expression -> - expression','expression',2,'p_expression_uminus','test.py',136),
  ('expression -> ( expression )','expression',3,'p_expression_group','test.py',142),
  ('expression -> NUMBER','expression',1,'p_expression_number','test.py',148),
]