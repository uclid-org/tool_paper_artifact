/*******************************************************************\

Module: Struct abstract object

Author: Thomas Kiley, thomas.kiley@diffblue.com

\*******************************************************************/

#include <ostream>

#include <analyses/variable-sensitivity/abstract_enviroment.h>
#include <util/std_expr.h>
#include <util/std_types.h>

#include "full_struct_abstract_object.h"

// #define DEBUG

#ifdef DEBUG
#  include <iostream>
#endif

full_struct_abstract_objectt::full_struct_abstract_objectt(
  const full_struct_abstract_objectt &ao)
  : struct_abstract_objectt(ao), map(ao.map)
{
}

full_struct_abstract_objectt::full_struct_abstract_objectt(const typet &t)
  : struct_abstract_objectt(t)
{
  PRECONDITION(t.id() == ID_struct);
  DATA_INVARIANT(verify(), "Structural invariants maintained");
}

full_struct_abstract_objectt::full_struct_abstract_objectt(
  const typet &t,
  bool top,
  bool bottom)
  : struct_abstract_objectt(t, top, bottom)
{
  PRECONDITION(t.id() == ID_struct);
  DATA_INVARIANT(verify(), "Structural invariants maintained");
}

full_struct_abstract_objectt::full_struct_abstract_objectt(
  const exprt &e,
  const abstract_environmentt &environment,
  const namespacet &ns)
  : struct_abstract_objectt(e, environment, ns)
{
  PRECONDITION(ns.follow(e.type()).id() == ID_struct);

  const struct_typet struct_type_def = to_struct_type(ns.follow(e.type()));

  bool did_initialize_values = false;
  auto struct_type_it = struct_type_def.components().begin();
  for(auto param_it = e.operands().begin(); param_it != e.operands().end();
      ++param_it, ++struct_type_it)
  {
    map.insert_or_replace(
      struct_type_it->get_name(),
      environment.abstract_object_factory(param_it->type(), *param_it, ns));
    did_initialize_values = true;
  }

  if(did_initialize_values)
  {
    set_not_top();
  }

  DATA_INVARIANT(verify(), "Structural invariants maintained");
}

abstract_object_pointert full_struct_abstract_objectt::read_component(
  const abstract_environmentt &environment,
  const member_exprt &member_expr,
  const namespacet &ns) const
{
#ifdef DEBUG
  std::cout << "Reading component " << member_expr.get_component_name() << '\n';
#endif

  if(is_top())
  {
    return environment.abstract_object_factory(member_expr.type(), ns, true);
  }
  else
  {
    PRECONDITION(!is_bottom());

    const irep_idt c = member_expr.get_component_name();

    auto const value = map.find(c);

    if(value.has_value())
    {
      return value.value();
    }
    else
    {
      return environment.abstract_object_factory(member_expr.type(), ns, true);
    }
  }
}

abstract_object_pointert full_struct_abstract_objectt::write_component(
  abstract_environmentt &environment,
  const namespacet &ns,
  const std::stack<exprt> &stack,
  const member_exprt &member_expr,
  const abstract_object_pointert &value,
  bool merging_write) const
{
#ifdef DEBUG
  std::cout << "Writing component " << member_expr.get_component_name() << '\n';
#endif

  if(is_bottom())
  {
    return std::make_shared<full_struct_abstract_objectt>(
      member_expr.compound().type(), false, true);
  }

  const auto &result =
    std::dynamic_pointer_cast<full_struct_abstract_objectt>(mutable_clone());

  if(!stack.empty())
  {
    abstract_object_pointert starting_value;
    const irep_idt c = member_expr.get_component_name();
    auto const old_value = map.find(c);
    if(!old_value.has_value())
    {
      starting_value = environment.abstract_object_factory(
        member_expr.type(), ns, true, false);
      result->map.insert(
        c, environment.write(starting_value, value, stack, ns, merging_write));
    }
    else
    {
      result->map.replace(
        c,
        environment.write(old_value.value(), value, stack, ns, merging_write));
    }

    result->set_not_top();
    DATA_INVARIANT(result->verify(), "Structural invariants maintained");
    return result;
  }
  else
  {
#ifdef DEBUG
    std::cout << "Setting component" << std::endl;
#endif

    const irep_idt c = member_expr.get_component_name();
    auto const old_value = result->map.find(c);

    if(merging_write)
    {
      if(is_top()) // struct is top
      {
        DATA_INVARIANT(result->verify(), "Structural invariants maintained");
        return result;
      }

      INVARIANT(!result->map.empty(), "If not top, map cannot be empty");

      if(!old_value.has_value()) // component is top
      {
        DATA_INVARIANT(result->verify(), "Structural invariants maintained");
        return result;
      }

      bool dummy;

      result->map.replace(
        c, abstract_objectt::merge(old_value.value(), value, dummy));
    }
    else
    {
      if(old_value.has_value())
      {
        result->map.replace(c, value);
      }
      else
      {
        result->map.insert(c, value);
      }
      result->set_not_top();
      INVARIANT(!result->is_bottom(), "top != bottom");
    }

    DATA_INVARIANT(result->verify(), "Structural invariants maintained");

    return result;
  }
}

void full_struct_abstract_objectt::output(
  std::ostream &out,
  const ai_baset &ai,
  const namespacet &ns) const
{
  // To ensure that a consistent ordering of fields is output, use
  // the underlying type declaration for this struct to determine
  // the ordering
  struct_union_typet type_decl = to_struct_union_type(ns.follow(type()));

  bool first = true;

  out << "{";
  for(const auto &field : type_decl.components())
  {
    auto value = map.find(field.get_name());
    if(value.has_value())
    {
      if(!first)
      {
        out << ", ";
      }
      out << '.' << field.get_name() << '=';
      static_cast<const abstract_object_pointert &>(value.value())
        ->output(out, ai, ns);
      first = false;
    }
  }
  out << "}";
}

bool full_struct_abstract_objectt::verify() const
{
  // Either the object is top or bottom (=> map empty)
  // or the map is not empty => neither top nor bottom
  return (is_top() || is_bottom()) == map.empty();
}

abstract_object_pointert
full_struct_abstract_objectt::merge(abstract_object_pointert other) const
{
  constant_struct_pointert cast_other =
    std::dynamic_pointer_cast<const full_struct_abstract_objectt>(other);
  if(cast_other)
  {
    return merge_constant_structs(cast_other);
  }
  else
  {
    // TODO(tkiley): How do we set the result to be toppish? Does it matter?
    return struct_abstract_objectt::merge(other);
  }
}

abstract_object_pointert full_struct_abstract_objectt::merge_constant_structs(
  constant_struct_pointert other) const
{
  if(is_bottom())
  {
    return std::make_shared<full_struct_abstract_objectt>(*other);
  }
  else
  {
    const auto &result =
      std::dynamic_pointer_cast<full_struct_abstract_objectt>(mutable_clone());

    bool modified = abstract_objectt::merge_shared_maps<irep_idt>(
      map, other->map, result->map);

    if(!modified)
    {
      DATA_INVARIANT(verify(), "Structural invariants maintained");
      return shared_from_this();
    }
    else
    {
      INVARIANT(!result->is_top(), "Merge of maps will not generate top");
      INVARIANT(!result->is_bottom(), "Merge of maps will not generate bottom");
      DATA_INVARIANT(result->verify(), "Structural invariants maintained");
      return result;
    }
  }
}

abstract_object_pointert full_struct_abstract_objectt::visit_sub_elements(
  const abstract_object_visitort &visitor) const
{
  const auto &result =
    std::dynamic_pointer_cast<full_struct_abstract_objectt>(mutable_clone());

  bool modified = false;

  shared_struct_mapt::viewt view;
  result->map.get_view(view);

  for(auto &item : view)
  {
    auto newval = visitor.visit(item.second);
    if(newval != item.second)
    {
      result->map.replace(item.first, newval);
      modified = true;
    }
  }

  if(modified)
  {
    return result;
  }
  else
  {
    return shared_from_this();
  }
}

void full_struct_abstract_objectt::get_statistics(
  abstract_object_statisticst &statistics,
  abstract_object_visitedt &visited,
  const abstract_environmentt &env,
  const namespacet &ns) const
{
  struct_abstract_objectt::get_statistics(statistics, visited, env, ns);
  shared_struct_mapt::viewt view;
  map.get_view(view);
  for(auto const &object : view)
  {
    if(visited.find(object.second) == visited.end())
    {
      object.second->get_statistics(statistics, visited, env, ns);
    }
  }
  statistics.objects_memory_usage += memory_sizet::from_bytes(sizeof(*this));
}
