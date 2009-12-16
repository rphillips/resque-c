/*
Copyright (c) 2009 Ryan Phillips <ryan@trolocsis.com>
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef RESQUE_ERROR_H_
#define RESQUE_ERROR_H_

typedef struct {
    unsigned int line;
    const char *file;
    char *msg;
} resque_error_t;

#define RESQUE_SUCCESS (NULL)

#define resque_error_create(id, err, msg) resque_error_create_impl(err,      \
                                                       msg,      \
                                                       __LINE__, \
                                                       __FILE__)

#define resque_error_createf(id, err, fmt, ...) resque_error_createf_impl(err,      \
                                                              __LINE__, \
                                                              __FILE__, \
                                                              fmt,      \
                                                              __VA_ARGS__)

resque_error_t*
resque_error_create_impl(const char *msg,
                         unsigned int line,
                         const char *file);

resque_error_t*
resque_error_createf_impl(unsigned int line,
                          const char *file, 
                          const char *fmt,
                          ...);

void
resque_error_destroy(resque_error_t *err);

#endif
