// This file was generated by ODB, object-relational mapping (ORM)
// compiler for C++.
//

#ifndef PERSON1_ODB_HXX
#define PERSON1_ODB_HXX

#include <odb/version.hxx>

#if (ODB_VERSION != 20400UL)
#error ODB runtime version mismatch
#endif

#include <odb/pre.hxx>

#include "student.hpp"

#include <memory>
#include <cstddef>
#include <utility>

#include <odb/core.hxx>
#include <odb/traits.hxx>
#include <odb/callback.hxx>
#include <odb/wrapper-traits.hxx>
#include <odb/pointer-traits.hxx>
#include <odb/container-traits.hxx>
#include <odb/no-op-cache-traits.hxx>
#include <odb/result.hxx>
#include <odb/simple-object-result.hxx>

#include <odb/details/unused.hxx>
#include <odb/details/shared-ptr.hxx>

namespace odb
{
  // Person1
  //
  template <>
  struct class_traits< ::Person1 >
  {
    static const class_kind kind = class_object;
  };

  template <>
  class access::object_traits< ::Person1 >
  {
    public:
    typedef ::Person1 object_type;
    typedef ::Person1* pointer_type;
    typedef odb::pointer_traits<pointer_type> pointer_traits;

    static const bool polymorphic = false;

    typedef long unsigned int id_type;

    static const bool auto_id = true;

    static const bool abstract = false;

    static id_type
    id (const object_type&);

    typedef
    no_op_pointer_cache_traits<pointer_type>
    pointer_cache_traits;

    typedef
    no_op_reference_cache_traits<object_type>
    reference_cache_traits;

    static void
    callback (database&, object_type&, callback_event);

    static void
    callback (database&, const object_type&, callback_event);
  };
}

#include <odb/details/buffer.hxx>

#include <odb/mysql/version.hxx>
#include <odb/mysql/forward.hxx>
#include <odb/mysql/binding.hxx>
#include <odb/mysql/mysql-types.hxx>
#include <odb/mysql/query.hxx>

namespace odb
{
  // Person1
  //
  template <typename A>
  struct query_columns< ::Person1, id_mysql, A >
  {
    // id
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        long unsigned int,
        mysql::id_ulonglong >::query_type,
      mysql::id_ulonglong >
    id_type_;

    static const id_type_ id;

    // first
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        ::std::string,
        mysql::id_string >::query_type,
      mysql::id_string >
    first_type_;

    static const first_type_ first;

    // second
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        ::std::string,
        mysql::id_string >::query_type,
      mysql::id_string >
    second_type_;

    static const second_type_ second;

    // age
    //
    typedef
    mysql::query_column<
      mysql::value_traits<
        short unsigned int,
        mysql::id_ushort >::query_type,
      mysql::id_ushort >
    age_type_;

    static const age_type_ age;
  };

  template <typename A>
  const typename query_columns< ::Person1, id_mysql, A >::id_type_
  query_columns< ::Person1, id_mysql, A >::
  id (A::table_name, "`id`", 0);

  template <typename A>
  const typename query_columns< ::Person1, id_mysql, A >::first_type_
  query_columns< ::Person1, id_mysql, A >::
  first (A::table_name, "`first`", 0);

  template <typename A>
  const typename query_columns< ::Person1, id_mysql, A >::second_type_
  query_columns< ::Person1, id_mysql, A >::
  second (A::table_name, "`second`", 0);

  template <typename A>
  const typename query_columns< ::Person1, id_mysql, A >::age_type_
  query_columns< ::Person1, id_mysql, A >::
  age (A::table_name, "`age`", 0);

  template <typename A>
  struct pointer_query_columns< ::Person1, id_mysql, A >:
    query_columns< ::Person1, id_mysql, A >
  {
  };

  template <>
  class access::object_traits_impl< ::Person1, id_mysql >:
    public access::object_traits< ::Person1 >
  {
    public:
    struct id_image_type
    {
      unsigned long long id_value;
      my_bool id_null;

      std::size_t version;
    };

    struct image_type
    {
      // id_
      //
      unsigned long long id_value;
      my_bool id_null;

      // first_
      //
      details::buffer first_value;
      unsigned long first_size;
      my_bool first_null;

      // second_
      //
      details::buffer second_value;
      unsigned long second_size;
      my_bool second_null;

      // age_
      //
      unsigned short age_value;
      my_bool age_null;

      std::size_t version;
    };

    struct extra_statement_cache_type;

    using object_traits<object_type>::id;

    static id_type
    id (const id_image_type&);

    static id_type
    id (const image_type&);

    static bool
    grow (image_type&,
          my_bool*);

    static void
    bind (MYSQL_BIND*,
          image_type&,
          mysql::statement_kind);

    static void
    bind (MYSQL_BIND*, id_image_type&);

    static bool
    init (image_type&,
          const object_type&,
          mysql::statement_kind);

    static void
    init (object_type&,
          const image_type&,
          database*);

    static void
    init (id_image_type&, const id_type&);

    typedef mysql::object_statements<object_type> statements_type;

    typedef mysql::query_base query_base_type;

    static const std::size_t column_count = 4UL;
    static const std::size_t id_column_count = 1UL;
    static const std::size_t inverse_column_count = 0UL;
    static const std::size_t readonly_column_count = 0UL;
    static const std::size_t managed_optimistic_column_count = 0UL;

    static const std::size_t separate_load_column_count = 0UL;
    static const std::size_t separate_update_column_count = 0UL;

    static const bool versioned = false;

    static const char persist_statement[];
    static const char find_statement[];
    static const char update_statement[];
    static const char erase_statement[];
    static const char query_statement[];
    static const char erase_query_statement[];

    static const char table_name[];

    static void
    persist (database&, object_type&);

    static pointer_type
    find (database&, const id_type&);

    static bool
    find (database&, const id_type&, object_type&);

    static bool
    reload (database&, object_type&);

    static void
    update (database&, const object_type&);

    static void
    erase (database&, const id_type&);

    static void
    erase (database&, const object_type&);

    static result<object_type>
    query (database&, const query_base_type&);

    static unsigned long long
    erase_query (database&, const query_base_type&);

    public:
    static bool
    find_ (statements_type&,
           const id_type*);

    static void
    load_ (statements_type&,
           object_type&,
           bool reload);
  };

  template <>
  class access::object_traits_impl< ::Person1, id_common >:
    public access::object_traits_impl< ::Person1, id_mysql >
  {
  };

  // Person1
  //
}

#include "person1-odb.ixx"

#include <odb/post.hxx>

#endif // PERSON1_ODB_HXX
