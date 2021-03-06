#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "nlr.h"
#include "misc.h"
#include "mpconfig.h"
#include "qstr.h"
#include "obj.h"
#include "runtime.h"
#include "stream.h"

typedef struct _mp_obj_fdfile_t {
    mp_obj_base_t base;
    int fd;
} mp_obj_fdfile_t;

static const mp_obj_type_t rawfile_type;

static void fdfile_print(void (*print)(void *env, const char *fmt, ...), void *env, mp_obj_t self_in, mp_print_kind_t kind) {
    mp_obj_fdfile_t *self = self_in;
    print(env, "<io.FileIO %d>", self->fd);
}

static machine_int_t fdfile_read(mp_obj_t o_in, void *buf, machine_uint_t size, int *errcode) {
    mp_obj_fdfile_t *o = o_in;
    machine_int_t r = read(o->fd, buf, size);
    if (r == -1) {
        *errcode = errno;
    }
    return r;
}

static machine_int_t fdfile_write(mp_obj_t o_in, const void *buf, machine_uint_t size, int *errcode) {
    mp_obj_fdfile_t *o = o_in;
    machine_int_t r = write(o->fd, buf, size);
    if (r == -1) {
        *errcode = errno;
    }
    return r;
}

static mp_obj_t fdfile_close(mp_obj_t self_in) {
    mp_obj_fdfile_t *self = self_in;
    close(self->fd);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(fdfile_close_obj, fdfile_close);

static mp_obj_t fdfile_fileno(mp_obj_t self_in) {
    mp_obj_fdfile_t *self = self_in;
    return MP_OBJ_NEW_SMALL_INT((machine_int_t)self->fd);
}
static MP_DEFINE_CONST_FUN_OBJ_1(fdfile_fileno_obj, fdfile_fileno);

static mp_obj_fdfile_t *fdfile_new(int fd) {
    mp_obj_fdfile_t *o = m_new_obj(mp_obj_fdfile_t);
    o->base.type = &rawfile_type;
    o->fd = fd;
    return o;
}

static mp_obj_t fdfile_make_new(mp_obj_t type_in, uint n_args, uint n_kw, const mp_obj_t *args) {
    mp_obj_fdfile_t *o = m_new_obj(mp_obj_fdfile_t);
    o->base.type = type_in;

    if (MP_OBJ_IS_SMALL_INT(args[0])) {
        o->fd = MP_OBJ_SMALL_INT_VALUE(args[0]);
        return o;
    }

    const char *fname = mp_obj_str_get_str(args[0]);
    const char *mode_s;
    if (n_args > 1) {
        mode_s = mp_obj_str_get_str(args[1]);
    } else {
        mode_s = "r";
    }

    int mode = 0;
    while (*mode_s) {
        switch (*mode_s++) {
            // Note: these assume O_RDWR = O_RDONLY | O_WRONLY
            case 'r':
                mode |= O_RDONLY;
                break;
            case 'w':
                mode |= O_WRONLY | O_CREAT | O_TRUNC;
                break;
            case 'a':
                mode |= O_APPEND;
                break;
            case '+':
                mode |= O_RDWR;
                break;
        }
    }

    int fd = open(fname, mode, 0644);
    if (fd == -1) {
        nlr_jump(mp_obj_new_exception_msg_varg(MP_QSTR_OSError, "[Errno %d]", errno));
    }
    return fdfile_new(fd);
}

static const mp_method_t rawfile_type_methods[] = {
        { "fileno", &fdfile_fileno_obj },
        { "read", &mp_stream_read_obj },
        { "readall", &mp_stream_readall_obj },
        { "readline", &mp_stream_unbuffered_readline_obj},
        { "write", &mp_stream_write_obj },
        { "close", &fdfile_close_obj },
        { NULL, NULL },
};

static const mp_obj_type_t rawfile_type = {
    { &mp_const_type },
    "io.FileIO",
    .print = fdfile_print,
    .make_new = fdfile_make_new,
    .getiter = mp_identity,
    .iternext = mp_stream_unbuffered_iter,
    .stream_p = {
        .read = fdfile_read,
        .write = fdfile_write,
    },
    .methods = rawfile_type_methods,
};

// Factory function for I/O stream classes
mp_obj_t mp_builtin_open(uint n_args, const mp_obj_t *args) {
    // TODO: analyze mode and buffering args and instantiate appropriate type
    return fdfile_make_new((mp_obj_t)&rawfile_type, n_args, 0, args);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mp_builtin_open_obj, 1, 2, mp_builtin_open);

void file_init() {
    rt_store_name(MP_QSTR_open, (mp_obj_t)&mp_builtin_open_obj);

    mp_obj_t m_sys = mp_obj_new_module(MP_QSTR_sys);
    rt_store_attr(m_sys, MP_QSTR_stdin, fdfile_new(STDIN_FILENO));
    rt_store_attr(m_sys, MP_QSTR_stdout, fdfile_new(STDOUT_FILENO));
    rt_store_attr(m_sys, MP_QSTR_stderr, fdfile_new(STDERR_FILENO));
}
