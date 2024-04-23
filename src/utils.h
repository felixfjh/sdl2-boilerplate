#ifndef UTILS_H
#define UTILS_H

#define ASSERT(success, ...) 			\
	if (!success)			 			\
	{ 						 			\
		fprintf(stderr, __VA_ARGS__);	\
		exit(1);						\
	}

#define LOG_INFO(...)				\
	 fprintf(stdout, "[INFO]: ");	\
	 fprintf(stdout, __VA_ARGS__);	\

#define LOG_ERROR(...)				\
	 fprintf(stderr, "[ERROR]: ");	\
	 fprintf(stderr, __VA_ARGS__);	\

#endif /* UTILS_H */
