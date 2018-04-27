/*
 * 
 * This file is part of pyA20Lime.
 * gpio.c is python GPIO extension.
 * 
 * Copyright (c) 2014 Stefan Mavrodiev @ OLIMEX LTD, <support@olimex.com> 
 * 
 * pyA20Lime is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "Python.h"

#if PY_MAJOR_VERSION >= 3
    #define PyInt_FromLong PyLong_FromLong
    #define PyInt_AsLong PyLong_AsLong
#endif

#include "gpio_lib.h"

/**
 * 
 * Make initial initialization of the extention. This is done by map physical
 * memory to the virtual and thus gaining access to the memory. From there we
 * can do anything. 
 * 
 * @return none
 */
static PyObject* py_init(PyObject* self, PyObject* args) {

    if(sunxi_gpio_init() < 0)
        return PyErr_SetFromErrno(PyExc_IOError);

    Py_RETURN_NONE;
}

/**
 * Set configuration. See A20 datasheet for correct values.
 * 
 * @param self
 * @param args GPIO number and configuration value
 * @return none
 */
static PyObject* py_setcfg(PyObject* self, PyObject* args) {

    uint32_t pin, cfg;

    /* Parse arguments. Require two integers for GPIO number and configuration */
    if (!PyArg_ParseTuple(args, "ii", &pin, &cfg))
        return NULL;
    
    /* Set configuration and check return status */
    if (sunxi_gpio_set_cfgpin(pin, cfg) < 0)
        return PyErr_SetFromErrno(PyExc_IOError);

    Py_RETURN_NONE;
}

/**
 * Read current configuration of GPIO.
 * 
 * @param self
 * @param args GPIO number
 * @return current configuration
 */
static PyObject* py_getcfg(PyObject* self, PyObject* args) {

    uint32_t pin;
	int ret;

    /* Parse arguments */
    if (!PyArg_ParseTuple(args, "i", &pin))
        return NULL;

    /* Read configuration*/
    ret = sunxi_gpio_get_cfgpin(pin);
    if (ret < 0)
        return PyErr_SetFromErrno(PyExc_IOError);

    /* Return configuration */
    return Py_BuildValue("i", ret);
}

/**
 * Set output value of GPIO. Pin must be configured as output or else raises
 * exception. For high output level - 1, low - 0.
 * 
 * @param self
 * @param args Tuple that holds GPIO number and value 
 * @return none
 */
static PyObject* py_output(PyObject* self, PyObject* args) {

    uint32_t pin, value;

    /* Parse arguments. Two integers are required. */
    if (!PyArg_ParseTuple(args, "ii", &pin, &value))
        return NULL;

    /* Set output value and check return status. */
    if (sunxi_gpio_output(pin, value) < 0)
        return PyErr_SetFromErrno(PyExc_IOError);

    Py_RETURN_NONE;
}

/**
 * Read value of the pin configured as input. If its output raises exception.
 * 
 * @param self
 * @param args GPIO number
 * @return value of the given pin
 */
static PyObject* py_input(PyObject* self, PyObject* args) {

    uint32_t pin;
	int ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &pin))
        return NULL;
    
    /* Read value */
    ret = sunxi_gpio_input(pin);
    if (ret < 0)
        return PyErr_SetFromErrno(PyExc_IOError);

    /* Return read value */
    return Py_BuildValue("i", ret);
}

/**
 *  Set pull-up/pull-down on pin defined as input.
 * 
 * @param self
 * @param args
 * @return 
 */
static PyObject* py_pullup(PyObject* self, PyObject* args) {
    
    uint32_t pin, pull;
    
    /* Parse arguments */
    if(!PyArg_ParseTuple(args, "ii", &pin, &pull))
        return NULL;

    /* Set pull-up */
    if(sunxi_gpio_pullup(pin, pull) < 0)
        return PyErr_SetFromErrno(PyExc_IOError);

    Py_RETURN_NONE;
}

static PyObject* py_close(PyObject* self, PyObject* args) {

    sunxi_gpio_close();

    Py_RETURN_NONE;
}

static PyObject* py_PA(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPA(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PB(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPB(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PC(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPC(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PD(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPD(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PE(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPE(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PF(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPF(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PG(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPG(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PH(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPH(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PI(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPI(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PJ(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPJ(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PK(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPK(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PL(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPL(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PM(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPM(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PN(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPN(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

static PyObject* py_PO(PyObject* self, PyObject* args) {

    int gpio, ret;

    /* Parse argument. One integer is required */
    if (!PyArg_ParseTuple(args, "i", &gpio))
        return NULL;
    
    /* Check value */
	if (gpio < 0 || gpio > 32)
		return PyErr_Format(PyExc_Exception, "bad gpio number.");

    ret = SUNXI_GPO(gpio);

    /* Return value */
    return Py_BuildValue("i", ret);
}

/* Define module methods */
static PyMethodDef module_methods[] = {
    {"init",    py_init,        METH_NOARGS,    "Initialize module"},
    {"setcfg",  py_setcfg,      METH_VARARGS,   "Set pin function"},
    {"getcfg",  py_getcfg,      METH_VARARGS,   "Get pin function"},
    {"input",   py_input,       METH_VARARGS,   "Get input state"},
    {"output",  py_output,      METH_VARARGS,   "Set output state"},
    {"pullup",  py_pullup,      METH_VARARGS,   "Set pull-up/pull-down"},
	{"close",   py_close,       METH_NOARGS,    "Deinitialize module"},
    {"PA", py_PA, METH_VARARGS, "Returns gpio number"},
	{"PB", py_PB, METH_VARARGS, "Returns gpio number"},
	{"PC", py_PC, METH_VARARGS, "Returns gpio number"},
	{"PD", py_PD, METH_VARARGS, "Returns gpio number"},
	{"PE", py_PE, METH_VARARGS, "Returns gpio number"},
	{"PF", py_PF, METH_VARARGS, "Returns gpio number"},
	{"PG", py_PG, METH_VARARGS, "Returns gpio number"},
	{"PH", py_PH, METH_VARARGS, "Returns gpio number"},
	{"PI", py_PI, METH_VARARGS, "Returns gpio number"},
	{"PJ", py_PJ, METH_VARARGS, "Returns gpio number"},
	{"PK", py_PK, METH_VARARGS, "Returns gpio number"},
	{"PL", py_PL, METH_VARARGS, "Returns gpio number"},
	{"PM", py_PM, METH_VARARGS, "Returns gpio number"},
	{"PN", py_PN, METH_VARARGS, "Returns gpio number"},
	{"PO", py_PO, METH_VARARGS, "Returns gpio number"},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "gpio",
    NULL,
    -1,
    module_methods
};
#endif

PyMODINIT_FUNC
#if PY_MAJOR_VERSION >= 3
    PyInit_gpio(void) {
#else
    initgpio(void) {
#endif

    PyObject* module = NULL;


#if PY_MAJOR_VERSION >= 3
    module = PyModule_Create(&module_def);
#else
    module = Py_InitModule("gpio", module_methods);
#endif


    if (module == NULL)
#if PY_MAJOR_VERSION >= 3
        return NULL;
#else
        return;
#endif

    PyModule_AddObject(module, "INPUT", Py_BuildValue("i", SUNXI_GPIO_INPUT));
    PyModule_AddObject(module, "OUTPUT", Py_BuildValue("i", SUNXI_GPIO_OUTPUT));
	PyModule_AddObject(module, "PER", Py_BuildValue("i", SUNXI_GPIO_PER));

	PyModule_AddObject(module, "PULLNONE", Py_BuildValue("i", SUNXI_PULL_NONE));
    PyModule_AddObject(module, "PULLUP", Py_BuildValue("i", SUNXI_PULL_UP));
    PyModule_AddObject(module, "PULLDOWN", Py_BuildValue("i", SUNXI_PULL_DOWN));

    PyModule_AddObject(module, "LOW", Py_BuildValue("i", SUNXI_LOW));
    PyModule_AddObject(module, "HIGH", Py_BuildValue("i", SUNXI_HIGH));

#if PY_MAJOR_VERSION >= 3
        return module;
#else
        return;
#endif

}
