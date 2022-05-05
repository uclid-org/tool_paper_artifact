/*******************************************************************\

 Module: Variable Sensitivity

 Author: Thomas Kiley, thomas.kiley@diffblue.com

\*******************************************************************/

/// \file
/// An abstraction of an array value

#ifndef CPROVER_ANALYSES_VARIABLE_SENSITIVITY_CONSTANT_ARRAY_ABSTRACT_OBJECT_H
#define CPROVER_ANALYSES_VARIABLE_SENSITIVITY_CONSTANT_ARRAY_ABSTRACT_OBJECT_H

#include <iosfwd>
#include <vector>

#include <analyses/variable-sensitivity/array_abstract_object.h>
#include <analyses/variable-sensitivity/constant_abstract_value.h>

class ai_baset;
class abstract_environmentt;

class constant_array_abstract_objectt : public array_abstract_objectt
{
public:
  typedef sharing_ptrt<constant_array_abstract_objectt> const
    constant_array_pointert;

  /// \param type: the type the abstract_object is representing
  explicit constant_array_abstract_objectt(typet type);

  /// Start the abstract object at either top or bottom or neither
  /// Asserts if both top and bottom are true
  ///
  /// \param type: the type the abstract_object is representing
  /// \param top: is the abstract_object starting as top
  /// \param bottom: is the abstract_object starting as bottom
  constant_array_abstract_objectt(typet type, bool top, bool bottom);

  /// \param expr: the expression to use as the starting pointer for
  ///              an abstract object
  /// \param environment: the environment the abstract object is
  ///                     being created in
  /// \param ns: the namespace
  constant_array_abstract_objectt(
    const exprt &expr,
    const abstract_environmentt &environment,
    const namespacet &ns);

  virtual ~constant_array_abstract_objectt()
  {
  }

  /// the current known value about this object. For this array we
  /// print: { [0] - <output of object at index 0... }
  ///
  /// \param out: the stream to write to
  /// \param ai: the abstract interpreter that contains the abstract domain
  ///            (that contains the object ... )
  /// \param ns: the current namespace
  void output(std::ostream &out, const ai_baset &ai, const namespacet &ns)
    const override;

  /**
   * Apply a visitor operation to all sub elements of this abstract_object.
   * A sub element might be a member of a struct, or an element of an array,
   * for instance, but this is entirely determined by the particular
   * derived instance of abstract_objectt.
   *
   * \param visitor an instance of a visitor class that will be applied to
   * all sub elements
   * \return A new abstract_object if it's contents is modifed, or this if
   * no modification is needed
   */
  abstract_object_pointert
  visit_sub_elements(const abstract_object_visitort &visitor) const override;

  void get_statistics(
    abstract_object_statisticst &statistics,
    abstract_object_visitedt &visited,
    const abstract_environmentt &env,
    const namespacet &ns) const override;

protected:
  CLONE

  /// A helper function to read elements from an array. This will return
  /// the abstract object stored for that index, or top if we don't know
  /// about the specified index.
  /// If we can't resolve the index to a constant, we return top
  ///
  /// \param env: the environment
  /// \param index: the expression used to access the specific value
  ///               in the array
  /// \param ns: the namespace
  ///
  /// \return An abstract object representing the value in the array
  abstract_object_pointert read_index(
    const abstract_environmentt &env,
    const index_exprt &index,
    const namespacet &ns) const override;

  /// A helper function to evaluate writing to a index of an array.
  ///
  /// \param environment: the abstract environment
  /// \param ns: the namespace
  /// \param stack: the remaining stack of expressions on the LHS to evaluate
  /// \param index_expr: the expression uses to access a specific index
  /// \param value: the value we are trying to assign to that value in the array
  /// \param merging_write: Should this and all future writes be merged with the
  ///                       current value
  /// \return The abstract_object_pointert representing the result of writing
  ///         to a specific index.
  abstract_object_pointert write_index(
    abstract_environmentt &environment,
    const namespacet &ns,
    const std::stack<exprt> &stack,
    const index_exprt &index_expr,
    const abstract_object_pointert &value,
    bool merging_write) const override;

  /// Tries to do an array/array merge if merging with a constant array
  /// If it can't, falls back to parent merge
  ///
  /// \param other: The object to merge in
  ///
  /// \return Returns the result of the merge.
  abstract_object_pointert merge(abstract_object_pointert other) const override;

  /// To validate that the struct object is in a valid state.
  /// This means either it is top or bottom, or if neither of those
  /// then there exists something in the map of components.
  /// If there is something in the map, then it can't be top or bottom
  ///
  /// \return Returns true if the struct is valid
  bool verify() const override;

  /// \brief Perform any additional structural modifications when setting this
  /// object to TOP
  void set_top_internal() override;

  /// Evaluates the index and tries to convert it to a constant integer
  ///
  /// \param index: the index expression showing where to access the array
  /// \param env: the abstract environment
  /// \param ns: the namespace
  /// \param out_index: the index if it can be converted to a constant
  ///
  /// \return An abstract object pointer of type type().subtype() (i.e. the
  ///         type of the array's values).
  virtual bool eval_index(
    const index_exprt &index,
    const abstract_environmentt &env,
    const namespacet &ns,
    mp_integer &out_index) const;

private:
  // Since we don't store for any index where the value is top
  // we don't use a regular array but instead a map of array indices
  // to the value at that index
  struct mp_integer_hasht
  {
    size_t operator()(const mp_integer &i) const
    {
      return std::hash<BigInt::ullong_t>{}(i.to_ulong());
    }
  };

  typedef sharing_mapt<
    mp_integer,
    abstract_object_pointert,
    false,
    mp_integer_hasht>
    shared_array_mapt;

  shared_array_mapt map;

  /// Short hand method for creating a top element of the array
  ///
  /// \param env: the abstract environment
  /// \param ns: the namespace
  ///
  /// \return An abstract object pointer of type type().subtype() (i.e. the
  ///         type of the array's values).
  ///
  abstract_object_pointert
  get_top_entry(const abstract_environmentt &env, const namespacet &ns) const;

  /// Merges an array into this array
  ///
  /// \param other: The object to merge in
  ///
  /// \return Returns a new abstract object that is the result of the merge
  ///         unless the merge is the same as this abstract object, in which
  ///         case it returns this..
  abstract_object_pointert
  constant_array_merge(const constant_array_pointert other) const;
};

#endif // CPROVER_ANALYSES_VARIABLE_SENSITIVITY_CONSTANT_ARRAY_ABSTRACT_OBJECT_H
