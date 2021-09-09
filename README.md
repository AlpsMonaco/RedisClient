# RedisClient
A c++ RedisClient based on hiredis_cluster,used for redis cluster .



## connect

```c++
RedisClient rc("127.0.0.1:8000,127.0.0.1:8001");
if (rc.err)
{
	//handle error
	return 1;
}
```



## command

```c++
RedisReply* r = rc.command("set foo bar");
// whenever there's an error,both RedisReply* r and RedisClient rc's err field is larget // than zero and they are the same.

if(rc.err){
    // handle err
   std::cout << rc.errstr << std::endl;
}else{
	println("type is:");
	println(r->GetTypeStr());
	if (r->GetStrSize() > 0)
		println(r->GetStr());
}
```

