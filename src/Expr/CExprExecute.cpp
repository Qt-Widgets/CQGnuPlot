#include <CExprI.h>

class CExprExecuteImpl {
 public:
  CExprExecuteImpl(CExpr *expr) : expr_(expr) { }

 ~CExprExecuteImpl() { }

  bool executeCTokenStack(const CExprTokenStack &stack, CExprValueArray &values);
  bool executeCTokenStack(const CExprTokenStack &stack, CExprValuePtr &value);

 private:
  bool executeToken                (const CExprTokenBaseP &ctoken);
  bool executeOperator             (const CExprTokenBaseP &ctoken);
  void executeQuestionOperator     ();
  void executeUnaryOperator        (CExprOpType type);
  void executeLogicalUnaryOperator (CExprOpType type);
  void executeBitwiseUnaryOperator (CExprOpType type);
  bool executeBinaryOperator       (CExprOpType type);
  void executeLogicalBinaryOperator(CExprOpType type);
  void executeBitwiseBinaryOperator(CExprOpType type);
  void executeColonOperator        ();
  void executeCommaOperator        ();
#ifdef GNUPLOT_EXPR
  void executeSubscriptOperator    ();
#endif
  void executeEqualsOperator       ();
  bool executeFunction             (const CExprFunctionPtr &function, CExprValuePtr &value);
  bool executeBlock                (const CExprTokenStack &stack, CExprValuePtr &value);

  CExprValuePtr  etokenToValue(const CExprTokenBaseP &etoken);
#if 0
  CExprValueType etokenToValueType(const CExprTokenBaseP &etoken);
#endif

  void stackValue (const CExprValuePtr &value);
  void stackBlock ();
  void stackEToken(const CExprTokenBaseP &etoken);

  CExprValuePtr   unstackValue ();
  CExprTokenBaseP unstackEToken();

 private:
  CExpr*          expr_;
  CExprTokenStack ctoken_stack_;
  uint            ctoken_pos_;
  uint            num_ctokens_;
  CExprTokenStack etoken_stack_;
};

//------------

CExprExecute::
CExprExecute(CExpr *expr) :
 expr_(expr)
{
  impl_ = new CExprExecuteImpl(expr);
}

CExprExecute::
~CExprExecute()
{
}

bool
CExprExecute::
executeCTokenStack(const CExprTokenStack &stack, CExprValueArray &values)
{
  return impl_->executeCTokenStack(stack, values);
}

bool
CExprExecute::
executeCTokenStack(const CExprTokenStack &stack, CExprValuePtr &value)
{
  return impl_->executeCTokenStack(stack, value);
}

//------------

bool
CExprExecuteImpl::
executeCTokenStack(const CExprTokenStack &stack, CExprValueArray &values)
{
  ctoken_stack_ = stack;

  etoken_stack_.clear();

  num_ctokens_ = ctoken_stack_.getNumTokens();
  ctoken_pos_  = 0;

  while (ctoken_pos_ < num_ctokens_) {
    CExprTokenBaseP ctoken = ctoken_stack_.getToken(ctoken_pos_++);

    if (! executeToken(ctoken))
      return false;

    if (expr_->getDebug())
      std::cerr << "EToken Stack:" << etoken_stack_ << std::endl;
  }

  std::deque<CExprValuePtr> values1;

  bool rc = true;

  while (! etoken_stack_.empty()) {
    CExprValuePtr value = unstackValue();

    if (value.isValid())
      values1.push_front(value);
    else
      rc = false;
  }

  std::copy(values1.begin(), values1.end(), std::back_inserter(values));

  return rc;
}

bool
CExprExecuteImpl::
executeCTokenStack(const CExprTokenStack &stack, CExprValuePtr &value)
{
  CExprValueArray values;

  if (! executeCTokenStack(stack, values))
    return false;

  if (values.empty())
    value = CExprValuePtr();
  else
    value = values.back();

  return true;
}

bool
CExprExecuteImpl::
executeToken(const CExprTokenBaseP &ctoken)
{
  switch (ctoken->type()) {
    case CExprTokenType::IDENTIFIER:
      stackEToken(ctoken);

      break;
    case CExprTokenType::OPERATOR:
      if (! executeOperator(ctoken))
        return false;

      break;
    case CExprTokenType::INTEGER: {
      CExprValuePtr value = expr_->createIntegerValue(ctoken->getInteger());

      stackValue(value);

      break;
    }
    case CExprTokenType::REAL: {
      CExprValuePtr value = expr_->createRealValue(ctoken->getReal());

      stackValue(value);

      break;
    }
    case CExprTokenType::STRING: {
      CExprValuePtr value = expr_->createStringValue(ctoken->getString());

      stackValue(value);

      break;
    }
#ifdef GNUPLOT_EXPR
    case CExprTokenType::COMPLEX: {
      CExprValuePtr value = expr_->createComplexValue(ctoken->getComplex());

      stackValue(value);

      break;
    }
#endif
    case CExprTokenType::FUNCTION: {
      stackEToken(ctoken);

      break;
    }
    case CExprTokenType::VALUE:
      stackEToken(ctoken);

      break;
    default:
      assert(false);
      break;
  }

  return true;
}

bool
CExprExecuteImpl::
executeOperator(const CExprTokenBaseP &ctoken)
{
  CExprOpType type = ctoken->getOperator();

  switch (type) {
    case CExprOpType::LOGICAL_NOT:
      executeLogicalUnaryOperator(type);
      break;
    case CExprOpType::BIT_NOT:
      executeBitwiseUnaryOperator(type);
      break;
    case CExprOpType::UNARY_PLUS:
    case CExprOpType::UNARY_MINUS:
#ifdef GNUPLOT_EXPR
    case CExprOpType::FACTORIAL:
#endif
      executeUnaryOperator(type);
      break;
    case CExprOpType::POWER:
    case CExprOpType::TIMES:
    case CExprOpType::DIVIDE:
    case CExprOpType::MODULUS:
    case CExprOpType::PLUS:
    case CExprOpType::MINUS:
    case CExprOpType::LESS:
    case CExprOpType::LESS_EQUAL:
    case CExprOpType::GREATER:
    case CExprOpType::GREATER_EQUAL:
    case CExprOpType::EQUAL:
#ifdef GNUPLOT_EXPR
    case CExprOpType::STR_EQUAL:
#endif
    case CExprOpType::NOT_EQUAL:
#ifdef GNUPLOT_EXPR
    case CExprOpType::STR_NOT_EQUAL:
#endif
#ifdef GNUPLOT_EXPR
    case CExprOpType::STR_CONCAT:
#endif
      if (! executeBinaryOperator(type))
        return false;

      break;
    case CExprOpType::LOGICAL_AND:
    case CExprOpType::LOGICAL_OR:
      executeLogicalBinaryOperator(type);
      break;
    case CExprOpType::BIT_LSHIFT:
    case CExprOpType::BIT_RSHIFT:
    case CExprOpType::BIT_AND:
    case CExprOpType::BIT_XOR:
    case CExprOpType::BIT_OR:
      executeBitwiseBinaryOperator(type);
      break;
    case CExprOpType::QUESTION:
      executeQuestionOperator();
      break;
    case CExprOpType::COLON:
      executeColonOperator();
      break;
    case CExprOpType::EQUALS:
      executeEqualsOperator();
      break;
    case CExprOpType::COMMA:
      executeCommaOperator();
      break;
    case CExprOpType::OPEN_RBRACKET:
      stackEToken(ctoken);
      break;
#ifdef GNUPLOT_EXPR
    case CExprOpType::OPEN_SBRACKET:
      stackEToken(ctoken);
      break;
    case CExprOpType::CLOSE_SBRACKET:
      executeSubscriptOperator();
      break;
#endif
    case CExprOpType::START_BLOCK:
      stackBlock();
      break;
    case CExprOpType::UNKNOWN:
      stackEToken(ctoken);
      break;
    default: {
      expr_->errorMsg("Invalid operator for 'executeOperator'");
      return false;
    }
  }

  return true;
}

void
CExprExecuteImpl::
executeQuestionOperator()
{
  // pop boolean
  CExprValuePtr value = unstackValue();

  if (! value.isValid())
    return;

  bool flag;

  if (! value->getBooleanValue(flag))
    flag = false;

  //---

  // pop second token (rhs=false)
  CExprTokenBaseP etoken2 = unstackEToken();

  //---

  // pop first token (lhs=tue)
  CExprTokenBaseP etoken1 = unstackEToken();

  //---

  CExprValuePtr value1;

  if (flag) {
    if (etoken1.isValid())
      value1 = etokenToValue(etoken1);
  }
  else {
    if (etoken2.isValid())
      value1 = etokenToValue(etoken2);
  }

  if (! value1.isValid())
    return;

  stackValue(value1);
}

/* <value> <unary_op> */
void
CExprExecuteImpl::
executeUnaryOperator(CExprOpType type)
{
  CExprValuePtr value = unstackValue();

  if (! value.isValid())
    return;

  CExprValuePtr value1 = value->execUnaryOp(expr_, type);

  if (! value1.isValid())
    return;

  stackValue(value1);
}

/* <value> <unary_op> */
void
CExprExecuteImpl::
executeLogicalUnaryOperator(CExprOpType type)
{
  CExprValuePtr value = unstackValue();

  if (! value.isValid())
    return;

  if (! value->isBooleanValue()) {
    value = value->dup();

    if (! value->convToBoolean())
      return;
  }

  CExprValuePtr value1 = value->execUnaryOp(expr_, type);

  if (! value1.isValid())
    return;

  stackValue(value1);
}

/* <value> <unary_op> */
void
CExprExecuteImpl::
executeBitwiseUnaryOperator(CExprOpType type)
{
  CExprValuePtr value = unstackValue();

  if (! value.isValid())
    return;

  if (! value->isIntegerValue()) {
    value = value->dup();

    if (! value->convToInteger())
      return;
  }

  CExprValuePtr value1 = value->execUnaryOp(expr_, type);

  if (! value1.isValid())
    return;

  stackValue(value1);
}

/* <value1> <value2> <binary_op> */
bool
CExprExecuteImpl::
executeBinaryOperator(CExprOpType type)
{
  // pop rhs
  CExprValuePtr value2 = unstackValue();

  // pop lhs
  CExprValuePtr value1 = unstackValue();

  //---

  if (! value1.isValid() || ! value2.isValid())
    return false;

#ifdef GNUPLOT_EXPR
  if (value1->isComplexValue() || value2->isComplexValue()) {
    if (! value1->isComplexValue()) value1 = value1->dup();
    if (! value2->isComplexValue()) value2 = value2->dup();

    if (! value1->convToComplex()) return false;
    if (! value2->convToComplex()) return false;
  }
#endif

  if (value1->isRealValue() || value2->isRealValue()) {
    if (! value1->isRealValue()) value1 = value1->dup();
    if (! value2->isRealValue()) value2 = value2->dup();

    if (! value1->convToReal()) return false;
    if (! value2->convToReal()) return false;
  }

#ifdef GNUPLOT_EXPR
  if (value2->isStringValue() && ! value1->isStringValue()) {
    if (! value1->isStringValue()) value1 = value1->dup();

    if (! value1->convToString()) return false;
  }
#endif

  CExprValuePtr value = value1->execBinaryOp(expr_, value2, type);

  stackValue(value);

  return true;
}

/* <value1> <value2> <binary_op> */
void
CExprExecuteImpl::
executeLogicalBinaryOperator(CExprOpType type)
{
  // pop rhs
  CExprValuePtr value2 = unstackValue();

  // pop lhs
  CExprValuePtr value1 = unstackValue();

  //---

  if (! value1.isValid() || ! value2.isValid())
    return;

  if (! value1->isBooleanValue()) {
    value1 = value1->dup();

    if (! value1->convToBoolean())
      return;
  }

  if (! value2->isBooleanValue()) {
    value2 = value2->dup();

    if (! value2->convToBoolean())
      return;
  }

  CExprValuePtr value = value1->execBinaryOp(expr_, value2, type);

  stackValue(value);
}

/* <value1> <value2> <binary_op> */
void
CExprExecuteImpl::
executeBitwiseBinaryOperator(CExprOpType type)
{
  // pop rhs
  CExprValuePtr value2 = unstackValue();

  // pop lhs
  CExprValuePtr value1 = unstackValue();

  //---

  if (! value1.isValid() || ! value2.isValid())
    return;

  if (! value1->isIntegerValue()) {
    value1 = value1->dup();

    if (! value1->convToInteger())
      return;
  }

  if (! value2->isIntegerValue()) {
    value2 = value2->dup();

    if (! value2->convToInteger())
      return;
  }

  CExprValuePtr value = value1->execBinaryOp(expr_, value2, type);

  stackValue(value);
}

void
CExprExecuteImpl::
executeColonOperator()
{
}

void
CExprExecuteImpl::
executeCommaOperator()
{
}

#ifdef GNUPLOT_EXPR
void
CExprExecuteImpl::
executeSubscriptOperator()
{
  // get subscript values (popped in reverse order)
  std::deque<CExprValuePtr> values;

  CExprTokenBaseP etoken1 = unstackEToken();

  while (etoken1.isValid()) {
    CExprValuePtr value1;

    if (etoken1->type() != CExprTokenType::OPERATOR) {
      value1 = etokenToValue(etoken1);
    }
    else {
      if (etoken1->getOperator() != CExprOpType::UNKNOWN)
        break;
    }

    values.push_front(value1);

    etoken1 = unstackEToken();
  }

  CExprValueArray values1;

  std::copy(values.begin(), values.end(), std::back_inserter(values1));

  //---

  // unscript value
  CExprValuePtr value2 = unstackValue();

  if (! value2.isValid())
    return;

  //---

  CExprValuePtr value = value2->subscript(expr_, values1);

  stackValue(value);
}
#endif

void
CExprExecuteImpl::
executeEqualsOperator()
{
  // rhs
  CExprValuePtr value1 = unstackValue();

  if (! value1.isValid())
    return;

  //---

  CExprTokenBaseP etoken2 = unstackEToken();

  if (! etoken2.isValid())
    return;

  //---

  CExprValuePtr value;

  if (etoken2->type() == CExprTokenType::IDENTIFIER) {
    CExprVariablePtr variable = expr_->createVariable(etoken2->getIdentifier(), value1);

    value = variable->getValue();
  }
  else {
    expr_->errorMsg("Non lvalue for asssignment");
    return;
  }

  stackValue(value);
}

bool
CExprExecuteImpl::
executeFunction(const CExprFunctionPtr &function, CExprValuePtr &value)
{
  std::deque<CExprValuePtr> values;

  CExprTokenBaseP etoken = unstackEToken();

  if (! etoken.isValid())
    return false;

  while (etoken->type() != CExprTokenType::OPERATOR) {
    CExprValuePtr value1 = etokenToValue(etoken);

    values.push_front(value1);

    etoken = unstackEToken();
    assert(etoken.isValid());
  }

  for (uint i = 0; i < values.size(); ++i) {
    CExprValuePtr value1 = values[i];

    CExprValueType argType = function->argType(i);

    if (! value1.isValid()) {
      if (! (uint(argType) & uint(CExprValueType::NUL))) {
        expr_->errorMsg("Invalid type for function argument");
        return false;
      }
    }
    else {
      if (! (uint(argType) & uint(CExprValueType::NUL))) {
        if (! (uint(value1->getType()) & uint(argType)))
          value1 = value1->dup();

        if (! value1->convToType(argType)) {
          expr_->errorMsg("Invalid type for function argument");
          return false;
        }
      }
    }
  }

  assert(etoken->type() == CExprTokenType::OPERATOR);

  CExprValueArray values1;

  std::copy(values.begin(), values.end(), std::back_inserter(values1));

  if (! function->checkValues(values1)) {
    std::stringstream ostr;
    ostr << "Invalid function values : ";
    function->print(ostr);
    expr_->errorMsg(ostr.str());
    return false;
  }

  value = function->exec(expr_, values1);

  return true;
}

bool
CExprExecuteImpl::
executeBlock(const CExprTokenStack &stack, CExprValuePtr &value)
{
  CExprExecuteImpl impl(expr_);

  return impl.executeCTokenStack(stack, value);
}

CExprValuePtr
CExprExecuteImpl::
etokenToValue(const CExprTokenBaseP &etoken)
{
  switch (etoken->type()) {
    case CExprTokenType::IDENTIFIER: {
      CExprVariablePtr variable = expr_->getVariable(etoken->getIdentifier());

      if (variable.isValid())
        return variable->getValue();

      break;
    }
    case CExprTokenType::FUNCTION: {
      CExprValuePtr value;

      if (executeFunction(etoken->getFunction(), value))
        return value;

      break;
    }
    case CExprTokenType::BLOCK: {
      CExprValuePtr value;

      if (executeBlock(etoken->getBlock(), value))
        return value;

      break;
    }
    case CExprTokenType::VALUE:
      return etoken->getValue();
    default:
      break;
  }

  return CExprValuePtr();
}

void
CExprExecuteImpl::
stackValue(const CExprValuePtr &value)
{
  if (! value.isValid()) return;

  CExprTokenBaseP base(CExprTokenMgrInst->createValueToken(value));

  stackEToken(base);
}

void
CExprExecuteImpl::
stackBlock()
{
  CExprTokenStack stack;

  int brackets = 1;

  while (ctoken_pos_ < num_ctokens_) {
    CExprTokenBaseP ctoken = ctoken_stack_.getToken(ctoken_pos_++);

    if (! ctoken.isValid())
      break;

    if (ctoken->type() == CExprTokenType::OPERATOR) {
      CExprOpType op = ctoken->getOperator();

      if      (op == CExprOpType::START_BLOCK)
        ++brackets;
      else if (op == CExprOpType::END_BLOCK) {
        --brackets;

        if (brackets <= 0)
          break;
      }
    }

    stack.addToken(ctoken);
  }

  CExprTokenBaseP base(new CExprTokenBlock(stack));

  stackEToken(base);
}

void
CExprExecuteImpl::
stackEToken(const CExprTokenBaseP &base)
{
  etoken_stack_.addToken(base);
}

CExprValuePtr
CExprExecuteImpl::
unstackValue()
{
  CExprTokenBaseP etoken = unstackEToken();

  if (! etoken.isValid())
    return CExprValuePtr();

  return etokenToValue(etoken);
}

CExprTokenBaseP
CExprExecuteImpl::
unstackEToken()
{
  CExprTokenBaseP etoken = etoken_stack_.pop_back();

  return etoken;
}
