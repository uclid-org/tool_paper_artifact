/*******************************************************************\

 Module: Analyses Variable Sensitivity

 Author: DiffBlue Limited.

\*******************************************************************/

/// \file
/// Represents an entry in the write_stackt

#ifndef CPROVER_ANALYSES_VARIABLE_SENSITIVITY_WRITE_STACK_ENTRY_H
#define CPROVER_ANALYSES_VARIABLE_SENSITIVITY_WRITE_STACK_ENTRY_H

#include <memory>
#include <stack>

#include <analyses/variable-sensitivity/abstract_enviroment.h>
#include <util/namespace.h>
#include <util/std_expr.h>

class write_stack_entryt
{
public:
  virtual ~write_stack_entryt() = default;
  virtual exprt get_access_expr() const = 0;
  virtual void adjust_access_type(exprt &expr) const = 0;
  virtual bool try_squash_in(
    std::shared_ptr<const write_stack_entryt> new_entry,
    const abstract_environmentt &enviroment,
    const namespacet &ns);
};

class simple_entryt : public write_stack_entryt
{
public:
  explicit simple_entryt(exprt expr);
  exprt get_access_expr() const override;
  void adjust_access_type(exprt &expr) const override;

private:
  exprt simple_entry;
};

class offset_entryt : public write_stack_entryt
{
public:
  explicit offset_entryt(abstract_object_pointert offset_value);
  exprt get_access_expr() const override;
  void adjust_access_type(exprt &expr) const override;
  bool try_squash_in(
    std::shared_ptr<const write_stack_entryt> new_entry,
    const abstract_environmentt &enviroment,
    const namespacet &ns) override;

private:
  abstract_object_pointert offset;
};

#endif // CPROVER_ANALYSES_VARIABLE_SENSITIVITY_WRITE_STACK_ENTRY_H
