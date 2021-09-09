#pragma once

#define extern_pointer void *
#pragma comment(lib,"WS2_32.Lib")

enum RedisReplyType
{
	R_TYPE_STRING = 1,
	R_TYPE_ARRAY,
	R_TYPE_INTEGER,
	R_TYPE_NIL,
	R_TYPE_STATUS,
	R_TYPE_ERROR,
	R_TYPE_DOUBLE,
	R_TYPE_BOOL,
	R_TYPE_MAP,
	R_TYPE_SET,
	R_TYPE_ATTR,
	R_TYPE_PUSH,
	R_TYPE_BIGNUM,
	R_TYPE_VERB,
};

enum RedisError
{
	R_ERR_IO = 1,
	R_ERR_OTHER,
	R_ERR_EOF,
	R_ERR_PROTOCOL,
	R_ERR_OOM,
	R_ERR_TIMEOUT,
};

#ifndef _WIN32
#define size_t long unsigned int
#endif

class RedisReply
{
public:
	int err;
	char errstr[128];
	virtual void Type(int *p) = 0;
	virtual void Integer(long long *p) = 0;
	virtual void Double(double *p) = 0;
	virtual void StrSize(size_t *p) = 0;
	virtual void Str(char *p) = 0;
	virtual int GetType() = 0;
	virtual long long GetInteger() = 0;
	virtual double GetDouble() = 0;
	virtual size_t GetStrSize() = 0;
	virtual char *GetStr() = 0;
	virtual const char *GetTypeStr() = 0;
};

// hiredis is not thread safe
class __declspec(dllexport) RedisClient
{
public:
	// usage "127.0.0.1:8000,127.0.0.1:8001"
	RedisClient(const char *addrs);
	~RedisClient();

	RedisReply *command(const char *cmd);

	int err;
	char errstr[128];

protected:
	extern_pointer p;
};

#ifndef _WIN32
#undef size_t
#endif