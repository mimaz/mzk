#ifndef __MZK_SIGNAL_H
#define __MZK_SIGNAL_H

#include <vector>

#ifndef __MZK_NO_IMPL
# define __MZK_NO_IMPL
# include "object.h"
# undef __MZK_NO_IMPL
#else
# include "object.h"
#endif

namespace mzk
{
	class base_connection : public object
	{
	 public:
		 virtual void disconnect() = 0;
	};

	using slot = ptr<base_connection>;



	template<typename ...arg_types>
		class connection;



	template<typename ...arg_types>
		class signal
		{
		 public:
			 using connection = mzk::connection<arg_types...>;

			 signal();
			 signal(const signal &other) = delete;
			 signal(signal &&other) = delete;

			 ~signal();

			 template<typename ...bind_arg_types>
				 slot bind(const bind_arg_types &...args);

			 void unbind(connection *conn);

			 signal &operator()(const arg_types &...args);

		 private:
			 std::vector<slot> _slot_set;
		};
}

#ifndef __MZK_NO_IMPL
# include "bits/signal.inl"
# include "bits/object.inl"
#endif

#endif
