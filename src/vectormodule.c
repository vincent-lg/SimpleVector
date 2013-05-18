#include <Python.h>
#include "structmember.h"

typedef struct {
    PyObject_HEAD
    double x;
    double y;
    double z;
} Vector;

static PyObject *
Vector_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Vector *self;

    self = (Vector *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->x = 0.0;
        self->y = 0.0;
        self->z = 0.0;
    }

    return (PyObject *)self;
}

static int
Vector_init(Vector *self, PyObject *args, PyObject *kwds)
{
    PyObject *tmp;

    static char *kwlist[] = {"x", "y", "z", NULL};

    if (! PyArg_ParseTupleAndKeywords(args, kwds, "|ddd", kwlist,
                                      &self->x, &self->y, &self->z))
        return -1;

    return 0;
}


static PyMemberDef Vector_members[] = {
    {"x", T_DOUBLE, offsetof(Vector, x), 0,
     "X axis"},
    {"y", T_DOUBLE, offsetof(Vector, y), 0,
     "Y axis"},
    {"z", T_DOUBLE, offsetof(Vector, z), 0,
     "Z axis"},
    {NULL}  /* Sentinel */
};

static PyObject *
Vector_mag(Vector* self)
{
    return PyFloat_FromDouble(sqrt(self->x * self->x + self->y * self->y + self->z * self->z));
}

static PyObject *
Vector_repr(Vector* self)
{
    PyObject* x = PyFloat_FromDouble(self->x);
    PyObject* y = PyFloat_FromDouble(self->y);
    PyObject* z = PyFloat_FromDouble(self->z);
    return PyUnicode_FromFormat("<Vector x=%S y=%S z=%S>", x, y, z);
}

static PyObject *
Vector_str(Vector* self)
{
    PyObject* x = PyFloat_FromDouble(self->x);
    PyObject* y = PyFloat_FromDouble(self->y);
    PyObject* z = PyFloat_FromDouble(self->z);
    return PyUnicode_FromFormat("Vector(%S, %S, %S)", x, y, z);
}

static PyMethodDef Vector_methods[] = {
    {"mag", (PyCFunction)Vector_mag, METH_NOARGS,
     "Return the vector magnitude"
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject VectorType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "vector.Vector",             /* tp_name */
    sizeof(Vector),             /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    Vector_repr,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    Vector_str,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT |
        Py_TPFLAGS_BASETYPE,   /* tp_flags */
    "3D vector objects",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Vector_methods,             /* tp_methods */
    Vector_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Vector_init,      /* tp_init */
    0,                         /* tp_alloc */
    Vector_new,                 /* tp_new */
};

static PyModuleDef vectormodule = {
    PyModuleDef_HEAD_INIT,
    "vector",
    "Basic module for 3D vectors",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_vector(void)
{
    PyObject* m;

    if (PyType_Ready(&VectorType) < 0)
        return NULL;

    m = PyModule_Create(&vectormodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&VectorType);
    PyModule_AddObject(m, "Vector", (PyObject *)&VectorType);
    return m;
}
