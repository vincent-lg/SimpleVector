#define _USE_MATH_DEFINES
#include <Python.h>
#include "structmember.h"

static PyTypeObject VectorType;

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
Vector_getmag(Vector* self, void *closure)
{
    return PyFloat_FromDouble(sqrt(self->x * self->x + self->y * self->y + self->z * self->z));
}

static int
Vector_setmag(Vector *self, PyObject *value, void *closure)
{
    double mag;
    double oldmag;
    if (value == NULL) {
        PyErr_SetString(PyExc_TypeError, "Cannot delete the mag attribute");
        return -1;
    }

    oldmag = sqrt(self->x * self->x + self->y * self->y + self->z * self->z);
    if (oldmag == 0) {
        PyErr_SetString(PyExc_TypeError, "Cannot set a vector with a null magnitude");
        return -1;
    }

    mag = PyFloat_AsDouble(value);
    if(PyErr_Occurred())
    {
        PyErr_SetString(PyExc_TypeError, "The mag attribute must be a float");
        return -1;
    }

    self->x = self->x / oldmag * mag;
    self->y = self->y / oldmag * mag;
    self->z = self->z / oldmag * mag;
    return 0;
}

static PyObject *
Vector_getargument(Vector* self, void *closure)
{
    double x, y, ret;
    x = self->x;
    y = self->y;
    if(x > 0)
    {
        ret = atan(y / x);
    }
    else if(x < 0)
    {
        ret = M_PI + atan(y / x);
    }
    else if(y > 0)
    {
        ret = M_PI / 2;
    }
    else if(y < 0)
    {
        ret = -M_PI / 2;
    }
    else
    {
        ret = 0;
    }

    if(ret < 0)
    {
       ret = M_PI * 2 - ret;
    }

    return PyFloat_FromDouble(ret);
}

static PyGetSetDef Vector_getseters[] = {
    {"mag",
     (getter)Vector_getmag, (setter)Vector_setmag,
     "vector magnitude",
     NULL},
    {"argument",
     (getter)Vector_getargument, NULL,
     "vector 2D argument",
     NULL},
    {NULL}  /* Sentinel */
};

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

static PyObject *
vector_around_x(Vector *self, PyObject *args)
{
    const double angle;
    double y, z;
    if (!PyArg_ParseTuple(args, "d", &angle))
        return NULL;

    y = self->y;
    z = self->z;
    self->y = y * cos(angle) - z * sin(angle);
    self->z = y * sin(angle) + z * cos(angle);
    return Py_BuildValue("");
}

static PyObject *
vector_around_y(Vector *self, PyObject *args)
{
    const double angle;
    double x, z;
    if (!PyArg_ParseTuple(args, "d", &angle))
        return NULL;

    x = self->x;
    z = self->z;
    self->x = x * cos(angle) + z * sin(angle);
    self->z = x * sin(angle) + z * cos(angle);
    return Py_BuildValue("");
}

static PyObject *
vector_around_z(Vector *self, PyObject *args)
{
    const double angle;
    double x, y;
    if (!PyArg_ParseTuple(args, "d", &angle))
        return NULL;

    x = self->x;
    y = self->y;
    self->x = x * cos(angle) + y * sin(angle);
    self->y = -1 * x * sin(angle) + y * cos(angle);
    return Py_BuildValue("");
}

static PyObject *
Vector_richcmp(PyObject *obj1, PyObject *obj2, int op)
{
    PyObject *result;
    Vector *vector1, *vector2;
    double value, x1, x2, y1, y2, z1, z2;

    vector1 = (Vector *)obj1;
    vector2 = (Vector *)obj2;
    x1 = vector1->x;
    x2 = vector2->x;
    y1 = vector1->y;
    y2 = vector2->y;
    z1 = vector1->z;
    z2 = vector2->z;

    switch (op) {
        case Py_EQ: value = x1 == x2 && y1 == y2 && z1 == z2; break;
        case Py_NE: value = x1 != x2 || y1 != y2 || z1 != z2; break;
        default: return Py_NotImplemented; break;
    }
    result = value ? Py_True : Py_False;
    Py_INCREF(result);
    return result;
}

static PyObject *
Vector_add(PyObject *self, PyObject *other)
{
    Vector *vector1, *vector2, *result;
    double x, y, z;

    vector1 = (Vector *)self;
    vector2 = (Vector *)other;
    x = vector1->x + vector2->x;
    y = vector1->y + vector2->y;
    z = vector1->z + vector2->z;
    result = PyObject_CallObject((PyObject *) &VectorType, NULL);
    if(result == NULL)
        return NULL;

    result->x = x;
    result->y = y;
    result->z = z;
    return (PyObject *)result;
}

static PyNumberMethods vector_as_number = {
    Vector_add,          /*nb_add*/
    0,          /*nb_subtract*/
    0,          /*nb_multiply*/
    0,          /*nb_remainder*/
    0,                  /*nb_divmod*/
    0,                  /*nb_power*/
    0,                  /*nb_negative*/
    0,                  /*nb_positive*/
    0,                  /*nb_absolute*/
    0,                  /*nb_bool*/
    0,                  /*nb_invert*/
    0,                  /*nb_lshift*/
    0,                  /*nb_rshift*/
    0,                  /*nb_and*/
    0,                  /*nb_xor*/
    0,                  /*nb_or*/
    0,                  /*nb_int*/
    0,                  /*nb_reserved*/
    0,                  /*nb_float*/
    0,                  /* nb_inplace_add */
    0,                  /* nb_inplace_subtract */
    0,                  /* nb_inplace_multiply */
    0,                  /* nb_inplace_remainder */
    0,                  /* nb_inplace_power */
    0,                  /* nb_inplace_lshift */
    0,                  /* nb_inplace_rshift */
    0,                  /* nb_inplace_and */
    0,                  /* nb_inplace_xor */
    0,                  /* nb_inplace_or */
    0,                  /* nb_floor_divide */
    0,                  /* nb_true_divide */
    0,                  /* nb_inplace_floor_divide */
    0,                  /* nb_inplace_true_divide */
};

static PyMethodDef Vector_methods[] = {
    {"around_x",   vector_around_x,  METH_VARARGS,
     "Turn around the X axis.  Give the angle as radians."},
    {"around_y",   vector_around_y,  METH_VARARGS,
     "Turn around the Y axis.  Give the angle as radians."},
    {"around_z",   vector_around_z,  METH_VARARGS,
     "Turn around the Z axis.  Give the angle as radians."},
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
    &vector_as_number,                         /* tp_as_number */
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
    Vector_richcmp,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Vector_methods,             /* tp_methods */
    Vector_members,             /* tp_members */
    Vector_getseters,                         /* tp_getset */
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
