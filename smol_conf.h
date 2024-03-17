/**
 * Small Configuration File Utility, licensed under the zlib license
 * Copyright (C) 2022 JeFaisDesSpaghettis

 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a product,
 *    an acknowledgment in the product documentation would be appreciated but is
 *    not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef SMOL_CONF_H
#define SMOL_CONF_H

#ifdef __cplusplus
    extern "C" {
#endif

#define SMOL_CONF_MAJOR 2
#define SMOL_CONF_MINOR 0
#define SMOL_CONF_PATCH 1

#include <ctype.h>
#include <float.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t SCNF_HASH_SIZE = 256;

#define SCNF_NO_CLAMP_INT_MIN INT32_MIN
#define SCNF_NO_CLAMP_INT_MAX INT32_MAX

#define SCNF_NO_CLAMP_UINT_MIN 0
#define SCNF_NO_CLAMP_UINT_MAX UINT32_MAX

#define SCNF_NO_CLAMP_FLT_MIN -FLT_MAX
#define SCNF_NO_CLAMP_FLT_MAX FLT_MAX

/* -------------------- TYPEDEFS -------------------- */

typedef struct scnf_kvpair
{
    char* key;
    char* value;
}scnf_kvpair;

typedef struct scnf_index_node
{
    size_t index;
    struct scnf_index_node* next;
}scnf_index_node;

typedef struct scnf_config
{
    scnf_kvpair* config;
    size_t capacity;
    size_t size;
    scnf_index_node** hash_to_index;
}scnf_config;

typedef enum SCNF_ERROR
{
    SCNF_SUCCESS = 0,
    SCNF_FAILED_FOPEN = 1,
    SCNF_FAILED_SYNTAX_ERROR = 2,
    SCNF_FAILED_NOT_FOUND = 3,
    SCNF_FAILED_WRONG_TYPE = 4
}SCNF_ERROR;

/* -------------------- DEFINITIONS -------------------- */

// SCNF Config
static uint32_t scnf_hash_(const char* str);
static scnf_config* scnf_init_scnf_config(size_t len);
static SCNF_ERROR scnf_read_config_(scnf_config* config, const char* path);
static SCNF_ERROR scnf_read_config(scnf_config** pconfig, const char* path);
static SCNF_ERROR scnf_append_config(scnf_config* config, const char* path);
static void scnf_free_scnf_config(scnf_config* config);
static bool scnf_config_append_kv(scnf_config* config, const char* key, const char* value);
static const char* scnf_find(scnf_config* config, const char* key);

// Read
static SCNF_ERROR scnf_get_bool(scnf_config* config, const char* key, bool* dest);
static SCNF_ERROR scnf_get_int(scnf_config* config, const char* key, int* dest, int min, int max);
static SCNF_ERROR scnf_get_uint(scnf_config* config, const char* key, unsigned int* dest, unsigned int min, unsigned int max);
static SCNF_ERROR scnf_get_float(scnf_config* config, const char* key, float* dest, float min, float max);
static bool scnf_file_exists_(const char* path);
static SCNF_ERROR scnf_get_path(scnf_config* config, const char* key, const char** dest);

// Write
static bool scnf_append_bool_scnf_config(scnf_config* config, const char* key, bool value);
static bool scnf_append_int_scnf_config(scnf_config* config, const char* key, int value);
static bool scnf_append_uint_scnf_config(scnf_config* config, const char* key, unsigned int value);
static bool scnf_append_float_scnf_config(scnf_config* config, const char* key, float value);

// OTHER
static void scnf_concat_scnf_configs(scnf_config* dest_config, scnf_config* src_config);
static void scnf_write_config_(scnf_config* config, FILE* out_file);
static bool scnf_write_config(scnf_config* config, const char* path);

/* -------------------- SCNF_CONFIG -------------------- */

static uint32_t scnf_hash_(const char* str)
{
    // DJB2 Hash
    uint32_t hash = 5381;
    int c;
    while ((c = *str++) != '\0')
        hash = ((hash << 5) + hash) + c;
    return hash;
}

static scnf_config* scnf_init_scnf_config(size_t len)
{
    scnf_config* config = (scnf_config*)malloc(sizeof(scnf_config));
    config->config = (scnf_kvpair*)malloc(sizeof(scnf_kvpair) * len);
    config->capacity = len;
    config->size = 0;
    config->hash_to_index = (scnf_index_node**)calloc(SCNF_HASH_SIZE, sizeof(scnf_index_node*));
    return config;
}

static SCNF_ERROR scnf_read_config_(scnf_config* config, const char* path)
{
    FILE* in_config = NULL;
    if ((in_config = fopen(path, "r")) == NULL)
        return SCNF_FAILED_FOPEN;

    char* cur_line = NULL;
    size_t cur_line_len = 0;
    size_t line_count = 0;
    while(!feof(in_config))
    {
        getline(&cur_line, &cur_line_len, in_config);
        ++line_count;

        // Skip leading spaces
        char* cur_line_start = cur_line;
        while(*cur_line_start == ' ') cur_line_start++;

        // Check for comment
        char* cur_line_end = strchr(cur_line_start, '#');

        if (cur_line_end != NULL) *cur_line_end = '\0';
        else cur_line_end = cur_line_start + strlen(cur_line_start);
        if (cur_line_end[-1] == '\n')
        {
            cur_line_end[-1] = '\0';
            cur_line_end--;
        }

        if (cur_line_end == cur_line_start) continue;

        char* equal_ptr = strchr(cur_line_start, '=');
        if (equal_ptr == NULL)
        {
            printf("Error at line %zu, missing '=' sign\n", line_count);
            free(cur_line);
            fclose(in_config);
            return SCNF_FAILED_SYNTAX_ERROR;
        }

        // Get key and value
        size_t key_len = equal_ptr - cur_line_start;
        size_t value_len = (cur_line_end) - (equal_ptr + 1);

        if (key_len == 0 || value_len == 0)
        {
            printf("Error at line %zu, key/value pair malformed\n", line_count);
            free(cur_line);
            fclose(in_config);
            return SCNF_FAILED_SYNTAX_ERROR;
        }

        char* key = (char*)malloc(key_len + 1);
        char* value = (char*)malloc(value_len + 1);
        strncpy(key, cur_line_start, key_len);
        key[key_len] = '\0';
        strncpy(value, (equal_ptr + 1), value_len);
        value[value_len] = '\0';

        // Check key validity
        char* key_it = key;
        while (*key_it != '\0')
        {
            if(!isalnum(*key_it) && (*key_it) != '_')
            {
                printf("Error at line %zu, key is non-alphanumeric\n", line_count);
                free(cur_line);
                fclose(in_config);
                free(key);
                free(value);
                return SCNF_FAILED_SYNTAX_ERROR;
            }
            key_it++;
        }
        char* value_it = value;
        while (*value_it != '\0')
        {
            if(!isprint(*value_it))
            {
                printf("Error at line %zu, value is not a printable character\n", line_count);
                free(cur_line);
                fclose(in_config);
                free(key);
                free(value);
                return SCNF_FAILED_SYNTAX_ERROR;
            }
            value_it++;
        }

        scnf_config_append_kv(config, key, value);
        free(key);
        free(value);
    }
    if (cur_line) { free(cur_line); }
    fclose(in_config);
    return SCNF_SUCCESS;
}

static SCNF_ERROR scnf_read_config(scnf_config** pconfig, const char* path)
{
    (*pconfig) = scnf_init_scnf_config(16);
    return scnf_read_config_((*pconfig), path);
}

static SCNF_ERROR scnf_append_config(scnf_config* config, const char* path)
{
    return scnf_read_config_(config, path);
}

static void scnf_free_scnf_config(scnf_config* config)
{
    for (size_t index = 0; index < SCNF_HASH_SIZE; index++)
    {
        scnf_index_node* index_node;
        while (config->hash_to_index[index] != NULL)
        {
            index_node = config->hash_to_index[index];
            config->hash_to_index[index] = config->hash_to_index[index]->next;
            free(index_node);
        }
    }
    free(config->hash_to_index);
    for (size_t index = 0; index < config->size; index++)
    {
        free(config->config[index].key);
        free(config->config[index].value);
    }
    free(config->config);
    free(config);
}

static bool scnf_config_append_kv(scnf_config* config, const char* key, const char* value)
{
    if (scnf_find(config, key) != NULL)
        return false;
    if (config->size >= config->capacity)
    {
        config->config = (scnf_kvpair*)realloc(config->config, sizeof(scnf_kvpair) * config->capacity * 1.5F);
        config->capacity *= 1.5F;
    }
    config->config[config->size] = (scnf_kvpair){strdup(key), strdup(value)};

    uint32_t hash = scnf_hash_(config->config[config->size].key);

    scnf_index_node** target_nodep = &config->hash_to_index[hash % SCNF_HASH_SIZE];
    if ((*target_nodep) == NULL)
    {
        (*target_nodep) = (scnf_index_node*)malloc(sizeof(scnf_index_node));
        (*target_nodep)->index = config->size;
        (*target_nodep)->next = NULL;
    }
    else
    {
        target_nodep = &(*target_nodep)->next;
        while (*target_nodep != NULL)
            (*target_nodep) = (*target_nodep)->next;
        (*target_nodep) = (scnf_index_node*)malloc(sizeof(scnf_index_node));
        (*target_nodep)->index = config->size;
        (*target_nodep)->next = NULL;
    }
    config->size++;
    return true;
}

static const char* scnf_find(scnf_config* config, const char* key)
{
    uint32_t hash = scnf_hash_(key);
    scnf_index_node* target_node = config->hash_to_index[hash % SCNF_HASH_SIZE];
    if (target_node == NULL)
    {
        return NULL;
    }
    if (target_node->next == NULL)
    {
        return config->config[target_node->index].value;
    }
    else
    {
        target_node = target_node->next;
        while (strcmp(config->config[target_node->index].key, key) != 0)
        {
            if (target_node == NULL)
            {
                return NULL;
            }
            if (target_node->next == NULL)
            {
                return config->config[target_node->index].value;
            }
            target_node = target_node->next;
        }
    }
    return NULL;
}

/* -------------------- READ -------------------- */

static SCNF_ERROR scnf_get_bool(scnf_config* config, const char* key, bool* dest)
{
    const char* value = scnf_find(config, key);
    if (value == NULL)
        return SCNF_FAILED_NOT_FOUND;

    char* upper_str = (char*)malloc(strlen(value) + 1);
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
        free(upper_str);
        return SCNF_SUCCESS;
    }
    else if (*value == '0' ||
        (strcmp(value, "OFF") == 0) ||
        (strcmp(value, "FALSE") == 0))
    {
        *dest = false;
        free(upper_str);
        return SCNF_SUCCESS;
    }
    return SCNF_FAILED_WRONG_TYPE;
}

static SCNF_ERROR scnf_get_int(scnf_config* config, const char* key, int* dest, int min, int max)
{
    const char* value = scnf_find(config, key);
    if (value == NULL)
        return SCNF_FAILED_NOT_FOUND;
    char* end;
    *dest = strtol(value, &end, 10);
    if (*end != 0)
        return SCNF_FAILED_WRONG_TYPE;
    if (*dest > max)
        *dest = max;
    else if (*dest < min)
        *dest = min;
    return SCNF_SUCCESS;
}

static SCNF_ERROR scnf_get_uint(
    scnf_config* config,
    const char* key,
    unsigned int* dest,
    unsigned int min,
    unsigned int max)
{
    const char* value = scnf_find(config, key);
    if (value == NULL)
        return SCNF_FAILED_NOT_FOUND;
    char* end;
    *dest = strtoul(value, &end, 10);
    if (*end != 0)
        return SCNF_FAILED_WRONG_TYPE;
    if (*dest > max)
        *dest = max;
    else if (*dest < min)
        *dest = min;
    return SCNF_SUCCESS;
}

static SCNF_ERROR scnf_get_float(
    scnf_config* config,
    const char* key,
    float* dest,
    float min,
    float max)
{
    const char* value = scnf_find(config, key);
    if (value == NULL)
        return SCNF_FAILED_NOT_FOUND;
    char* end;
    *dest = strtof(value, &end);
    if (*end != 0)
        return SCNF_FAILED_WRONG_TYPE;
    if (*dest > max)
        *dest = max;
    else if (*dest < min)
        *dest = min;
    return SCNF_SUCCESS;
}

static bool scnf_file_exists_(const char* path)
{
    FILE* in_file = NULL;
    if ((in_file = fopen(path, "r")) == NULL)
        return false;
    fclose(in_file);
    return true;
}

static SCNF_ERROR scnf_get_path(scnf_config* config, const char* key, const char** dest)
{
    const char* value = scnf_find(config, key);
    if (value == NULL)
        return SCNF_FAILED_NOT_FOUND;
    if (!scnf_file_exists_(value))
        return SCNF_FAILED_WRONG_TYPE;
    (*dest) = value;
    return SCNF_SUCCESS;
}

/* -------------------- WRITE -------------------- */

static bool scnf_append_bool_scnf_config(scnf_config* config, const char* key, bool value)
{
    return scnf_append_uint_scnf_config(config, key, (unsigned int)value);
}

static bool scnf_append_int_scnf_config(scnf_config* config, const char* key, int value)
{
    int len = snprintf(NULL, 0, "%i", value);
    char* value_str = (char*)malloc(len + 1);
    snprintf(value_str, len + 1, "%i", value);
    return scnf_config_append_kv(config, key, value_str);
}

static bool scnf_append_uint_scnf_config(scnf_config* config, const char* key, unsigned int value)
{
    int len = snprintf(NULL, 0, "%u", value);
    char* value_str = (char*)malloc(len + 1);
    snprintf(value_str, len + 1, "%u", value);
    return scnf_config_append_kv(config, key, value_str);
}

static bool scnf_append_float_scnf_config(scnf_config* config, const char* key, float value)
{
    int len = snprintf(NULL, 0, "%f", value);
    char* value_str = (char*)malloc(len + 1);
    snprintf(value_str, len + 1, "%f", value);
    return scnf_config_append_kv(config, key, value_str);
}

/* -------------------- OTHER-------------------- */

static void scnf_concat_scnf_configs(scnf_config* dest_config, scnf_config* src_config)
{
    for (size_t index = 0; index < src_config->size; index++)
        scnf_config_append_kv(dest_config, src_config->config[index].key, src_config->config[index].value);
}

static void scnf_write_config_(scnf_config* config, FILE* out_file)
{
    fprintf(out_file, "# Generated by CSmolConf v%i.%i.%i\n", SMOL_CONF_MAJOR, SMOL_CONF_MINOR, SMOL_CONF_PATCH);
    for (size_t index = 0; index < config->size; index++)
        fprintf(out_file, "%s=%s\n", config->config[index].key, config->config[index].value);
}

static bool scnf_write_config(scnf_config* config, const char* path)
{
    FILE* out_file;
    if ((out_file = fopen(path, "w+")) == NULL)
        return false;
    scnf_write_config_(config, out_file);
    fclose(out_file);
    return true;
}

#ifdef __cplusplus
    }
#endif

#endif /* SMOL_CONF_H */