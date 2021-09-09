#include "redis.h"
#include "stdlib.h"
#include "string.h"
#include "hiredis_cluster/hircluster.h"

#define redisClientImplPtr ((redisClientImpl *)p)

#ifndef _WIN32
#define size_t long unsigned int
#endif

class RedisReplyImpl : public RedisReply
{
public:
	RedisReplyImpl(redisReply *r)
	{
		err = 0;
		strcpy(errstr, "\0");
		this->r = r;
	};

	RedisReplyImpl(int err, char *errstr)
	{
		this->err = err;
		strcpy(this->errstr, errstr);
		this->r = (redisReply *)malloc(sizeof(redisReply));
	};

	~RedisReplyImpl()
	{
		if (this->err)
			free(this->r);
	}

	void Type(int *p)
	{
		p = &this->r->type;
	}

	void Integer(long long *p)
	{
		p = &this->r->integer;
	}

	void Double(double *p)
	{
		p = &this->r->dval;
	}

	void StrSize(size_t *p)
	{
		p = &this->r->len;
	}

	void Str(char *p)
	{
		p = this->r->str;
	}

	int GetType()
	{
		return this->r->type;
	}

	long long GetInteger()
	{
		return this->r->integer;
	}

	double GetDouble()
	{
		return this->r->dval;
	}

	size_t GetStrSize()
	{
		return this->r->len;
	}

	char *GetStr()
	{
		return this->r->str;
	}

	const char *GetTypeStr()
	{
		return SwitchTypeStr(this->r->type);
	}

protected:
	redisReply *r;
	const char *SwitchTypeStr(int TypeEnum)
	{
		switch (TypeEnum)
		{
		case R_TYPE_STRING:
		{
			return "STRING";
			break;
		}
		case R_TYPE_ARRAY:
		{
			return "ARRAY";
			break;
		}
		case R_TYPE_INTEGER:
		{
			return "INTEGER";
			break;
		}
		case R_TYPE_NIL:
		{
			return "NIL";
			break;
		}
		case R_TYPE_STATUS:
		{
			return "STATUS";
			break;
		}
		case R_TYPE_ERROR:
		{
			return "ERROR";
			break;
		}
		case R_TYPE_DOUBLE:
		{
			return "DOUBLE";
			break;
		}
		case R_TYPE_BOOL:
		{
			return "BOOL";
			break;
		}
		case R_TYPE_MAP:
		{
			return "MAP";
			break;
		}
		case R_TYPE_SET:
		{
			return "SET";
			break;
		}
		case R_TYPE_ATTR:
		{
			return "ATTR";
			break;
		}
		case R_TYPE_PUSH:
		{
			return "PUSH";
			break;
		}
		case R_TYPE_BIGNUM:
		{
			return "BIGNUM";
			break;
		}
		case R_TYPE_VERB:
		{
			return "VERB";
			break;
		}
		default:
			return "UNKNOWN";
		}
	}
};

struct redisClientImpl
{
	redisClusterContext *cc;
	redisReply *r;
	RedisReplyImpl *reply;
};

RedisClient::RedisClient(const char *addrs)
{
	err = 0;
	p = new redisClientImpl();
	memset(errstr, '\0', 128);
	redisClientImplPtr->cc = nullptr;
	redisClientImplPtr->r = nullptr;
	redisClientImplPtr->reply = new RedisReplyImpl(0, "\0");

	redisClientImplPtr->cc = redisClusterContextInit();
	redisClusterSetOptionAddNodes(redisClientImplPtr->cc, addrs);
	redisClusterConnect2(redisClientImplPtr->cc);
	if (redisClientImplPtr->cc == NULL || redisClientImplPtr->cc->err)
	{
		this->err = redisClientImplPtr->cc->err > 0 ? redisClientImplPtr->cc->err : R_TYPE_ERROR;
		strcpy(this->errstr, redisClientImplPtr->cc->errstr);
		return;
	}
}

RedisClient::~RedisClient()
{
	// delete reply;
	if (redisClientImplPtr->reply != nullptr)
		delete redisClientImplPtr->reply;
	delete redisClientImplPtr;
	redisClusterFree(redisClientImplPtr->cc);
}

RedisReply *RedisClient::command(const char *cmd)
{
	err = 0;
	redisClientImplPtr->r = (redisReply *)redisClusterCommand(redisClientImplPtr->cc, cmd);
	delete redisClientImplPtr->reply;

	if (redisClientImplPtr->r == NULL || redisClientImplPtr->cc->err)
	{
		err = redisClientImplPtr->cc->err > 0 ? redisClientImplPtr->cc->err : R_ERR_IO;
		strcpy(errstr, redisClientImplPtr->cc->errstr);
		redisClientImplPtr->reply = new RedisReplyImpl(err, errstr);
	}
	else
	{
		redisClientImplPtr->reply = new RedisReplyImpl(redisClientImplPtr->r);
	}

	return redisClientImplPtr->reply;
}

#ifndef _WIN32
#undef size_t
#endif