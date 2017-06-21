#ifndef RETRIEVE_HPP
#define RETRIEVE_HPP

#define RETRIEVE(from, name, type) \
	if(from.has_field(#name) and from[#name].is_##type()) \
		name = from[#name].as_##type();

#define RETRIEVE_BOOL(from, name) \
	if(from.has_field(#name) and from[#name].is_boolean()) \
		name = from[#name].as_bool();

#endif
