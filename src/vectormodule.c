#define _USE_MATH_DEFINES
#include <Python.h>
#include "structmember.h"

static PyTypeObject VectorType;

/* Module functions */

static PyObject *
vector_mag(PyObject *self, PyObject *args)
{
    const double ax, ay, az, bx, by, bz;
    double x, y, z;

    if (!PyArg_ParseTuple(args, "dddddd", &ax, &ay, &az, &bx, &by, &bz))
        return NULL;

    x = ax - bx;
    y = ay - by;
    z = az - bz;
    return PyFloat_FromDouble(sqrt(x * x + y * y + z * z));
}

/* Vector class */

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

static double
Vector_getdoublemag(Vector* self)
{
    return sqrt(self->x * self->x + self->y * self->y + self->z * self->z);
}

static PyObject *
Vector_getmag(Vector* self, void *closure)
{
    return PyFloat_FromDouble(Vector_getdoublemag(self));
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

static double
Vector_getdoubleargument(Vector* self)
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

    return ret;
}

static PyObject *
Vector_getargument(Vector* self, void *closure)
{
    return PyFloat_FromDouble(Vector_getdoubleargument(self));
}

static PyGetSetDef Vector_getseters[] = {
    {"mag",
     (getter)Vector_getmag, (setter)Vector_setmag,
     "vector magnitude",
     NULL},
    {"argument",
     (getter)Vector_getargument, NULL,
     "vector 2D argument, between 0 and 2 * PI",
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
Vector_distance(Vector *self, PyObject *args)
{
    Vector *a, *b, *c, *ab, *ac;
    double d, alpha, beta_ab, beta_ac, beta, mc_x, mc_z;
    if (!PyArg_ParseTuple(args, "OO", &b, &c))
        return NULL;

    a = self;
    b = (Vector *)b;
    c = (Vector *)c;
    ab = PyObject_CallObject((PyObject *) &VectorType, NULL);
    ab->x = b->x - a->x;
    ab->y = b->y - a->y;
    ab->z = b->z - a->z;
    ac = PyObject_CallObject((PyObject *) &VectorType, NULL);
    ac->x = c->x - a->x;
    ac->y = c->y - a->y;
    ac->z = c->z - a->z;
    d = 0;
    alpha = Vector_getdoubleargument(ab) - Vector_getdoubleargument(ac);
    if(ab->x != 0 || ab->y != 0)
    {
        beta_ab = atan(ab->z / sqrt(ab->x * ab->x + ab->y * ab->y));
    }
    else if(ab->z < 0)
    {
        beta_ab = -M_PI;
    }
    else
    {
        beta_ab = M_PI;
    }
    if(ac->x != 0 || ac->y != 0)
    {
        beta_ac = atan(ac->z / sqrt(ac->x * ac->x + ac->y * ac->y));
    }
    else if(ac->z < 0)
    {
        beta_ac = -M_PI;
    }
    else
    {
        beta_ac = M_PI;
    }
    beta = beta_ab - beta_ac;
    mc_x = Vector_getdoublemag(ac) * sin(alpha);
    mc_z = Vector_getdoublemag(ac) * sin(beta);
    d = sqrt(mc_x * mc_x + mc_z * mc_z);
    return PyFloat_FromDouble(d);
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

static PyObject *
Vector_sub(PyObject *self, PyObject *other)
{
    Vector *vector1, *vector2, *result;
    double x, y, z;

    vector1 = (Vector *)self;
    vector2 = (Vector *)other;
    x = vector1->x - vector2->x;
    y = vector1->y - vector2->y;
    z = vector1->z - vector2->z;
    result = PyObject_CallObject((PyObject *) &VectorType, NULL);
    if(result == NULL)
        return NULL;

    result->x = x;
    result->y = y;
    result->z = z;
    return (PyObject *)result;
}

static PyObject *
Vector_mul(PyObject *self, PyObject *value)
{
    Vector *vector, *result;
    double x, y, z, times;

    times = PyFloat_AsDouble(value);
    if(PyErr_Occurred())
    {
        PyErr_SetString(PyExc_TypeError, "can only multiply by a number");
        return NULL;
    }

    vector = (Vector *)self;
    x = vector->x * times;
    y = vector->y * times;
    z = vector->z * times;
    result = PyObject_CallObject((PyObject *) &VectorType, NULL);
    if(result == NULL)
        return NULL;

    result->x = x;
    result->y = y;
    result->z = z;
    return (PyObject *)result;
}

static PyObject *
Vector_trueDivide(PyObject *self, PyObject *value)
{
    Vector *vector, *result;
    double x, y, z, divided;

    divided = PyFloat_AsDouble(value);
    if(PyErr_Occurred())
    {
        PyErr_SetString(PyExc_TypeError, "can only divide by a number");
        return NULL;
    }

    vector = (Vector *)self;
    x = vector->x / divided;
    y = vector->y / divided;
    z = vector->z / divided;
    result = PyObject_CallObject((PyObject *) &VectorType, NULL);
    if(result == NULL)
        return NULL;

    result->x = x;
    result->y = y;
    result->z = z;
    return (PyObject *)result;
}

static PyObject *
Vector_neg(PyObject *self)
{
    Vector *vector, *result;
    double x, y, z;

    vector = (Vector *)self;
    x = -vector->x;
    y = -vector->y;
    z = -vector->z;
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
    Vector_sub,          /*nb_subtract*/
    Vector_mul,          /*nb_multiply*/
    0,          /*nb_remainder*/
    0,                  /*nb_divmod*/
    0,                  /*nb_power*/
    Vector_neg,                  /*nb_negative*/
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
    Vector_trueDivide,  /* nb_true_divide */
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
    {"distance",   Vector_distance,  METH_VARARGS,
     "a.distance(b, c) -- return the distance between c and AB"},
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

static PyMethodDef VectorMethods[] = {
    {"mag",  vector_mag, METH_VARARGS,
     "Return the magnitude between two points"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static PyModuleDef vectormodule = {
    PyModuleDef_HEAD_INIT,
    "vector",
    "Basic module for 3D vectors",
    -1,
    VectorMethods
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
