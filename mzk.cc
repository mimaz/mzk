#include "object.h"

namespace mzk
{
	object::~object()
	{
		for (base_pointer *ptr : _pointer_set)
			ptr->on_mzk_object_delete();
	}
}
