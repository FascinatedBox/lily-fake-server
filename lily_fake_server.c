#include "lily_api_embed.h"
#include "lily_api_msgbuf.h"
#include "lily_api_value.h"

#include "extras_server.h"

/**
package server

This provides a mock implementation of the Lily server protocol detailed at
https://github.com/FascinatedBox/lily-server-protocol

You may find this to be a useful starting point for mocking a server without
having one running (ex: for debugging), or writing your own server
implementation.
*/

/**
native HtmlString
    private var @text: String

This class provides a wrapper over a `String`. The constructor of this class
will replace any of `"&<>"` with the appropriate html entity. Thus, instances of
this class are guaranteed to be html-encoded. The caller is responsible for
not encoding the data themselves beforehand (or it will be double-encoded).
*/

/**
constructor HtmlString(value: String): HtmlString
*/
void lily_server_HtmlString_new(lily_state *s)
{
    lily_container_val *con;
    lily_instance_super(s, &con, ID_HtmlString(s), 1);

    const char *text = lily_arg_string_raw(s, 0);
    lily_msgbuf *msgbuf = lily_get_clean_msgbuf(s);

    if (lily_mb_html_escape(msgbuf, text) == text)
        lily_nth_set(con, 0, lily_arg_value(s, 0));
    else
        lily_nth_set(con, 0, lily_box_string(s, lily_new_string(text)));

    lily_return_value(s, lily_take_value(s));
}

/**
native Tainted[A]
    private var @value: A

The `Tainted` type represents a wrapper over some data that is considered
unsafe. Data, once inside a `Tainted` value can only be retrieved using the
`Tainted.sanitize` function.
*/

/**
constructor Tainted[A](value: A): Tainted[A]
*/
void lily_server_Tainted_new(lily_state *s)
{
    lily_container_val *con;
    lily_instance_super(s, &con, ID_Tainted(s), 1);
    lily_nth_set(con, 0, lily_arg_value(s, 0));
    lily_return_value(s, lily_take_value(s));
}

/**
method Tainted.sanitize[A, B](self: Tainted[A], fn: Function(A => B)): B

This calls `fn` with the value contained within `self`. `fn` is assumed to be a
function that can sanitize the data within `self`.
*/
void lily_server_Tainted_sanitize(lily_state *s)
{
    lily_container_val *instance_val = lily_arg_container(s, 0);

    lily_push_value(s, lily_nth_get(instance_val, 0));

    lily_call_simple(s, lily_arg_function(s, 1), 1);

    lily_return_value(s, lily_result_value(s));
}

/* This demonstrates how to add an item to the hash if you plan on extending
   this example. */
static void add_hash_item(lily_state *s, lily_hash_val *hash, const char *key,
        const char *value)
{
    lily_string_val *str_key = lily_new_string(key);

    lily_container_val *instance = lily_new_instance(ID_Tainted(s), 1);
    lily_string_val *str_value = lily_new_string(value);
    lily_nth_set(instance, 0, lily_box_string(s, str_value));

    lily_hash_insert_str(hash, str_key, lily_box_instance(s, instance));
}

/**
var env: Hash[String, Tainted[String]]

This should contain key+value pairs representing the environment of the mock
server. Defaults to being empty.
*/
static void *load_var_env(lily_state *s)
{
    lily_push_hash(s, lily_new_hash_strtable());
}

/**
var get: Hash[String, Tainted[String]]

This contains key+value pairs that were sent to the server as GET variables.
Defaults to being empty.
*/
static void *load_var_get(lily_state *s)
{
    lily_push_hash(s, lily_new_hash_strtable());
}

/**
var http_method: String

This is the method that was used to make the request to the server. Should be
in all-caps. Default value is `"?"`.
*/
static void load_var_http_method(lily_state *s)
{
    lily_push_string(s, lily_new_string("?"));
}

/**
var post: Hash[String, Tainted[String]]

This contains key+value pairs that were sent to the server as POST variables.
Defaults to being empty.
*/
static void load_var_post(lily_state *s)
{
    lily_push_hash(s, lily_new_hash_strtable());
}

/**
define write(text: HtmlString)

This writes the contents of the `String` hidden within `text`. No escape is
performed, because the `HtmlString` constructor is assumed to have done that
already.
*/
void lily_server__write(lily_state *s)
{
    const char *to_write = lily_value_string_raw(lily_arg_nth_get(s, 0, 0));
    lily_op_get_render_func(s)(to_write, lily_op_get_data(s));
}

/**
define write_literal(text: String)

Write `text` to the server **without** any entity escaping. This function
assumes that the value passed is a `String` literal. Internally, this does the
same work as `server.write_unsafe`. The use of this function is that it implies
a contract (only `String` literals are passed). In doing so calls to
`server.write_unsafe` (a necessary evil) stand out more.
*/
void lily_server__write_literal(lily_state *s)
{
    const char *to_write = lily_arg_string_raw(s, 0);
    lily_op_get_render_func(s)(to_write, lily_op_get_data(s));
}

/**
define write_unsafe(text: String)

This writes `text` to the server **without** any entity escaping. This
function exists for cases when `text` is already escaped, or when `text` could
never reasonably contain html entities.
*/
void lily_server__write_unsafe(lily_state *s)
{
    const char *to_write = lily_arg_string_raw(s, 0);
    lily_op_get_render_func(s)(to_write, lily_op_get_data(s));
}

#include "dyna_server.h"
