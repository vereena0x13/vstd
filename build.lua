#!/usr/bin/env lua

local function panic(s)
	print(s)
	os.exit(1)
end

local fout = io.open("vstd.hpp", "w")

local function include(fn)
	local fh = io.open("src/" .. fn, "r")
	if not fh then panic("File not found: src/" .. fn) end
	local s = fh:read("*a")
	fh:close()
	fout:write("// begin file: " .. fn .. "\n")
	fout:write(s)
	fout:write("\n// end file " .. fn .. "\n\n")
end


include "header_top.hpp"
	include "math/math.hpp"
	include "mem/allocator.hpp"
	include "mem/temporary_storage.hpp"
	include "mem/arena.hpp"
	include "hash.hpp"
	include "string.hpp"
	include "general.hpp"
	include "io/file.hpp"
	include "io/data_input.hpp"
	include "io/data_output.hpp"
	include "io/bytebuf.hpp"
	include "ds/static_array.hpp"
	include "ds/array.hpp"
	include "ds/slot_allocator.hpp"
	include "ds/hash_table.hpp"
include "header_bot.hpp"

include "impl_top.cpp"
	include "general.cpp"
	include "mem/allocator.cpp"
	include "mem/temporary_storage.cpp"
	include "hash.cpp"
	include "string.cpp"
	include "io/file.cpp"
include "impl_bot.cpp"


fout:close()