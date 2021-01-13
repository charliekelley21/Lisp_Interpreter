#include "expression.hpp"


// system includes
#include <sstream>
#include <cmath>
#include <limits>
#include <cctype>
#include <tuple>

Expression::Expression(bool tf){
  head.type = BooleanType;
  head.value.bool_value = tf;
}

Expression::Expression(double num){
  head.type = NumberType;
  head.value.num_value = num;
}

Expression::Expression(const std::string & sym){
  head.type = SymbolType;
  head.value.sym_value = sym;
}

Expression::Expression(std::tuple<double,double> value){
  // TODO: implement this function
	head.type = PointType;
	head.value.point_value.x = std::get<0>(value);
	head.value.point_value.y = std::get<1>(value);
}

Expression::Expression(std::tuple<double,double> start,
		       std::tuple<double,double> end){
	head.type = LineType;
	head.value.line_value.first.x = std::get<0>(start);
	head.value.line_value.first.y = std::get<1>(start);

	head.value.line_value.second.x = std::get<0>(end);
	head.value.line_value.second.y = std::get<1>(end);
}


Expression::Expression(std::tuple<double,double> center,
		       std::tuple<double,double> start,
		       double angle){
	head.type = ArcType;
	head.value.arc_value.center.x = std::get<0>(center);
	head.value.arc_value.center.y = std::get<1>(center);

	head.value.arc_value.start.x = std::get<0>(start);
	head.value.arc_value.start.y = std::get<1>(start);

	head.value.arc_value.span = angle;
}

bool Expression::operator==(const Expression & exp) const noexcept{
	bool equals = (this->head.type == exp.head.type);
  equals &= (this->tail.size() == exp.tail.size());

  if (this->head.type == NumberType)
      equals &= (fabs(this->head.value.num_value - exp.head.value.num_value) < std::numeric_limits<double>::epsilon());
  else if (this->head.type == BooleanType)
      equals &= (this->head.value.bool_value == exp.head.value.bool_value);
  else if (this->head.type == SymbolType)
      equals &= (this->head.value.sym_value.compare(exp.head.value.sym_value) == 0);
	else if (this->head.type == PointType)
			equals &= (this->head.value.point_value == this->head.value.point_value);
	else if (this->head.type == LineType) {
			equals &= (this->head.value.line_value.first == this->head.value.line_value.first);
			equals &= (this->head.value.line_value.second == this->head.value.line_value.second);
	}
	else if (this->head.type == ArcType) {
		equals &= (this->head.value.arc_value.center == this->head.value.arc_value.center);
		equals &= (this->head.value.arc_value.start == this->head.value.arc_value.start);
		equals &= (fabs(this->head.value.arc_value.span - exp.head.value.arc_value.span) < std::numeric_limits<double>::epsilon());
	}
  return equals;
}

std::ostream & operator<<(std::ostream & out, const Expression & exp){
  out << "(";
  if (exp.head.type == NumberType)
      out << exp.head.value.num_value;
  else if (exp.head.type == BooleanType) {
      if (exp.head.value.bool_value)
          out << "True";
      else
          out << "False";
  }
  else if (exp.head.type == SymbolType)
      out << exp.head.value.sym_value;
	else if (exp.head.type == PointType)
	    out << exp.head.value.point_value.x << ", " << exp.head.value.point_value.y;
  else if (exp.head.type == LineType){
	    out << "(" << exp.head.value.line_value.first.x << ", " << exp.head.value.line_value.first.y << ") ";
			out << "(" << exp.head.value.line_value.second.x << ", " << exp.head.value.line_value.second.y << ")";
		}
	else if (exp.head.type == ArcType){
			out << "(" << exp.head.value.arc_value.center.x << ", " << exp.head.value.arc_value.center.y << ") ";
			out << "(" << exp.head.value.arc_value.start.x << ", " << exp.head.value.arc_value.start.y << ")";
			out << " " << exp.head.value.arc_value.span;
	}
	else if (exp.head.type == NoneType)
	    out << "NONE";
  out << ")";
  return out;
}

bool token_to_atom(const std::string & token, Atom & atom){
    try {
        size_t end = 0;
        atom.type = NumberType;
        atom.value.num_value = stod(token, &end);
        return (end == token.length());
      }
    catch (std::invalid_argument) {
        if (isdigit(token.front())) {
          return false;
        }
        else if (token == "True") {
          atom.type = BooleanType;
          atom.value.bool_value = true;
          return true;
        }
        else if (token == "False") {
          atom.type = BooleanType;
          atom.value.bool_value = false;
          return true;
        }
        else {
          atom.type = SymbolType;
          atom.value.sym_value = token;
          return true;
        }
    }
    return false;
}
