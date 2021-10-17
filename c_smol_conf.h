#ifndef C_SMOL_CONF_H
#define C_SMOL_CONF_H

/**
 * @file c_smol_conf.h
 * @brief C Small Configuration File Utility @n
 * CSmolConf is dual licensed, choose any of the two following
 * LICENSE A - Apache 2.0 License, see LICENSE.md
 * LICENSE B - Public Domain (Unlicense), see UNLICENSE.md
*/

#ifdef __cplusplus
    extern "C" {
#endif

#define C_SMOL_CONF_MAJOR 1
#define C_SMOL_CONF_MINOR 1
#define C_SMOL_CONF_PATCH 0

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* -------------------- STRUCTS -------------------- */

/**
 * @brief Custom string with allocated length and true length stored
*/
typedef struct _csc_str
{
    char* str;  //!< String with length of .len
    size_t capacity; //!< Allocated space
    size_t size; //!< Position of the null char
}csc_str;

/**
 * @brief Just a struct contains csc_str pointers
*/
typedef struct _csc_row
{
    csc_str* key;   //!< Pointer to key name
    csc_str* value; //!< Pointer to value
}csc_row;

/**
 * @brief Config file
*/
typedef struct _csc_config
{
    csc_row* config;    //!< Array of csc_rows
    size_t capacity; // Number of csc_row elements allocated
    size_t size; // Current number of csc_row element filled
}csc_config;

/**
 * @brief Check if given string is a float
 * @param str String to check
 * @return True if float
*/
static bool __csc_is_float(const char* str);

/**
 * @brief Check if given string is a int
 * @param str String to check
 * @return True if int
*/
static bool __csc_is_int(const char* str);

/**
 * @brief Check if given string is a unsigned int
 * @param str String to check
 * @return True if unsigned int
*/
static bool __csc_is_uint(const char* str);

/**
 * @brief Check if given path is a file
 * @param path Path to check
 * @return True if it can be accessed
*/
static bool __csc_is_file(const char* path);

/**
 * @brief Check if given string is a bool @n
 * TRUE, FALSE, ON, OFF, 1 and 0 are counted as valid (case insensitive)
 * @param str String to check
 * @return True if bool
*/
static bool __csc_is_bool(const char* str);

/**
 * @brief Initialize csc_str
 * @param len String length
 * @return Initialized csc_str
*/
static csc_str* csc_init_csc_str(size_t len);

/**
 * @brief Initialize csc_str
 * @param str Base string, will be copied
 * @return Initialized csc_str
*/
static csc_str* csc_convert_csc_str(const char* str);

/**
 * @brief Free csc_str
 * @param _csc_str Pointer to free
*/
static void csc_free_csc_str(csc_str* _csc_str);

/**
 * @brief Safe append char to csc_str
 * @param _csc_str String to append to
 * @param c Char to append
*/
static void csc_append_c_csc_str(csc_str* _csc_str, char c);

/**
 * @brief Initiliaze csc_config
 * @param len Number of rows allocated
 * @return Initiliazed csc_config
*/
static csc_config* csc_init_csc_config(size_t len);

/**
 * @brief Read .cscf config file
 * @param _csc_config Initialized configuration
 * @param path Path to config file
 * @return -1 On error, 0 on success
*/
static int __csc_read_config(csc_config* _csc_config, const char* path);

/**
 * @brief Read .cscf config file
 * @param _csc_config Uninitialized configuration
 * @param path Path to config file
 * @return -1 On error, 0 on success
*/
static int csc_read_config(csc_config** _csc_config, const char* path);

/**
 * @brief Read .cscf config file, alias of __csc_read_config
 * @param _csc_config Initialized configuration
 * @param path Path to config file
 * @return -1 On error, 0 on success
*/
static int csc_append_config(csc_config* _csc_config, const char* path);

/**
 * @brief Free csc_config
 * @param _csc_config csc_config to free
*/
static void csc_free_csc_config(csc_config* _csc_config);

/**
 * @brief Append row to csc_config
 * @param _csc_config Destination config
 * @param _csc_row Row to add
 * @return True on success
*/
static bool __csc_append_row_csc_config(csc_config* _csc_config, csc_row _csc_row);

/**
 * @brief Append row to csc_config
 * @param _csc_config Destination config
 * @param csc_key Key
 * @param csc_key Value
 * @return True on success
*/
static bool __csc_append_csc_str_csc_config(csc_config* _csc_config, csc_str* csc_key, csc_str* csc_value);

/**
 * @brief Append row to csc_config
 * @param _csc_config Destination config
 * @param key Key
 * @param key Value
 * @return True on success
*/
static bool csc_append_kvstr_csc_config(csc_config* _csc_config, const char* key, const char* value);

/**
 * @brief Linear search
 * @param _csc_config Configuration
 * @param key_name Key
 * @return Value associated with the key, do not free
*/
static const char* csc_find(csc_config* _csc_config, const char* key_name);

/**
 * @brief Initialize csc_str
 * @param _csc_config Configuration
 * @param key_name Key
 * @param dest Store output
 * @param min Minimum accepted value, fallbacks if inferior to this min
 * @param max Maximum accepted value, fallbacks if superior to this max
 * @return -1 -> No associated value, -2 Associated value isn't a int, 0 Success
*/
static int csc_get_int(csc_config* _csc_config, const char* key_name, int* dest, int min, int max);

/**
 * @brief Initialize csc_str
 * @param _csc_config Configuration
 * @param key_name Key
 * @param dest Store output
 * @param min Minimum accepted value, fallbacks if inferior to this min
 * @param max Maximum accepted value, fallbacks if superior to this max
 * @return -1 -> No associated value, -2 Associated value isn't a int, 0 Success
*/
static int csc_get_uint(csc_config* _csc_config, const char* key_name, unsigned int* dest, unsigned int min, unsigned int max);

/**
 * @brief Initialize csc_str
 * @param _csc_config Configuration
 * @param key_name Key
 * @param dest Store output
 * @param min Minimum accepted value, fallbacks if inferior to this min
 * @param max Maximum accepted value, fallbacks if superior to this max
 * @return -1 -> No associated value, -2 Associated value isn't a int, 0 Success
*/
static int csc_get_float(csc_config* _csc_config, const char* key_name, float* dest, float min, float max);

/**
 * @brief Initialize csc_str
 * @param _csc_config Configuration
 * @param key_name Key
 * @param dest Store output do not free
 * @return -1 -> No associated value, -2 Associated value isn't a int, 0 Success
*/
static int csc_get_path(csc_config* _csc_config, const char* key_name, const char** dest);

/**
 * @brief Initialize csc_str
 * @param _csc_config Configuration
 * @param key_name Key
 * @param dest Store output
 * @return -1 -> No associated value, -2 Associated value isn't a int, 0 Success
*/
static int csc_get_bool(csc_config* _csc_config, const char* key_name, bool* dest);

/**
 * @brief Check if key as an associated value
 * @param _csc_config Configuration
 * @param key_name Key
 * @return True if defined
*/
static bool csc_is_def(csc_config* _csc_config, const char* key_name);

/**
 * @brief Adds entry to configuration
 * @param _csc_config Target config
 * @param key_name Key name
 * @param _long Value
 * @return True on success
*/
static bool csc_append_long_csc_config(csc_config* _csc_config, const char* key_name, long _long);

/**
 * @brief Adds entry to configuration
 * @param _csc_config Target config
 * @param key_name Key name
 * @param _llong Value
 * @return True on success
*/
static bool csc_append_llong_csc_config(csc_config* _csc_config, const char* key_name, long long _llong);

/**
 * @brief Adds entry to configuration
 * @param _csc_config Target config
 * @param key_name Key name
 * @param _int Value
 * @return True on success
*/
static bool csc_append_int_csc_config(csc_config* _csc_config, const char* key_name, int _int);

/**
 * @brief Adds entry to configuration
 * @param _csc_config Target config
 * @param key_name Key name
 * @param _ulong Value
 * @return True on success
*/
static bool csc_append_ulong_csc_config(csc_config* _csc_config, const char* key_name, unsigned long _ulong);

/**
 * @brief Adds entry to configuration
 * @param _csc_config Target config
 * @param key_name Key name
 * @param _ullong Value
 * @return True on success
*/
static bool csc_append_ullong_csc_config(csc_config* _csc_config, const char* key_name, unsigned long long _ullong);

/**
 * @brief Adds entry to configuration
 * @param _csc_config Target config
 * @param key_name Key name
 * @param _uint Value
 * @return True on success
*/
static bool csc_append_uint_csc_config(csc_config* _csc_config, const char* key_name, unsigned int _uint);

/**
 * @brief Adds entry to configuration
 * @param _csc_config Target config
 * @param key_name Key name
 * @param _float Value
 * @return True on success
*/
static bool csc_append_float_csc_config(csc_config* _csc_config, const char* key_name, float _float);

/**
 * @brief Adds entry to configuration
 * @param _csc_config Target config
 * @param key_name Key name
 * @param _double Value
 * @return True on success
*/
static bool csc_append_double_csc_config(csc_config* _csc_config, const char* key_name, double _double);

/**
 * @brief Adds entry to configuration
 * @param _csc_config Target config
 * @param key_name Key name
 * @param _ldouble Value
 * @return True on success
*/
static bool csc_append_long_double_csc_config(csc_config* _csc_config, const char* key_name, long double ldouble);

/**
 * @brief Adds entry to configuration
 * @param _csc_config Target config
 * @param key_name Key name
 * @param _bool Value
 * @return True on success
*/
static bool csc_append_bool_csc_config(csc_config* _csc_config, const char* key_name, bool _bool);

/**
 * @brief Concatenated two configurations
 * @param _csc_config0 Destination
 * @param _csc_config1 Configuration to copys
*/
static void csc_concat_csc_configs(csc_config* _csc_config0, csc_config* _csc_config1);

/**
 * @brief Concatenated two configurations
 * @param _csc_config Configuration
 * @param out_file Opened file
*/
static void __csc_write_config_fp(csc_config* _csc_config, FILE* out_file);

/**
 * @brief Concatenated two configurations
 * @param _csc_config Configuration
 * @param path File path
 * @return True on success
*/
static bool csc_write_config(csc_config* _csc_config, const char* path);

/* -------------------- MACROS -------------------- */

#define CSC_LOG_ERR(FMT, ...)  \
    fprintf(stderr, "ERROR: (%s:%i), " FMT "\n", __FILE__, __LINE__, ##__VA_ARGS__);

#define CSC_ASSERT_W_ERR_LOG(ASSERT, FMT, ...) \
    if(!(ASSERT))   \
    {   \
        CSC_LOG_ERR("ASSERTION ERROR: " FMT, ##__VA_ARGS__)    \
        assert(ASSERT); \
    }

#define CSC_FOPEN_W_ERR_CHK(FPTR, FN, MODE, CLEANUPS, RETCODE)  \
    if (!((FPTR) = fopen(FN, MODE))) \
    {   \
        CSC_LOG_ERR("fopen(%s, %s) failed", FN, MODE)  \
        CLEANUPS  \
        return (RETCODE);   \
    }

/* -------------------- STRING TYPE CHECKING -------------------- */

static bool __csc_is_float(const char* str)
{
    char* end;
    strtof(str, &end);
    return (*end == 0);
}

static bool __csc_is_int(const char* str)
{
    char* end;
    strtol(str, &end, 10);
    return (*end == 0);
}

static bool __csc_is_uint(const char* str)
{
    char* end;
    strtoul(str, &end, 10);
    return (*end == 0);
}

static bool __csc_is_file(const char* path)
{
    FILE* in_file = NULL;
    CSC_FOPEN_W_ERR_CHK(in_file, path, "r", , false)
    fclose(in_file);
    return true;
}

static bool __csc_is_bool(const char* str)
{
    CSC_ASSERT_W_ERR_LOG(str != NULL, "str is a null pointer")
    if (*str == '1' || *str == '0')
        return true;

    char* upper_str = (char*)malloc(sizeof(char) * strlen(str) + 1);
    const char* pstr = str;
    while (*pstr != 0)
    {
        upper_str[pstr - str] = toupper(*pstr);
        pstr++;
    }
    upper_str[pstr - str] = 0;

    if ((strcmp(upper_str, "TRUE") == 0) ||
        (strcmp(upper_str, "FALSE") == 0) ||
        (strcmp(upper_str, "ON") == 0) ||
        (strcmp(upper_str, "OFF") == 0))
    {
        free(upper_str);
        return true;
    }
    free(upper_str);
    return false;
}

/* -------------------- CSC_STR -------------------- */

static csc_str* csc_init_csc_str(size_t len)
{
    CSC_ASSERT_W_ERR_LOG(len != 0, "len is null")
    csc_str* new_csc_str = (csc_str*)malloc(sizeof(csc_str));
    new_csc_str->str = (char*)malloc(sizeof(char) * len);
    new_csc_str->capacity = len;
    new_csc_str->size = 0;
    return new_csc_str;
}

static csc_str* csc_convert_csc_str(const char* str)
{
    CSC_ASSERT_W_ERR_LOG(str != 0, "str is a null pointer")
    size_t str_len = strlen(str);
    csc_str* new_csc_str = (csc_str*)malloc(sizeof(csc_str));
    new_csc_str->str = (char*)malloc(sizeof(char) * str_len + 1);
    strcpy(new_csc_str->str, str);
    new_csc_str->capacity = str_len + 1;
    new_csc_str->size = str_len + 1;
    return new_csc_str;
}

static void csc_free_csc_str(csc_str* _csc_str)
{
    free(_csc_str->str);
    free(_csc_str);
}

static void csc_append_c_csc_str(csc_str* _csc_str, char c)
{
    CSC_ASSERT_W_ERR_LOG(_csc_str != NULL, "_csc_str is a null pointer")
    if ((_csc_str->size + 1) >= _csc_str->capacity)
    {
        _csc_str->str = (char*)realloc(_csc_str->str, sizeof(char) * _csc_str->capacity * 1.5F);
        _csc_str->capacity *= 1.5F;
    }
    _csc_str->str[_csc_str->size++] = c;
    _csc_str->str[_csc_str->size] = 0;
}

/* -------------------- CSC_CONFIG -------------------- */

static csc_config* csc_init_csc_config(size_t len)
{
    csc_config* new_csc_config = (csc_config*)malloc(sizeof(csc_config));
    new_csc_config->config = (csc_row*)malloc(sizeof(csc_row) * len);
    new_csc_config->capacity = len;
    new_csc_config->size = 0;
    return new_csc_config;
}

static int __csc_read_config(csc_config* _csc_config, const char* path)
{
    CSC_ASSERT_W_ERR_LOG(path != NULL, "path is a null pointer")

    FILE* in_config = NULL;
    CSC_FOPEN_W_ERR_CHK(in_config, path, "r", , -1)

    int cur_ch;
    while (!feof(in_config))
    {
        cur_ch = fgetc(in_config);
        while (!feof(in_config) && isspace(cur_ch))
            cur_ch = fgetc(in_config);
        if (cur_ch == '#')
        {
            cur_ch = fgetc(in_config);
            while (!feof(in_config) && (cur_ch != '\n' && cur_ch != '\r'))
                cur_ch = fgetc(in_config);
            continue;
        }
        if (isalnum(cur_ch) || cur_ch == '_')
        {
            csc_str* key = csc_init_csc_str(16);
            csc_append_c_csc_str(key, cur_ch);
            cur_ch = fgetc(in_config);
            while (!feof(in_config) && cur_ch != '=')
            {
                if (isalnum(cur_ch) || cur_ch == '_')
                    csc_append_c_csc_str(key, cur_ch);
                cur_ch = fgetc(in_config);
            }
            csc_str* value = csc_init_csc_str(16);
            cur_ch = fgetc(in_config);
            while (!feof(in_config) && cur_ch != ';')
            {
                if (isprint(cur_ch))
                    csc_append_c_csc_str(value, cur_ch);
                cur_ch = fgetc(in_config);
            }
            __csc_append_csc_str_csc_config(_csc_config, key, value);
            csc_free_csc_str(key);
            csc_free_csc_str(value);
            cur_ch = fgetc(in_config);
        }
        else cur_ch = fgetc(in_config);
    }
    fclose(in_config);
    return 0;
}

static int csc_read_config(csc_config** _csc_config, const char* path)
{
    (*_csc_config) = csc_init_csc_config(16);
    return __csc_read_config((*_csc_config), path);
}

static int csc_append_config(csc_config* _csc_config, const char* path)
{
    return __csc_read_config(_csc_config, path);
}

static void csc_free_csc_config(csc_config* _csc_config)
{
    CSC_ASSERT_W_ERR_LOG(_csc_config != NULL, "_csc_config is a null pointer")
    for (size_t index = 0; index < _csc_config->size; index++)
    {
        csc_free_csc_str(_csc_config->config[index].key);
        csc_free_csc_str(_csc_config->config[index].value);
    }
    free(_csc_config);
}

static bool __csc_append_row_csc_config(csc_config* _csc_config, csc_row _csc_row)
{
    for (size_t index = 0; index < _csc_config->size; index++)
    {
        if (strcmp(_csc_config->config[index].key->str, _csc_row.key->str) == 0)
        {
            CSC_LOG_ERR("Conflicting key : \"%s\" with respective values %s and %s, second key/value pair will be discarded",
                _csc_config->config[index].key->str,
                _csc_config->config[index].value->str,
                _csc_row.value->str)
            return false;
        }
    }
    if (_csc_config->size >= _csc_config->capacity)
    {
        _csc_config->config = (csc_row*)realloc(_csc_config->config, sizeof(csc_row) * _csc_config->capacity * 1.5F);
        _csc_config->capacity *= 1.5F;
    }
    csc_str* new_csc_key = csc_convert_csc_str(_csc_row.key->str);
    csc_str* new_csc_value = csc_convert_csc_str(_csc_row.value->str);
    _csc_config->config[_csc_config->size++] = (csc_row){new_csc_key, new_csc_value};
    return true;
}

static bool __csc_append_csc_str_csc_config(csc_config* _csc_config, csc_str* csc_key, csc_str* csc_value)
{
    return __csc_append_row_csc_config(_csc_config, (csc_row){csc_key, csc_value});
}

static bool csc_append_kvstr_csc_config(csc_config* _csc_config, const char* key, const char* value)
{
    csc_str* new_csc_key = csc_convert_csc_str(key);
    csc_str* new_csc_value = csc_convert_csc_str(value);
    bool res = __csc_append_row_csc_config(_csc_config, (csc_row){new_csc_key, new_csc_value});
    free(new_csc_key);
    free(new_csc_value);
    return res;
}

static const char* csc_find(csc_config* _csc_config, const char* key_name)
{
    CSC_ASSERT_W_ERR_LOG(_csc_config != NULL, "_csc_config is a null pointer")
    for (size_t index = 0; index < _csc_config->size; index++)
    {
        if (strcmp(_csc_config->config[index].key->str, key_name) == 0)
            return _csc_config->config[index].value->str;
    }
    return NULL;
}

/* -------------------- GET VALUE FROM CONFIG -------------------- */

static int csc_get_int(csc_config* _csc_config, const char* key_name, int* dest, int min, int max)
{
    CSC_ASSERT_W_ERR_LOG(_csc_config != NULL, "_csc_config is a null pointer")
    const char* value = csc_find(_csc_config, key_name);
    if (value == NULL)
    {
        CSC_LOG_ERR("Key not found (%s)", key_name)
        return -1;
    }
    if (!__csc_is_int(value))
    {
        CSC_LOG_ERR("Key is not a int (%s)", value)
        return -2;
    }
    *dest = atoi(value);
    if (*dest > max)
        *dest = max;
    else if (*dest < min)
        *dest = min;
    return 0;
}

static int csc_get_uint(
    csc_config* _csc_config,
    const char* key_name,
    unsigned int* dest,
    unsigned int min,
    unsigned int max)
{
    CSC_ASSERT_W_ERR_LOG(_csc_config != NULL, "_csc_config is a null pointer")
    const char* value = csc_find(_csc_config, key_name);
    if (value == NULL)
    {
        CSC_LOG_ERR("Key not found (%s)", key_name)
        return -1;
    }
    if (!__csc_is_uint(value))
    {
        CSC_LOG_ERR("Key is not a unsigned int (%s)", value)
        return -2;
    }
    *dest = strtoul(value, NULL, 10);
    if (*dest > max)
        *dest = max;
    else if (*dest < min)
        *dest = min;
    return 0;
}

static int csc_get_float(
    csc_config* _csc_config,
    const char* key_name,
    float* dest,
    float min,
    float max)
{
    CSC_ASSERT_W_ERR_LOG(_csc_config != NULL, "_csc_config is a null pointer")
    const char* value = csc_find(_csc_config, key_name);
    if (value == NULL)
    {
        CSC_LOG_ERR("Key not found (%s)", key_name)
        return -1;
    }
    if (!__csc_is_float(value))
    {
        CSC_LOG_ERR("Key is not a float (%s)", value)
        return -2;
    }
    *dest = strtof(value, NULL);
    if (*dest > max)
        *dest = max;
    else if (*dest < min)
        *dest = min;
    return 0;
}

static int csc_get_path(csc_config* _csc_config, const char* key_name, const char** dest)
{
    CSC_ASSERT_W_ERR_LOG(_csc_config != NULL, "_csc_config is a null pointer")
    const char* value = csc_find(_csc_config, key_name);
    if (value == NULL)
    {
        CSC_LOG_ERR("Key not found (%s)", key_name)
        return -1;
    }
    if (!__csc_is_file(value))
    {
        CSC_LOG_ERR("Path could not be accessed (%s)", value)
        return -2;
    }
    *dest = value;
    return 0;
}

static int csc_get_bool(csc_config* _csc_config, const char* key_name, bool* dest)
{
    CSC_ASSERT_W_ERR_LOG(_csc_config != NULL, "_csc_config is a null pointer")
    const char* value = csc_find(_csc_config, key_name);
    if (value == NULL)
    {
        CSC_LOG_ERR("Key not found (%s)", key_name)
        return -1;
    }
    if (!__csc_is_bool(value))
    {
        CSC_LOG_ERR("Value is not a bool (%s)", value)
        return -2;
    }
    char* upper_str = (char*)malloc(sizeof(char) * strlen(value) + 1);
    const char* pstr = value;
    while (*pstr != 0)
    {
        upper_str[pstr - value] = toupper(*pstr);
        pstr++;
    }
    upper_str[pstr - value] = 0;
    if (*value == '1' ||
        (strcmp(value, "ON") == 0) ||
        (strcmp(value, "TRUE") == 0))
    {
        *dest = true;
    }
    else *dest = false;
    return 0;
}

static bool csc_is_def(csc_config* _csc_config, const char* key_name)
{
    CSC_ASSERT_W_ERR_LOG(_csc_config != NULL, "_csc_config is a null pointer")
    for (size_t index = 0; index < _csc_config->size; index++)
    {
        if (strcmp(_csc_config->config[index].key->str, key_name) == 0)
            return true;
    }
    return false;
}

/* -------------------- WRITE VALUE TO CONFIG -------------------- */

static bool csc_append_long_csc_config(csc_config* _csc_config, const char* key_name, long _long)
{
    int len = snprintf(NULL, 0, "%li", _long);
    char* str = (char*)malloc(len + 1);
    snprintf(str, len + 1, "%li", _long);
    return csc_append_kvstr_csc_config(_csc_config, key_name, str);
}

static bool csc_append_llong_csc_config(csc_config* _csc_config, const char* key_name, long long _llong)
{
    int len = snprintf(NULL, 0, "%lli", _llong);
    char* str = (char*)malloc(len + 1);
    snprintf(str, len + 1, "%lli", _llong);
    return csc_append_kvstr_csc_config(_csc_config, key_name, str);
}

static bool csc_append_int_csc_config(csc_config* _csc_config, const char* key_name, int _int)
{
    int len = snprintf(NULL, 0, "%i", _int);
    char* str = (char*)malloc(len + 1);
    snprintf(str, len + 1, "%i", _int);
    return csc_append_kvstr_csc_config(_csc_config, key_name, str);
}

static bool csc_append_ulong_csc_config(csc_config* _csc_config, const char* key_name, unsigned long _ulong)
{
    int len = snprintf(NULL, 0, "%lu", _ulong);
    char* str = (char*)malloc(len + 1);
    snprintf(str, len + 1, "%lu", _ulong);
    return csc_append_kvstr_csc_config(_csc_config, key_name, str);
}

static bool csc_append_ullong_csc_config(csc_config* _csc_config, const char* key_name, unsigned long long _ullong)
{
    int len = snprintf(NULL, 0, "%llu", _ullong);
    char* str = (char*)malloc(len + 1);
    snprintf(str, len + 1, "%llu", _ullong);
    return csc_append_kvstr_csc_config(_csc_config, key_name, str);
}

static bool csc_append_uint_csc_config(csc_config* _csc_config, const char* key_name, unsigned int _uint)
{
    int len = snprintf(NULL, 0, "%u", _uint);
    char* str = (char*)malloc(len + 1);
    snprintf(str, len + 1, "%u", _uint);
    return csc_append_kvstr_csc_config(_csc_config, key_name, str);
}

static bool csc_append_float_csc_config(csc_config* _csc_config, const char* key_name, float _float)
{
    int len = snprintf(NULL, 0, "%f", _float);
    char* str = (char*)malloc(len + 1);
    snprintf(str, len + 1, "%f", _float);
    return csc_append_kvstr_csc_config(_csc_config, key_name, str);
}

static bool csc_append_double_csc_config(csc_config* _csc_config, const char* key_name, double _double)
{
    int len = snprintf(NULL, 0, "%lf", _double);
    char* str = (char*)malloc(len + 1);
    snprintf(str, len + 1, "%lf", _double);
    return csc_append_kvstr_csc_config(_csc_config, key_name, str);
}

static bool csc_append_long_double_csc_config(csc_config* _csc_config, const char* key_name, long double ldouble)
{
    int len = snprintf(NULL, 0, "%Lf", ldouble);
    char* str = (char*)malloc(len + 1);
    snprintf(str, len + 1, "%Lf", ldouble);
    return csc_append_kvstr_csc_config(_csc_config, key_name, str);
}

static bool csc_append_bool_csc_config(csc_config* _csc_config, const char* key_name, bool _bool)
{
    return csc_append_uint_csc_config(_csc_config, key_name, (unsigned int)_bool);
}

/* -------------------- OTHERS CONFIG RELATED FUNCTIONS -------------------- */

static void csc_concat_csc_configs(csc_config* _csc_config0, csc_config* _csc_config1)
{
    CSC_ASSERT_W_ERR_LOG(_csc_config0 != NULL, "_csc_config0 is a null pointer")
    CSC_ASSERT_W_ERR_LOG(_csc_config1 != NULL, "_csc_config1 is a null pointer")

    for (size_t index = 0; index < _csc_config1->size; index++)
        __csc_append_csc_str_csc_config(_csc_config0, _csc_config1->config[index].key, _csc_config1->config[index].value);
}

static void __csc_write_config_fp(csc_config* _csc_config, FILE* out_file)
{
    CSC_ASSERT_W_ERR_LOG(_csc_config != NULL, "_csc_config is a null pointer")
    CSC_ASSERT_W_ERR_LOG(out_file != NULL, "out_file is a null pointer")

    fprintf(out_file, "\n# Generated by CSmolConf v%i.%i.%i\n", C_SMOL_CONF_MAJOR, C_SMOL_CONF_MINOR, C_SMOL_CONF_PATCH);
    for (size_t index = 0; index < _csc_config->size; index++)
        fprintf(out_file, "%s=%s;\n", _csc_config->config[index].key->str, _csc_config->config[index].value->str);
}

static bool csc_write_config(csc_config* _csc_config, const char* path)
{
    FILE* out_file;
    CSC_FOPEN_W_ERR_CHK(out_file, path, "w+", , false)
    __csc_write_config_fp(_csc_config, out_file);
    fclose(out_file);
    return true;
}

#undef CSC_LOG_ERR
#undef CSC_ASSERT_W_ERR_LOG
#undef CSC_FOPEN_W_ERR_CHK

#ifdef __cplusplus
    }
#endif

#endif /* C_SMOL_CONF_H */