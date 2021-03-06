int rt_is_true(mp_obj_t arg);

mp_obj_t rt_load_const_dec(qstr qstr);
mp_obj_t rt_load_const_str(qstr qstr);
mp_obj_t rt_load_const_bytes(qstr qstr);
mp_obj_t rt_load_name(qstr qstr);
mp_obj_t rt_load_global(qstr qstr);
mp_obj_t rt_load_build_class(void);
mp_obj_t rt_get_cell(mp_obj_t cell);
void rt_set_cell(mp_obj_t cell, mp_obj_t val);
void rt_store_name(qstr qstr, mp_obj_t obj);
void rt_store_global(qstr qstr, mp_obj_t obj);
mp_obj_t rt_unary_op(int op, mp_obj_t arg);
mp_obj_t rt_binary_op(int op, mp_obj_t lhs, mp_obj_t rhs);
mp_obj_t rt_make_function_from_id(int unique_code_id, mp_obj_t def_args);
mp_obj_t rt_make_function_n(int n_args, void *fun); // fun must have the correct signature for n_args fixed arguments
mp_obj_t rt_make_function_var(int n_args_min, mp_fun_var_t fun);
mp_obj_t rt_make_function_var_between(int n_args_min, int n_args_max, mp_fun_var_t fun); // min and max are inclusive
mp_obj_t rt_make_closure_from_id(int unique_code_id, mp_obj_t closure_tuple);
mp_obj_t rt_call_function_0(mp_obj_t fun);
mp_obj_t rt_call_function_1(mp_obj_t fun, mp_obj_t arg);
mp_obj_t rt_call_function_2(mp_obj_t fun, mp_obj_t arg1, mp_obj_t arg2);
mp_obj_t rt_call_function_n_kw_for_native(mp_obj_t fun_in, uint n_args_kw, const mp_obj_t *args);
mp_obj_t rt_call_function_n_kw(mp_obj_t fun, uint n_args, uint n_kw, const mp_obj_t *args);
mp_obj_t rt_call_method_n_kw(uint n_args, uint n_kw, const mp_obj_t *args);
mp_obj_t rt_build_tuple(int n_args, mp_obj_t *items);
mp_obj_t rt_build_list(int n_args, mp_obj_t *items);
mp_obj_t rt_list_append(mp_obj_t list, mp_obj_t arg);
mp_obj_t rt_build_set(int n_args, mp_obj_t *items);
mp_obj_t rt_store_set(mp_obj_t set, mp_obj_t item);
void rt_unpack_sequence(mp_obj_t seq, uint num, mp_obj_t *items);
mp_obj_t rt_build_map(int n_args);
mp_obj_t rt_store_map(mp_obj_t map, mp_obj_t key, mp_obj_t value);
mp_obj_t rt_load_attr(mp_obj_t base, qstr attr);
void rt_load_method(mp_obj_t base, qstr attr, mp_obj_t *dest);
void rt_store_attr(mp_obj_t base, qstr attr, mp_obj_t val);
void rt_store_subscr(mp_obj_t base, mp_obj_t index, mp_obj_t val);
mp_obj_t rt_getiter(mp_obj_t o);
mp_obj_t rt_iternext(mp_obj_t o);
mp_obj_t rt_import_name(qstr name, mp_obj_t fromlist, mp_obj_t level);
mp_obj_t rt_import_from(mp_obj_t module, qstr name);

struct _mp_map_t;
struct _mp_map_t *rt_locals_get(void);
void rt_locals_set(struct _mp_map_t *m);
struct _mp_map_t *rt_globals_get(void);
void rt_globals_set(struct _mp_map_t *m);
struct _mp_map_t *rt_loaded_modules_get(void);
extern mp_obj_t sys_path;
