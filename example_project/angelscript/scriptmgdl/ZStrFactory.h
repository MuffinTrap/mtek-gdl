#pragma once
#include "../angelscript.h"
#include <mgdl/zstrh/zstr.h>

class ZstrEqual
{
public:
	bool operator()(zstr const& keyA, zstr const& keyB) const;
};

class ZstrHash
{
public:
	size_t operator()(const zstr& keyA) const;
};

// Copied this part from scriptstdstring
#include <unordered_map>
typedef std::unordered_map<zstr, int, ZstrHash, ZstrEqual> map_t;

// String interface to zstr.h
class ZStrFactory : public asIStringFactory
{
public:
	const void* GetStringConstant(const char * data, asUINT length) override;
	int GetRawStringData(const void * str, char * data, asUINT * length) const override;
	int ReleaseStringConstant(const void * str) override;
private:
	map_t stringCache;
};
