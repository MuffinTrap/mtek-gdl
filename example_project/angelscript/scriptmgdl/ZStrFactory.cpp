#include "ZStrFactory.h"
#include <stdlib.h>

bool ZstrEqual::operator()(zstr const& keyA, zstr const& keyB) const
{
	return zstr_eq(&keyA, &keyB);
}

size_t ZstrHash::operator()(const zstr& keyA) const
{
	size_t length = zstr_len(&keyA);
	const char* data =zstr_cstr(&keyA);
	size_t hash = 0;
	for(size_t i = 0; i < length; i++) {
		hash += data[i] % 7;
	}
	return hash;
}


const void * ZStrFactory::GetStringConstant(const char* data, asUINT length)
{
	zstr str = zstr_from_len(data, length);
	map_t::iterator it = stringCache.find(str);
	if (it != stringCache.end())
			it->second++;
		else
			it = stringCache.insert(map_t::value_type(str, 1)).first;

		return reinterpret_cast<const void*>(&it->first);
}

int ZStrFactory::GetRawStringData(const void* str, char* data, asUINT* length) const
{
	if (str == nullptr)
	{
			return asERROR;
	}
	zstr* key = (zstr*)(str);

	if (length != nullptr)
	{
		*length = (asUINT)zstr_len(key);
	}

	if (data != nullptr)
	{
		size_t length = zstr_len(key);
		memcpy(data, zstr_data(key), length);
	}

	return asSUCCESS;
}


int ZStrFactory::ReleaseStringConstant(const void* str)
{
	int ret = asSUCCESS;
	map_t::iterator it = stringCache.find(*reinterpret_cast<const zstr*>(str));
	if (it == stringCache.end())
		ret = asERROR;
	else
	{
		it->second--;
		if (it->second == 0)
			stringCache.erase(it);
	}


	return ret;
}

