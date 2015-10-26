#ifndef DBG_H
#define DBG_H

#include <iostream>
#include <string>

# define PRINT_VECTOR(V)\
	std::cout << "[";\
	for (auto _it = V.begin(); _it != V.end(); _it++) {\
		std::cerr << "'" << *_it << "', ";\
	}\
	std::cout << "]" << std::endl;


#include <stdio.h>
#include <errno.h>
#include <string.h>


#ifdef DEBUG_FLAG
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%4d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define debug(M, ...)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }


#endif
