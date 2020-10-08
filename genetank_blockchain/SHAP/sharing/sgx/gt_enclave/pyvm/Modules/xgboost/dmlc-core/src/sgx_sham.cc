#include "sgx_sham.h"

#ifdef _LIBCPP_SGX_AUXILIARY

char *getenv(const char *name) {
   return (char*) NULL;
}

char *mkdtemp(char *templatename) {
   return (char*) NULL;
}

bool S_ISLNK(int st_mode) {
    return true;
}

int stat(const char *path, struct stat *buf) {
    return 0;
}

int fstat(int fd, struct stat *buf) {
    return 0;
}

int lstat(const char *path, struct stat *buf) {
    return 0;
}

int remove(const char *filename) {
    return 0;
}

FILE *fopen(const char *filename, const char *mode) {
    return (FILE*) NULL;
}

int fclose(FILE *stream) {
    return 0;
}

int fflush(FILE *stream) {
    return 0;
}

int rmdir(const char *path) {
    return 0;
}

int fseek(FILE *stream, long int offset, int whence) {
    return 0;
}

int fileno(FILE *stream) {
    return 0;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
     return 0;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return 0;
}

int fprintf(FILE *stream, const char *format, ...) {
    return 0;
}

int feof(FILE *stream) {
    return 0;
}

long int ftell(FILE *stream) {
    return 0;
}

DIR *opendir(const char *name) {
    return (DIR *) NULL;
}

int closedir(DIR *dir) {
    return 0;
}

dirent *readdir(DIR *dir) {
    return (dirent *) NULL;
}

namespace std {
}

double get_time(){
	return 0;
}
ostream& operator<<(ostream& os, const bool d) {
    return os;
}

ostream& operator<<(ostream& os, const char* c) {
    return os;
}

ostream& operator<<(ostream& os, const char c) {
    return os;
}

ostream& operator<<(ostream& os, const std::string& str){
    return os;
}

ostream& operator<<(ostream& os, const int i) {
    return os;
}


istream& operator>>(istream& is, bool& d) {
    return is;
}

istream& operator>>(istream& is, char* c) {
    return is;
}

istream& operator>>(istream& is, int& i) {
    return is;
}

istream& operator>>(istream& is, std::string str) {
    return is;
}

istream& operator>>(istream& is, char& c) {
    return is;
}

int sscanf(const char *str, const char *format, ...){
	return 0;
}
#endif
