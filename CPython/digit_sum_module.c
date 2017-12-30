#include <Python.h>

static PyObject* DigitSum(PyObject* self, PyObject* args) {
    int x;
    if (!PyArg_ParseTuple(args, "i", &x)) {
        return NULL;
    }
    int ans = 0;
    do {
        ans += x % 10;
        x /= 10;
    } while (x != 0);
    return PyLong_FromLong(ans);
}

static PyMethodDef SpamMethods[] = {
        {"DigitSum", DigitSum, METH_VARARGS, ""},
        {NULL,       NULL,     0,            NULL}
};

static struct PyModuleDef digit_sum_module = {
        PyModuleDef_HEAD_INIT,
        "digit_sum_module",
        NULL,
        -1,
        SpamMethods
};

PyMODINIT_FUNC PyInit_digit_sum_module(void) {
    return PyModule_Create(&digit_sum_module);
}
