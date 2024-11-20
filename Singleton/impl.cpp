#include "impl.h"
#include "foo.h"
#include "bar.h"

namespace{const auto touch = singleton::instance();}

void inject(void (*_dynamic)(std::size_t, void*))
{
    singleton::inject(_dynamic);
}
