#include "environment.hpp"

#include <cassert>
#include <cmath>
#include <iostream>

#include "interpreter_semantic_error.hpp"

//  This module should define the C++ types
//  and code required to implement the slisp environment mapping.

Environment::Environment(){
  //adding special procedures as a check for variables
  envmap["define"];
  envmap["define"].type = ProcedureType;

  envmap["begin"];
  envmap["begin"].type = ProcedureType;

  envmap["if"];
  envmap["if"].type = ProcedureType;

  envmap["draw"];
  envmap["draw"].type = ProcedureType;

  envmap["pi"];
  envmap["pi"].type = ExpressionType;
  envmap["pi"].exp = Expression( atan2(0, -1) );

  envmap["not"];
  envmap["not"].type = ProcedureType;
  envmap["not"].proc = Procedure(&not_proc);

  envmap["and"];
  envmap["and"].type = ProcedureType;
  envmap["and"].proc = Procedure(&and_proc);

  envmap["or"];
  envmap["or"].type = ProcedureType;
  envmap["or"].proc = Procedure(&or_proc);

  envmap["<="];
  envmap["<="].type = ProcedureType;
  envmap["<="].proc = Procedure(&less_than_equal_proc);

  envmap["<"];
  envmap["<"].type = ProcedureType;
  envmap["<"].proc = Procedure(&less_than_proc);

  envmap[">="];
  envmap[">="].type = ProcedureType;
  envmap[">="].proc = Procedure(&more_than_equal_proc);

  envmap[">"];
  envmap[">"].type = ProcedureType;
  envmap[">"].proc = Procedure(&more_than_proc);

  envmap["="];
  envmap["="].type = ProcedureType;
  envmap["="].proc = Procedure(&equal_proc);

  envmap["+"];
  envmap["+"].type = ProcedureType;
  envmap["+"].proc = Procedure(&addition_proc);

  envmap["-"];
  envmap["-"].type = ProcedureType;
  envmap["-"].proc = Procedure(&dash_proc);

  envmap["*"];
  envmap["*"].type = ProcedureType;
  envmap["*"].proc = Procedure(&multiplication_proc);

  envmap["/"];
  envmap["/"].type = ProcedureType;
  envmap["/"].proc = Procedure(&slash_proc);

  envmap["log10"];
  envmap["log10"].type = ProcedureType;
  envmap["log10"].proc = Procedure(&log_ten_proc);

  envmap["pow"];
  envmap["pow"].type = ProcedureType;
  envmap["pow"].proc = Procedure(&pow_proc);

  envmap["sin"];
  envmap["sin"].type = ProcedureType;
  envmap["sin"].proc = Procedure(&sin_proc);

  envmap["cos"];
  envmap["cos"].type = ProcedureType;
  envmap["cos"].proc = Procedure(&cos_proc);

  envmap["arctan"];
  envmap["arctan"].type = ProcedureType;
  envmap["arctan"].proc = Procedure(&atan_proc);

  envmap["point"];
  envmap["point"].type = ProcedureType;
  envmap["point"].proc = Procedure(&point_proc);

  envmap["line"];
  envmap["line"].type = ProcedureType;
  envmap["line"].proc = Procedure(&line_proc);

  envmap["arc"];
  envmap["arc"].type = ProcedureType;
  envmap["arc"].proc = Procedure(&arc_proc);
}

bool Environment::isProcedure(std::string key){
    return (envmap[key].type == ProcedureType);
}

Expression Environment::getExpression(std::string key){
    return envmap[key].exp;
}

bool Environment::keyPresent(std::string key){
    return (envmap.find(key) != envmap.end());
}

bool Environment::addExpression(std::string key, Expression value){
    envmap[key].type = ExpressionType;
    envmap[key].exp = value;
}

Expression Environment::getResult(std::string key, const std::vector<Atom> & args){
    return envmap[key].proc(args);
}

void Environment::clear() {
  *this = Environment();
}

bool areNum(const std::vector<Atom> & args){
  bool isNum = true;
  for (auto arg: args) {
      isNum &= (arg.type == NumberType);
  }
  return isNum;
}

bool areBool(const std::vector<Atom> & args){
  bool isBool = true;
  for (auto arg: args) {
      isBool &= (arg.type == BooleanType);
  }
  return isBool;
}

//--------------------------------------------------------------------------
//  Below are all function to be used as Procedures in mapping

Expression not_proc(const std::vector<Atom> & args) {
  if (args.size() != 1 || !areBool(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for not function");

  return Expression(!args[0].value.bool_value);
}

Expression and_proc(const std::vector<Atom> & args) {
  if (args.size() < 1 || !areBool(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for and function");

  bool finalValue = true;
  for (auto arg: args) {
      finalValue &= arg.value.bool_value;
  }
  return Expression(finalValue);
}

Expression or_proc(const std::vector<Atom> & args) {
  if (args.size() < 1 || !areBool(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for or function");

  bool finalValue = false;
  for (auto arg: args) {
      finalValue |= arg.value.bool_value;
  }
  return Expression(finalValue);
}

Expression less_than_proc(const std::vector<Atom> & args) {
  if (args.size() != 2 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for < function");

  bool lessThan = args[0].value.num_value < args[1].value.num_value;
  return Expression(lessThan);
}

Expression less_than_equal_proc(const std::vector<Atom> & args) {
  if (args.size() != 2 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for <= function");

  bool lessThanEq = args[0].value.num_value <= args[1].value.num_value;
  return Expression(lessThanEq);
}

Expression more_than_proc(const std::vector<Atom> & args) {
  if (args.size() != 2 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for > function");

  bool moreThan = args[0].value.num_value > args[1].value.num_value;
  return Expression(moreThan);
}

Expression more_than_equal_proc(const std::vector<Atom> & args) {
  if (args.size() != 2 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for >= function");

  bool moreThanEq = args[0].value.num_value >= args[1].value.num_value;
  return Expression(moreThanEq);
}

Expression equal_proc(const std::vector<Atom> & args) {
  if (args.size() != 2 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for = function");

  bool equals = args[0].value.num_value == args[1].value.num_value;
  return Expression(equals);
}

Expression addition_proc(const std::vector<Atom> & args) {
  if (args.size() < 1 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for + function");

  Number sum = 0.0;
  for (auto arg: args) {
      sum += arg.value.num_value;
  }
  return Expression(sum);
}

Expression dash_proc(const std::vector<Atom> & args) {
  if (args.size() > 2 || args.size() < 1 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for - function");

  if (args.size() == 1)
    return Expression(args[0].value.num_value * -1);
  else if (args.size() == 2)
    return Expression(args[0].value.num_value - args[1].value.num_value);
}

Expression multiplication_proc(const std::vector<Atom> & args) {
  if (args.size() == 1 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for * function");

  double product = 1;
  for (auto it = args.begin(); it != args.end(); ++it) {
      product *= it->value.num_value;
  }
  return Expression(product);
}

Expression slash_proc(const std::vector<Atom> & args) {
  if (args.size() != 2 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for / function");

  return Expression(args[0].value.num_value / args[1].value.num_value);
}

Expression log_ten_proc(const std::vector<Atom> & args) {
  if (args.size() != 1 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for log10 function");

  return Expression(log10(args[0].value.num_value));
}

Expression pow_proc(const std::vector<Atom> & args) {
  if (args.size() != 2 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for pow function");

  Number power = pow(args[0].value.num_value, args[1].value.num_value);
  return Expression(power);
}

Expression sin_proc(const std::vector<Atom> & args) {
  if (args.size() != 1 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for sin function");

  return Expression( sin(args[0].value.num_value) );
}

Expression cos_proc(const std::vector<Atom> & args) {
  if (args.size() != 1 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for cos function");

  return Expression( cos(args[0].value.num_value) );
}

Expression atan_proc(const std::vector<Atom> & args) {
  if (args.size() != 2 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for atan function");

  return Expression( atan2(args[0].value.num_value, args[1].value.num_value) );
}

Expression point_proc(const std::vector<Atom> & args){
  if (args.size() != 2 || !areNum(args))
      throw InterpreterSemanticError("Error: invalid number of arguments for point function");

  std::tuple<double,double> pnt = std::make_tuple(args[0].value.num_value, args[1].value.num_value);
  return Expression( pnt );
}

Expression line_proc(const std::vector<Atom> & args){
  if (args.size() != 2)
      throw InterpreterSemanticError("Error: invalid number of arguments for line function");
    std::tuple<double,double> start = std::make_tuple(args[0].value.point_value.x, args[0].value.point_value.y);
    std::tuple<double,double> end = std::make_tuple(args[1].value.point_value.x, args[1].value.point_value.y);
  return Expression( start, end );
}

Expression arc_proc(const std::vector<Atom> & args){
  if (args.size() != 3)
      throw InterpreterSemanticError("Error: invalid number of arguments for arc function");
    std::tuple<double,double> center = std::make_tuple(args[0].value.point_value.x, args[0].value.point_value.y);
    std::tuple<double,double> start = std::make_tuple(args[1].value.point_value.x, args[1].value.point_value.y);
  return Expression( center, start, args[2].value.num_value );
}
