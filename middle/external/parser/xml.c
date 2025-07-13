/*********************************************************************************************************
 *  ------------------------------------------------------------------------------------------------------
 *  file description
 *  ------------------------------------------------------------------------------------------------------
 *         \file  xml.h
 *         \unit  xml
 *        \brief  This is a C language version of xml parser
 *       \author  Lamdonn
 *      \version  v1.0.0
 *      \license  GPL-2.0
 *    \copyright  Copyright (C) 2023 Lamdonn.
 ********************************************************************************************************/
#include "xml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* dump buffer define */
typedef struct
{
    char* address;                          /**< buffer base address */
    unsigned int size;                      /**< size of buffer */
    unsigned int end;                       /**< end of buffer used */
} BUFFER;

/* attribute define */
typedef struct ATTR
{
    struct ATTR *next;                      /**< next attribute */
    char* name;                             /**< name of attribute */
    char* value;                            /**< value of attribute */
} ATTR;

/* xml define */
typedef struct XML
{
    struct XML *next;                       /**< next xml */
    char* name;                             /**< name of xml */
    ATTR* attrs;                            /**< attributes */
    char* text;                             /**< text of xml */
    struct XML *child;                      /**< child of xml */
} XML;

static const char* lbegin = NULL;           /**< beginning of line */
static int etype = XML_E_OK;                /**< type of error message */
static int eline = 0;                       /**< line of error message */
static int ecolumn = 0;                     /**< column of error message */

/* set error message and type */
#define E(type)                             (etype=(type),ecolumn=text-lbegin)

/**
 *  \brief for analysing failed parses
 *  \param[out] line: error line
 *  \param[out] column: error column
 *  \return error type
 */
int xml_error_info(int* line, int* column)
{
    /* No error occurred, return directly */
    if (etype == XML_E_OK) return XML_E_OK;

    /* Output the line and column where the error is located */
    if (line) *line = eline;
    if (column) *column = ecolumn;

    /* Return error type */
    return etype;
}

/**
 *  \brief get the smallest power of 2 not greater than x.
 *  \param[in] x: positive integer
 *  \return the smallest power of 2 not greater than x
 */
static int pow2gt(int x)
{
    int b = sizeof(int) * 8;
    int i = 1;

    --x;
    while (i < b)
    {
        x |= (x >> i);
        i <<= 1;
    }

    return x + 1;
}

/**
 *  \brief Duplicate a given string.
 * 
 *  \param[in] str String to be duplicated.
 *  \return Pointer to the duplicated string if successful, NULL otherwise.
 */
static char* xml_strdup(const char* str)
{
    int size = (int)strlen(str) + 1;
    char* s;

    /* Allocate memory for the new string */
    s = (char*)malloc(size);
    if (!s) return NULL;

    /* Copy the given string into the allocated memory */
    memcpy(s, str, size);

    return s;
}

/**
* \brief Compare two strings.
*
* \param[in] s1 The first string to compare.
* \param[in] s2 The second string to compare.
* \return 0 if the strings are equal, a negative value if s1 is less than s2, or a positive value if s1 is greater than s2.
*/
static int string_compare(const char* s1, const char* s2)
{
    if (s1 == s2) return 0;             /* If the memory addresses are the same, the strings are equal */
    if (!s1) return -1;                 /* If s1 is NULL, it is considered less than s2 */
    if (!s2) return 1;                  /* If s2 is NULL, it is considered greater than s1 */

    /* Compare the strings character by character */
    while (*s1 == *s2)
    {
        if (*s1 == 0) return 0; /* If both characters are null terminators, the strings are equal */
        s1++, s2++; /* Move to the next characters */
    }

    /* Return the difference between the ASCII values of the characters at the current positions */
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

/**
 *  \brief confirm whether buf still has the required capacity, otherwise add capacity.
 *  \param[in] buf: buf handle
 *  \param[in] needed: required capacity
 *  \return 1 success or 0 fail
 */
static int expansion(BUFFER *buf, unsigned int needed)
{
    char* address;
    int size;
    if (!buf) return 0;
    needed += buf->end;
    if (needed <= buf->size) return 1; /* there is still enough space in the current buf */
    size = pow2gt(needed);
    address = (char*)realloc(buf->address, size);
    if (!address) return 0;
    buf->size = size;
    buf->address = address;
    return 1;
}
#define buf_append(n)          expansion(buf, (n)) /* append n size space for buf */
#define buf_putc(c)            (buf->address[buf->end++]=(c)) /* put a non zero character into buf */
#define buf_puts(s, len)       do{for(int i=0;i<len;i++)buf_putc((s)[i]);}while(0) /* put the specified length character set to buf */
#define buf_end()              (buf->address[buf->end]) /* obtain the address at the tail of buf */

/**
* \brief Skip leading whitespace characters in a text.
*
* \param[in] text The text to process.
* \return A pointer to the first non-whitespace character in the text.
*/
static const char* skip(const char* text)
{
    while (text && *text && (unsigned char)(*text) <= ' ')
    {
        /* when a newline character is encountered, record the current parsing line */
        if (*text == '\n') 
        { 
            /* Increase eline and set lbegin to the current position */
            eline++; 
            lbegin = text; 
        } 

        /* Move to the next character */
        text++;
    }

    /* Return a pointer to the first non-whitespace character */
    return text; 
}

/**
* \brief Create a new ATTR object.
*
* \return A pointer to a newly allocated ATTR object with its memory initialized to zero.
*/
static ATTR* new_attr()
{
    /* Allocate memory for a new ATTR object */
    ATTR* attr = (ATTR*)malloc(sizeof(ATTR));

    /* Initialize the memory of the ATTR object to zero */
    memset(attr, 0, sizeof(ATTR));

    return attr;
}

/**
* \brief Delete a linked list of ATTR objects and free the memory.
*
* \param[in] attrs The head of the linked list of ATTR objects to delete.
*/
static void attribute_delete(ATTR* attrs)
{
    ATTR* next;

    while (attrs)
    {
        /* Store the next ATTR object */
        next = attrs->next;

        /* Free the memory allocated for the name */
        if (attrs->name) free(attrs->name);

        /* Free the memory allocated for the value */
        if (attrs->value) free(attrs->value);

        /* Free the memory allocated for the current ATTR object */
        free(attrs);

        /* Move to the next ATTR object in the linked list */
        attrs = next;
    }
}

/**
* \brief Create a new XML node.
*
* \return A pointer to a newly allocated XML node object with its memory initialized to zero, or NULL if memory allocation fails.
*/
static xml_t new_node(void)
{
    /* Allocate memory for a new XML node object */
    xml_t node = (xml_t)malloc(sizeof(XML)); 

    /* Check if memory allocation fails, return NULL */
    if (!node) return NULL; 

    /* Initialize the memory of the XML node object to zero */
    memset(node, 0, sizeof(XML)); 

    /* Return a pointer to the newly created XML node object */
    return node; 
}

/**
 *  \brief delete a xml object.
 *  \param[in] xml: xml handle
 *  \return none
 */
void xml_delete(xml_t xml)
{
    xml_t next;

    if (xml)
    {
        /* Free the memory allocated for the name */
        if (xml->name) free(xml->name); 

        /* Free the memory allocated for the text */
        if (xml->text) free(xml->text); 

        /* Delete the attributes of the XML node */
        if (xml->attrs) attribute_delete(xml->attrs); 

        /* Recursively delete the child node */
        while (xml->child)
        {
            /* Store the next child node */
            next = xml->child->next; 

            /* Delete the child node */
            xml_delete(xml->child);

            /* Move to the next child node */
            xml->child = next; 
        }

        /* Free the memory allocated for the XML node itself */
        free(xml); 
    }
}

/**
* \brief Check the validity of an XML node name.
*
* \param[in] name The name of the XML node to check.
* \param[out] len The length of the XML node name.
* \return 0:illegal, 1:end<0>, 2:delim.
*/
static int check_name(const char* name, int *len)
{
    int ret = 1;
    char *s = (char *)name;
    int l = 0;

    /* Check the validity of the beginning */
    if (*s > 0 && !isalpha(*s)) return 0; 

    /* Check the name specification, and calculate the length */
    while (*s) 
    {
        if (*s == '>' || *s == ' ' || *s == '\t') 
        {
            ret = 2; 
            break;
        }

        /* illegal character */
        if (*s > 0 && !(isalnum(*s) || *s=='_' || *s=='-' || *s=='.' || *s==':')) 
        {
            ret = 0; 
            break;
        }

        s++, l++;
    }

    /* Output length */
    if (len) *len = l;

    return ret;
}

/**
* \brief Duplicate an XML node name with validity check.
*
* \param[in] name The name of the XML node to duplicate.
* \return A newly allocated string containing the duplicated name, or NULL if the name is invalid or memory allocation fails.
*/
static char *name_strdup(const char* name)
{
    char *s;
    int len = 0, ret = 0;

    /* Check the validity of name and return its length */
    ret = check_name(name, &len);
    if (ret == 0 || ret == 2) return NULL;

    /* Allocating space */
    s = malloc(len + 1); 
    if (!s) return NULL;

    /* Copy string */
    memcpy(s, name, len);
    s[len] = 0;

    return s;
}

/**
* \brief Duplicate an XML node value with validity check.
*
* \param[in] value The value of the XML node to duplicate.
* \return A newly allocated string containing the duplicated value, or NULL if the value contains non-printable characters or memory allocation fails.
*/
static char *value_strdup(const char* value)
{
    char *s = (char *)value;
    int len = 0;

    while (*s)
    {
        if (*s > 0 && !isprint(*s)) return NULL; /* Check the value specification, and calculate the length */

        s++, len++;
    }

    /* Allocating space and assigning */
    s = malloc(len + 1);
    if (!s) return NULL;
    memcpy(s, value, len);
    s[len] = 0;

    return s;
}

/**
 *  \brief create a xml object.
 *  \param[in] name: name of xml node
 *  \return new xml object
 */
xml_t xml_create(const char* name)
{
    xml_t xml = NULL;

    if (!name) return NULL;

    /* Create a new XML node */
    xml = new_node();
    if (!xml) return NULL;

    /* Assign to the name of XML */
    xml->name = name_strdup(name);
    if (!xml->name) 
    {
        free(xml); 
        return NULL;
    }

    return xml;
}

/**
 *  \brief add an attribute to xml.
 *  \param[in] xml: xml handle
 *  \param[in] name: attribute name
 *  \param[in] value: attribute value
 *  \return 1 success or 0 fail
 */
int xml_add_attribute(xml_t xml, const char *name, const char *value)
{
    ATTR* attr;

    /* Input value validity check */
    if (!xml) return 0;
    if (!name) return 0;
    if (!value) return 0;

    /* Create a new XML attribute */
    attr = new_attr();
    if (!attr) return 0;

    /* Assign to the attribute name */
    attr->name = name_strdup(name);
    if (!attr->name) goto FAIL;

    /* Assign to the attribute value */
    attr->value = value_strdup(value);
    if (!attr->value) goto FAIL;

    /* Attach attribute */
    attr->next = xml->attrs;
    xml->attrs = attr;

    return 1;

FAIL:
    if (attr->name) free(attr->name);
    if (attr->value) free(attr->value);
    free(attr);
    return 0;
}

/**
 *  \brief remove an attribute from xml.
 *  \param[in] xml: xml handle
 *  \param[in] name: attribute name or NULL matches all attributes
 *  \param[in] index: index
 *  \return 1 success or 0 fail
 */
int xml_remove_attribute(xml_t xml, const char *name, int index)
{
    ATTR *attr, *prev;

    if (!xml) return 0;
    if (index < 0) return 0;

    attr = xml->attrs;
    while (attr)
    {
        /* Check if the attribute name matches */
        if (!name || !string_compare(name, attr->name))
        {
            index--;

            /* Break if the correct attribute is found */
            if (index < 0) break;
        }

        prev = attr;
        attr = attr->next;
    }
    /* If the index is not found */
    if (index >= 0) return 0;
    /* If the attribute is not found */
    if (!attr) return 0;

    /* Adjust the pointers to remove the attribute */
    if (prev) prev->next = attr->next;
    /* Update the head of the attribute list if necessary */
    else xml->attrs = attr->next;

    /* Free the memory for the attribute name and value */
    if (attr->name) free(attr->name);
    if (attr->value) free(attr->value);

    /* Free the memory for the attribute object */
    free(attr);

    return 1;
}

/**
 *  \brief get attribute of xml.
 *  \param[in] xml: xml handle
 *  \param[in] name: attribute name or NULL matches all attributes
 *  \param[in] index: index
 *  \return value of attribute
 */
const char* xml_get_attribute(xml_t xml, const char *name, int index)
{
    ATTR *attr, *prev;

    if (!xml) return NULL;
    if (index < 0) return NULL;

    attr = xml->attrs;
    while (attr)
    {
        /* Check if the attribute name matches */
        if (!name || !string_compare(name, attr->name))
        {
            index--;

            /* Break if the correct attribute is found */
            if (index < 0) break;
        }
        
        prev = attr;
        attr = attr->next;
    }
    /* If the index is not found */
    if (index >= 0) return NULL;
    /* If the attribute is not found */
    if (!attr) return NULL;

    return attr->value;
}

/**
 *  \brief inserts the specified xml object into another xml object.
 *  \param[in] xml: xml handle
 *  \param[in] index: index
 *  \param[in] ins: insert xml object
 *  \return 1 success or 0 fail
 */
int xml_insert(xml_t xml, int index, xml_t ins)
{
    xml_t prev;

    /* Input value validity check */
    if (!xml) return 0;
    if (!ins) return 0;
    if (index < 0) return 0;

    /* Insert the node at the beginning */
    if (index == 0)
    {
        ins->next = xml->child;
        xml->child = ins;
    }

    /* Move to the desired index */
    prev = xml->child;
    while (prev && --index)
    {
        prev = prev->next;
    }
    /* If the index is not valid */
    if (!prev) return 0;

    /* Attach xml object */
    ins->next = prev->next;
    prev->next = ins;

    return 1;
}

/**
 *  \brief remove an child xml from xml.
 *  \param[in] xml: xml handle
 *  \param[in] name: child xml name or NULL matches all child
 *  \param[in] index: index
 *  \return 1 success or 0 fail
 */
int xml_remove(xml_t xml, const char *name, int index)
{
    xml_t prev = NULL, node;

    /* Input value validity check */
    if (!xml) return 0;
    if (xml->text) return 0;
    if (index < 0) return 0;

    node = xml->child;
    while (node)
    {
        /* Check if the node name matches */
        if (!name || !string_compare(name, node->name))
        {
            index--;

            /* Break if the correct node is found */
            if (index < 0) break;
        }

        prev = node;
        node = node->next;
    }
    /* If the index is not found */
    if (index >= 0) return 0;
    /* If the node is not found */
    if (!node) return 0;

    /* Adjust the pointers to remove the node */
    if (prev) prev->next = node->next;
    /* Update the head of the child list if necessary */
    else xml->child = node->next;

    /* Delete the node and free memory */
    xml_delete(node);

    return 1;
}

/**
 *  \brief goes to the specified child object.
 *  \param[in] xml: xml handle
 *  \param[in] name: children xml name or NULL matches all children
 *  \param[in] index: index
 *  \return specified child object
 */
xml_t xml_to(xml_t xml, const char *name, int index)
{
    xml_t prev = NULL, node;

    /* Input value validity check */
    if (!xml) return NULL;
    if (index < 0) return NULL;

    node = xml->child;
    while (node)
    {
        /* Check if the node name matches */
        if (!name || !string_compare(name, node->name))
        {
            index--;

            /* Break if the correct node is found */
            if (index < 0) break;
        }

        prev = node;
        node = node->next;
    }
    /* If the index is not found */
    if (index >= 0) return NULL;
    /* If the node is not found */
    if (!node) return NULL;

    return node;
}

/**
 *  \brief set text into xml.
 *  \param[in] xml: xml handle
 *  \param[in] text: text
 *  \return 1 success or 0 fail
 */
int xml_set_text(xml_t xml, const char *text)
{
    char *s;
    if (!xml) return 0;
    if (!text) return 0;

    /* Duplicate the text content */
    s = xml_strdup(text);
    if (!s) return 0;

    /* Free the previous text content if it exists */
    if (xml->text) free(xml->text);

    /* Set the text content of the XML node */
    xml->text = s;

    return 1;
}

/**
 *  \brief get text of xml.
 *  \param[in] xml: xml handle
 *  \return text of xml
 */
const char* xml_get_text(xml_t xml)
{
    if (!xml) return NULL;
    return xml->text;
}

/**
 * \brief Print text content with special characters converted to XML entities.
 *
 * \param[in] text The text content to print.
 * \param[in] buf The buffer for output.
 * \return 1 if the text content is successfully printed, 0 otherwise.
 */
static int print_text(char *text, BUFFER* buf)
{
    int len = 0, ap = 0;
    char *s = text;

    /* Calculate the length of the text and the length of the escape character that needs to be appended */
    while (*s)
    {
        if (*s == '<') ap += 3;
        else if (*s == '>') ap += 3;
        else if (*s == '&') ap += 4;
        else if (*s == '\'') ap += 5;
        else if (*s == '\"') ap += 5;
        len++;
        s++;
    }

    /* Append buffer space */
    if (!buf_append(len + ap)) return 0;

    /* Print text into buffer */
    s = text;
    while (len--)
    {
        if (*s == '<') buf_puts("&lt;", 4);
        else if (*s == '>') buf_puts("&gt;", 4);
        else if (*s == '&') buf_puts("&amp;", 5);
        else if (*s == '\'') buf_puts("&apos;", 6);
        else if (*s == '\"') buf_puts("&quot;", 6);
        else buf_putc(*s);
        s++;
    }

    return 1;
}

/**
 * \brief Print attribute content with special characters converted to XML entities.
 *
 * \param[in] attrs The attribute content to print.
 * \param[in] buf The buffer for output.
 * \return 1 if the attribute content is successfully printed, 0 otherwise.
 */
static int print_attrs(ATTR* attrs, BUFFER* buf)
{
    int len;

    /* Traverse and print each attribute */
    while (attrs)
    {
        /* Get the length of attrubute name */
        len = strlen(attrs->name);

        if (!buf_append(len + 3)) return 0;
        buf_putc(' ');
        buf_puts(attrs->name, len); /* Print attrubute name */
        buf_putc('=');
        buf_putc('"');

        /* Print attrubute value */
        print_text(attrs->value, buf);

        if (!buf_append(1)) return 0;
        buf_putc('"');

        attrs = attrs->next;
    }
    return 1;
}

/**
 *  \brief convert xml to text, using a buffered strategy.
 *  \param[in] xml: xml handle
 *  \param[in] buf: The buffer for output.
 *  \param[in] depth: The depth at which the current node is located
 *  \param[in] format: Neat formatted printing
 *  \return 1 if the xml content is successfully printed, 0 otherwise.
 */
static int print_node(xml_t xml, BUFFER* buf, int depth, int format)
{
    int len;
    int i;
    if (!xml) return 0;

    /* Traverse and print each xml */
    while (xml)
    {
        /* print indent */
        if (format)
        {
            if (format == 1)
            {
                if (!buf_append(depth)) return 0;
                for (i = 0; i < depth; i++) buf_putc('\t');
            }
            else format = 1;
        }

        /* print name */
        len = strlen(xml->name);
        if (!buf_append(len + 1)) return 0;
        buf_putc('<');
        buf_puts(xml->name, len);

        /* print attributes */
        if (xml->attrs) print_attrs(xml->attrs, buf);

        /* tag header closing */
        if (!buf_append(1)) return 0;
        buf_putc('>');

        /* print text */
        if (xml->text)
        {
            print_text(xml->text, buf);
            if (format) format = 2; // After printing the text, it must follow the label
        }

        /* print child */
        if (xml->child) 
        {
            if (format == 1)
            {
                if (!buf_append(1)) return 0;
                buf_putc('\n');
            }
            print_node(xml->child, buf, depth + 1, format);
        }

        /* print end label */
        if (format)
        {
            if (format == 1)
            {
                if (!buf_append(depth)) return 0;
                for (i = 0; i < depth; i++) buf_putc('\t');
            }
            else format = 1;
        }
        if (!buf_append(len + (format?4:3))) return 0;
        buf_putc('<');
        buf_putc('/');
        buf_puts(xml->name, len);
        buf_putc('>');
        if (format) buf_putc('\n');

        xml = xml->next;
    }

    return 1;
}

/**
 *  \brief convert xml to text, using a buffered strategy.
 *  \param[in] xml: xml handle
 *  \param[in] preset: preset is a guess at the final size, guessing well reduces reallocation
 *  \param[in] unformat: unformat=0 gives formatted, otherwise gives unformatted
 *  \param[out] len: address that receives the length of printed characters
 *  \return address of converted text, free the char* when finished
 */
char* xml_dumps(xml_t xml, int preset, int unformat, int* len)
{
    BUFFER p = {NULL, 0, 0};
    BUFFER *buf = &p;

    /* Initialize buf */
    if (preset < 1) preset = 1;
    if (!buf_append(preset)) return NULL;

    /* Print root node */
    if (!print_node(xml, buf, 0, unformat?0:1) || !buf_append(1))
    {
        free(buf->address);
        return NULL;
    }

    /* Add string terminator */
    buf_end() = '\0';

    /* Output string length */
    if (len) *len = buf->end;

    return buf->address;
}

/**
 *  \brief according to the xml object, generate a file.
 *  \param[in] xml: xml handle
 *  \param[in] filename: file name
 *  \return file len or negative fail
 */
int xml_file_dump(xml_t xml, char* filename)
{
    FILE* f;
    char* out;
    int len;

    if (!xml) return -1;

    out = xml_dumps(xml, 0, 0, &len);
    if (!out) return -1;

    f = fopen(filename, "w");
    if (!f)
    {
        free(out);
        return -1;
    }

    fwrite(out, 1, len, f);
    fclose(f);

    free(out);

    return len;
}

/**
 *  \brief skip XML comments.
 *  \param[in] text: The text content to skip.
 *  \return The character address after skip
 */
static const char* skip_comment(const char *text)
{
    if (*text == '<')
    {
        /* Matched to the front of the comment label */
        if (!strncmp(text, "<!--", 4))
        {
            /* skip "<!--" */
            text += 4;

            while (*text)
            {
                /* Because the tail is "-->", skip characters that are not '-' first */
                while (*text && *text != '-')
                {
                    /* Record line breaks */
                    if (*text == '\n') 
                    {
                        eline++, lbegin = text;
                    }
                    text++;
                }

                /* It ends at the end of the string, and the annotation does not form a closed interval */
                if (!*text) 
                {
                    E(XML_E_COMMENT); 
                    return text;
                }

                /* Matched to the end of the annotation label, the annotation has formed a complete closed interval */
                if (!strncmp(text, "-->", 3)) break;

                text++;
            }

            /* skip "-->" */
            text += 3;
        }
    }
    return text;
} 

/**
 *  \brief Parsing name.
 *  \param[in] text: The text content to parse.
 *  \param[out] name: Output the name generated by parsing
 *  \return The character address after parse
 */
static const char* parse_name(const char *text, char **name)
{
    int len = 0, ret = 0;
    char *s;

    /* Check the validity of the name and obtain the length */
    ret = check_name(text, &len);
    if (ret == 0 || ret == 1) 
    {
        E(XML_E_ILLEGAL); 
        return text + len;
    }

    /* Allocate space for name */
    s = (char*)malloc(len + 1);
    if (!s) 
    {
        E(XML_E_MEMORY); 
        return text + len;
    }

    /* Assign a value to name and output it */
    memcpy(s, text, len);
    s[len] = 0;
    *name = s;

    return text + len;
}

/**
 *  \brief Parsing one attribute.
 *  \param[in] text: The text content to parse.
 *  \param[out] attr: Output the attributes generated by parsing
 *  \return The character address after parse
 */
static const char* parse_one_attribute(const char *text, ATTR* attr)
{
    char* s;
    int len = 0, ret = 0;
    char *name, *value;
    char q = 0; /* quote: single or double */

    text = skip(text);
    if (*text == '>') return text; /* null attributes */

    /* Check the validity of the name and obtain the length */
    ret = check_name(text, &len);
    /* Encountered illegal characters */
    if (ret == 0)
    {
        if (text[len] != '=') 
        {
            E(XML_E_ILLEGAL); 
            return text + len;
        }
    }
    /* Parsed to the end of the text */
    else if (ret == 1) 
    {
        E(XML_E_END); 
        return text + len;
    }

    /* Allocate space for name */
    name = (char*)malloc(len + 1);
    if (!name) 
    {
        E(XML_E_MEMORY); 
        return text + len;
    }

    /* Assign a value to name */
    strncpy(name, text, len);
    name[len] = 0;

    /* Recorde name */
    attr->name = name;

    /* Skip */
    text += len;
    text = skip(text);

    /* No separator for name-value encountered */
    if (*text != '=') 
    {
        E(XML_E_VALUE); 
        return text;
    }

    /* Record whether the attribute value is currently included in `'` or in `"` */
    text = skip(++text);
    if (*text == '\'') q = '\'';
    else if (*text == '\"') q = '\"';
    else 
    {
        E(XML_E_QUOTE); 
        return text;
    }

    /* Initialize the value used to parse the value variable */
    text++;
    s = (char *)text;
    len = 0;

    /* How much space is needed for pre exploration and how many escape characters need to be parsed */
    while (*s != q)
    {
        if (*s == '&')
        {
            if      (!strncmp(s, "&lt;", 4))   s += 3;
            else if (!strncmp(s, "&gt;", 4))   s += 3;
            else if (!strncmp(s, "&amp;", 5))  s += 4;
            else if (!strncmp(s, "&apos;", 6)) s += 5;
            else if (!strncmp(s, "&quot;", 6)) s += 5;
        }
        len++, s++;
    }

    /* Allocate space to store parsed content */
    value = (char*)malloc(len + 1);
    if (!value) 
    {
        E(XML_E_MEMORY); 
        return text;
    }

    /* Store character by character */
    s = value;
    while (*text != q)
    {
        /* Record line breaks */
        if (*text == '\n') 
        {
            eline++, lbegin = text;
        }

        /* Record escape characters */
        if (*text == '&')
        {
            if      (!strncmp(text, "&lt;", 4))   {text += 4; *s++ = '<'; continue;}
            else if (!strncmp(text, "&gt;", 4))   {text += 4; *s++ = '>'; continue;}
            else if (!strncmp(text, "&amp;", 5))  {text += 5; *s++ = '&'; continue;}
            else if (!strncmp(text, "&apos;", 6)) {text += 6; *s++ = '\''; continue;}
            else if (!strncmp(text, "&quot;", 6)) {text += 6; *s++ = '\"'; continue;}
        }

        /* Normal character */
        *s++ = *text++;
    }

    /* Add the end of the string and record it */
    value[len] = 0;
    attr->value = value;

    return text + 1; /* skip quote */
}

/**
 *  \brief Parse all attributes of the entire label.
 *  \param[in] text: The text content to parse.
 *  \param[out] out: Output the attributes generated by parsing
 *  \return The character address after parse
 */
static const char* parse_attributes(const char *text, ATTR **out)
{
    ATTR *first = NULL, *prev = NULL, *attr = NULL;

    *out = NULL; /* Reset output */

    /* Create new attributes to record the results of parsing */
    attr = new_attr();
    if (!attr) 
    {
        E(XML_E_MEMORY); 
        return text;
    }

    text = parse_one_attribute(text, attr);
    /* parse error, or no attribute, return directly */
    if (etype != XML_E_OK || !attr->name) 
    {
        attribute_delete(attr); 
        return text;
    } 
    
    /* Continue parsing other attributes */
    first = attr, prev = first, *out = first;
    while (*text)
    {
        /* Create new attributes to record the results of parsing */
        attr = new_attr();
        if (!attr) 
        {
            E(XML_E_MEMORY); 
            return text;
        }

        /* Parse other attributes */
        text = parse_one_attribute(text, attr);
        /* parse error */
        if (etype != XML_E_OK) 
        {
            attribute_delete(attr); 
            return text;
        }
        /* end attribute, all attributes have been parsed */
        if (!attr->name) 
        {
            attribute_delete(attr); 
            break;
        } 

        /* Link */
        prev->next = attr;
    }

    return text;
}

/**
 *  \brief Parse text.
 *  \param[in] text: The text content to parse.
 *  \param[out] out: Output the attributes generated by parsing
 *  \return The character address after parse
 */
static const char* parse_text(const char *text, char **out)
{
    char *s, *t;
    int len = 0;

    s = (char *)skip(text);
    /* Not an escape, but a separate '<' character is not allowed */
    if (*s == '<' && s[1] != '!') 
    {
        *out = NULL; 
        return s;
    }

    /* Predict the length of text to allocate space */
    while (*s)
    {
        /* Text contains special escape */
        if (*s == '<')
        {
            /* Skip comments */
            if (!strncmp(s, "<!--", 4))
            {
                s += 4;
                while (*s)
                {
                    while (*s && *s != '-') s++;
                    if (!*s) {E(XML_E_COMMENT); return s;}
                    if (!strncmp(s, "-->", 3)) break;
                    s++;
                }
                s += 3;
                continue;
            }
            /* Get CDATA */
            else if (!strncmp(s, "<![CDATA[", 9))
            {
                s += 9;
                while (*s)
                {
                    while (*s && *s != ']') s++, len++;
                    if (!*s) {E(XML_E_COMMENT); return s;}
                    if (!strncmp(s, "]]>", 3)) break;
                    s++, len++;
                }
                s += 3;
                continue;
            }
            else break;
        }

        /* Normal escape characters */
        if (*s == '&')
        {
            if      (!strncmp(s, "&lt;", 4))   s += 3;
            else if (!strncmp(s, "&gt;", 4))   s += 3;
            else if (!strncmp(s, "&amp;", 5))  s += 4;
            else if (!strncmp(s, "&apos;", 6)) s += 5;
            else if (!strncmp(s, "&quot;", 6)) s += 5;
        }
        s++, len++;
    }

    /* Allocate space */
    t = (char *)malloc(len + 1);
    if (!t) 
    {
        E(XML_E_MEMORY); 
        return text;
    }

    /* Similar work when repeatedly predicting length to store characters in allocated space */
    *out = t;
    while (text < s)
    {
        /* Skip comments */
        text = skip_comment(text);
        if (etype != XML_E_OK) return text;

        if (*text == '<')
        {
            if (!strncmp(text, "<![CDATA[", 9)) /* get CDATA */
            {
                text += 9;
                while (*text)
                {
                    while (*text && *text != ']')
                    {
                        if (*text == '\n') {eline++, lbegin = text;}
                        *t++ = *text++;
                    }
                    if (!*text) {E(XML_E_CDATA); return text;}
                    if (!strncmp(text, "]]>", 3)) break;
                    *t++ = *text++;
                }
                text += 3;
                continue;
            }
        }
        if (*text == '\n') {eline++, lbegin = text;}
        if (*text == '&')
        {
            if      (!strncmp(text, "&lt;", 4))   {text += 4; *t++ = '<'; continue;}
            else if (!strncmp(text, "&gt;", 4))   {text += 4; *t++ = '>'; continue;}
            else if (!strncmp(text, "&amp;", 5))  {text += 5; *t++ = '&'; continue;}
            else if (!strncmp(text, "&apos;", 6)) {text += 6; *t++ = '\''; continue;}
            else if (!strncmp(text, "&quot;", 6)) {text += 6; *t++ = '\"'; continue;}
        }
        *t++ = *text++;
    }

    /* Add string terminator */
    *t = 0;

    return text;
}

/**
 *  \brief Parse xml node.
 *  \param[in] text: The text content to parse.
 *  \param[out] node: Output the node information
 *  \return The character address after parse
 */
static const char* parse_node(const char* text, xml_t node)
{
    xml_t tail = NULL, child;
    ATTR* attrs = NULL;
    int len = 0;
    char *out = NULL;
    const char *s = NULL;
    
    /* Get '<' */
    while (*text)
    {
        /* Exploring Character `<` */
        text = skip(text);
        if (*text != '<') 
        {
            E(XML_E_LABEL); 
            return text;
        }

        /* Check if this is a comment, if it is, skip it */
        s = skip_comment(text);
        if (etype != XML_E_OK) return text;

        /* Indicates that the middle comment has been skipped */
        if (s > text) 
        {
            text = s; 
            continue;
        }
        /* Normal `<` */
        else 
        {
            text = s; 
            break;
        }
    }
    /* Skip `<` */
    text++;

    /* parse name */
    s = parse_name(text, &node->name);
    if (etype != XML_E_OK) return s;
    len = s - text;
    text = s;

    /* parsing the header tag is not over yet, and then parsing the attribute */
    if (*text != '>')
    {
        text = parse_attributes(text, &attrs);
        node->attrs = attrs;
        if (etype != XML_E_OK) return text;
    }
 
    /* parse text */
    text = parse_text(text + 1, &out);
    node->text = out;

    /* parse children and end tag */
    while (*text)
    {
        /* Exploring Character `<` */
        text = skip(text);
        if (*text != '<') 
        {
            E(XML_E_ILLEGAL); 
            return text;
        }

        /* label tail */
        if (text[1] == '/')
        {
            /* Skip `</` */
            text += 2;
            if (memcmp(node->name, text, len)) 
            {
                E(XML_E_LABEL); 
                return text;
            }

            /* Skip name */
            text += len;

            /* Determine if a</table>end tag has been formed */
            if (*text != '>') 
            {
                E(XML_E_LABEL); 
                return text;
            }

            /* Reset `len` */
            len = 0;

            /* Skip `>` */
            text++;

            return text;
        }
        /* parsing child elements */
        else 
        {
            /* Check if this is a comment, if it is, skip it */
            s = skip_comment(text);
            if (etype != XML_E_OK) return text;

            /* Indicates that the middle comment has been skipped */
            if (s > text) 
            {
                text = s; 
                continue;
            }
            /* Normal `<` */
            else text = s;

            /* Create a new node to store new parsing content */
            child = new_node();
            if (!child) 
            {
                E(XML_E_MEMORY); 
                return text;
            }

            /* Add to child node linked list */
            if (!node->child) node->child = child;
            else tail->next = child;
            tail = child;

            /* Parse child node */
            text = parse_node(text, child);
            if (etype != XML_E_OK) return text;

            /* Skip the extra characters */
            while (*text && *text != '<')
            {
                if (*text == '\n') 
                {
                    eline++, lbegin = text;
                }
                text++;
            }
        }
    }

    /* No closing tag */
    if (len > 0) E(XML_E_LABEL);

    return text;
}

/**
 *  \brief Parse xml file head.
 *  \param[in] text: The text content to parse.
 *  \return The character address after parse
 */
static const char* parse_head(const char* text)
{
    char *name = NULL, *version = NULL, *encoding = NULL;
    ATTR *attr_version = NULL, *attr_encoding = NULL;

    /* Skip `<?` */
    text += 2;

    /* Parse name */
    text = parse_name(text, &name);
    if (etype != XML_E_OK) goto END;

    /* Must comply with XML file header signature */
    if (strcmp(name, "xml") == 0)
    {
        /* Create new attributes to store version information */
        attr_version = new_attr();
        if (!attr_version) 
        {
            E(XML_E_MEMORY); 
            goto END;
        }

        /* Parse attributes to determine if it is the expected `version` */
        text = parse_one_attribute(text, attr_version);
        if (etype != XML_E_OK) goto END; /* parse error */
        if (strcmp(attr_version->name, "version")) 
        {
            E(XML_E_VERSION); 
            goto END;
        }

        /* Create new attributes to store encoding information */
        attr_encoding = new_attr();
        if (!attr_encoding) 
        {
            E(XML_E_MEMORY); 
            goto END;
        }

        /* Parse attributes to determine if it is the expected `encoding` */
        text = parse_one_attribute(text, attr_encoding);
        if (etype != XML_E_OK) goto END; /* parse error */
        if (strcmp(attr_encoding->name, "encoding")) 
        {
            E(XML_E_ENCODING); 
            goto END;
        }
    }

    /* Determine whether the label forms a complete closed interval */
    text = skip(text);
    if (strncmp(text, "?>", 2)) 
    {
        E(XML_E_NOTES); 
        goto END;
    }

    /* Skip `?>` */
    text += 2;

END:
    if (attr_version) attribute_delete(attr_version);
    if (attr_encoding) attribute_delete(attr_encoding);
    if (name) free(name);
    return text;
}

/**
 *  \brief load xml from text string.
 *  \param[in] text: text
 *  \return xml object
 */
xml_t xml_loads(const char* text)
{
    xml_t root;

    etype = XML_E_OK;
    eline = 1;
    lbegin = text;
    ecolumn = 0;

    /* parse head notes */
    if (strncmp(text, "<?", 2) == 0)
    {
        text = parse_head(text);
        if (etype != XML_E_OK) return NULL;
    }

    /* Create a root node to store parsing information */
    root = new_node();
    if (!root) 
    {
        E(XML_E_MEMORY); 
        return NULL;
    }

    /* Start parsing and store the parsing results in root */
    text = parse_node(text, root);
    if (etype != XML_E_OK)
    {
        xml_delete(root);
        return NULL;
    }

    return root;
}

/**
 *  \brief load xml from file.
 *  \param[in] filename: file name
 *  \return xml object
 */
xml_t xml_file_load(const char* filename)
{
    FILE* f;
    xml_t xml = NULL;
    long len;
    char* text;

    /* open file and get the length of file */
    f = fopen(filename, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* read file */
    text = (char*)malloc(len + 1);
    if (text)
    {
        fread(text, 1, len, f);
        fclose(f);
    }
    else
    {
        fclose(f);
        return NULL;
    }
    text[len] = 0;

    /* load text */
    xml = xml_loads(text);
    free(text);

    return xml;
}
