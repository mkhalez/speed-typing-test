#include "function.h"
#include <string.h>
#include <cerrno>

void* Function::memcpy(void* s1, const void* s2, size_t n) {
    char* dest = static_cast<char*>(s1);
    const char* src = static_cast<const char*>(s2);


    for (size_t i = 0; i < n; ++i) {
        dest[i] = src[i];
    }
    return s1;
}

void* Function::memmove(void* s1, const void* s2, size_t n) {
    char* dest = static_cast<char*>(s1);
    const char* src = static_cast<const char*>(s2);
    if (dest < src) {
        for (size_t i = 0; i < n; ++i) {
            dest[i] = src[i];
        }
    } else {
        for (size_t i = n; i > 0; --i) {
            dest[i - 1] = src[i - 1];
        }
    }
    return s1;
}

char* Function::strcpy(char* s1, const char* s2) {
    char* start = s1;
    while ((*s1++ = *s2++) != '\0') {}
    return start;
}

char* Function::strncpy(char* s1, const char* s2, size_t n) {
    char* start = s1;
    size_t i = 0;


    for (; i < n && s2[i] != '\0'; ++i) {
        s1[i] = s2[i];
    }


    for (; i < n; ++i) {
        s1[i] = '\0';
    }
    return start;
}

char* Function::strcat(char* s1, const char* s2) {
    char* start = s1;
    while (*s1)
        s1++;
    while ((*s1++ = *s2++) != '\0') {}
    return start;
}

char* Function::strncat(char* s1, const char* s2, size_t n) {
    char* start = s1;
    while (*s1)
        s1++;
    size_t i = 0;
    while (i < n && (*s1++ = *s2++) != '\0') {
        i++;
    }
    *s1 = '\0';
    return start;
}

int Function::memcmp(const void* s1, const void* s2, size_t n) {
    const unsigned char* p1 = static_cast<const unsigned char*>(s1);
    const unsigned char* p2 = static_cast<const unsigned char*>(s2);


    for (size_t i = 0; i < n; ++i) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

int Function::strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int Function::strcoll(const char* s1, const char* s2) {
    return strcmp(s1, s2);
}

int Function::strncmp(const char* s1, const char* s2, size_t n) {
    if (n == 0)
        return 0;
    while (--n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

size_t Function::strxfrm(char* s1, const char* s2, size_t n) {
    size_t len = strlen(s2);
    if (n > len) {
        strcpy(s1, s2);
    }
    return len;
}

static char* saved_token = nullptr;

char* strtok(char* s1, const char* s2) {
    if (s1 == nullptr) {
        if (saved_token == nullptr) {
            return nullptr;
        }
        s1 = saved_token;
    }

    s1 += strspn(s1, s2);
    if (*s1 == '\0') {
        saved_token = nullptr;
        return NULL;
    }

    char* end = s1 + strcspn(s1, s2);
    if (*end == '\0') {
        saved_token = nullptr;
    } else {
        *end = '\0';
        saved_token = end + 1;
    }

    return s1;
}

void* Function::memset(void* s, int c, size_t n) {
    unsigned char* p = static_cast<unsigned char*>(s);


    for (size_t i = 0; i < n; ++i) {
        p[i] = static_cast<unsigned char>(c);
    }
    return s;
}

size_t Function::strlen(const char* s) {
    const char* p = s;
    while (*p)
        p++;
    return p - s;
}

char* Function::strerror(int errnum) {
    static char buf[256];
    const char* msg = "Unknown error";
    switch (errnum) {
        case 0:
            msg = "Success";
            break;
        case EINVAL:
            msg = "Invalid argument";
            break;
        case ENOMEM:
            msg = "Out of memory";
            break;
    }

    strncpy(buf, msg, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    return buf;
}
