#ifndef VITAMTP_PLATFORM_H
#define VITAMTP_PLATFORM_H

#ifdef _WIN32
#include <winsock2.h>
#include <basetsd.h>
#include <windows.h>
#ifdef _MSC_VER
typedef SSIZE_T ssize_t;
#endif
typedef SOCKET vitamtp_socket_t;
typedef CRITICAL_SECTION vitamtp_mutex_t;
#define vitamtp_mutex_init(m) (InitializeCriticalSection((m)), 0)
#define vitamtp_mutex_destroy(m) DeleteCriticalSection((m))
#define vitamtp_mutex_lock(m) (EnterCriticalSection((m)), 0)
#define vitamtp_mutex_unlock(m) (LeaveCriticalSection((m)), 0)
#ifndef strdup
#define strdup _strdup
#endif
#else
#include <pthread.h>
typedef int vitamtp_socket_t;
typedef pthread_mutex_t vitamtp_mutex_t;
#define vitamtp_mutex_init(m) pthread_mutex_init((m), NULL)
#define vitamtp_mutex_destroy(m) pthread_mutex_destroy((m))
#define vitamtp_mutex_lock(m) pthread_mutex_lock((m))
#define vitamtp_mutex_unlock(m) pthread_mutex_unlock((m))
#endif

#endif
