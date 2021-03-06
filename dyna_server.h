/* Contents autogenerated by dyna_tools.py */
#include "lily_api_dyna.h"

const char *lily_dynaload_table[] = {
    "\2HtmlString\0Tainted\0"
    ,"R\0env\0Hash[String, Tainted[String]]"
    ,"R\0get\0Hash[String, Tainted[String]]"
    ,"R\0http_method\0String"
    ,"R\0post\0Hash[String, Tainted[String]]"
    ,"F\0write\0(HtmlString)"
    ,"F\0write_literal\0(String)"
    ,"F\0write_unsafe\0(String)"
    ,"N\02HtmlString\0"
    ,"m\0<new>\0(String):HtmlString"
    ,"1\0text\0String"
    ,"N\03Tainted\0[A]"
    ,"m\0<new>\0[A](A):Tainted[A]"
    ,"m\0sanitize\0[A,B](Tainted[A],Function(A=>B)):B"
    ,"1\0value\0A"
    ,"Z"
};

void *lily_server_loader(lily_state *s, int id)
{
    switch (id) {
        case 1: load_var_env(s); return NULL;
        case 2: load_var_get(s); return NULL;
        case 3: load_var_http_method(s); return NULL;
        case 4: load_var_post(s); return NULL;
        case 5: return lily_server__write;
        case 6: return lily_server__write_literal;
        case 7: return lily_server__write_unsafe;
        case 9: return lily_server_HtmlString_new;
        case 12: return lily_server_Tainted_new;
        case 13: return lily_server_Tainted_sanitize;
        default: return NULL;
    }
}
