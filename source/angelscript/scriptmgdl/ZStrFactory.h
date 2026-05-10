#pragma once
#include <angelscript.h>
#include <mgdl/mgdl-types.h>

/**
 * @file ZStrFactory.h
 * @ingroup angelscript
 * @brief String factory needed by AngelScript
 */

/**
 * @brief Function object providing equal check
 */
class ZstrEqual
{
public:
	bool operator()(zstr const& keyA, zstr const& keyB) const;
};

/**
 * @brief Function object providing string hash
 */
class ZstrHash
{
public:
	size_t operator()(const zstr& keyA) const;
};

// Copied this part from scriptstdstring
#include <unordered_map>
typedef std::unordered_map<zstr, int, ZstrHash, ZstrEqual> map_t;

/**
 * @brief String factory for zstr, needed by AngelScript
 */
class ZStrFactory : public asIStringFactory
{
public:
	const void* GetStringConstant(const char * data, asUINT length) override;
	int GetRawStringData(const void * str, char * data, asUINT * length) const override;
	int ReleaseStringConstant(const void * str) override;
private:
	map_t stringCache;
};
