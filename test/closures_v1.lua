local function local_closure(a, b, c)
	print("a: ", a)
	print("b: ", b)
	print("c: ", c)
end

function global_closure(x, y, z)
	print("x: ", x)
	print("y: ", y)
	print("z: ", z)
end

local function local_vararg_closure(...)
	print("arg table?: ", arg)

	local args = {...}
	for i = 1, #args do
		print("args[" .. i .. "] = ", args[i])
	end
end

function global_vararg_closure(...)
	print("arg table?: ", arg)

	local args = arg
	for i = 1, #args do
		print("args[" .. i .. "] = ", args[i])
	end
end


local_closure(500, "some string", {})
global_closure(newproxy(), 420.69, false)

local_vararg_closure(true, nil, { "weee" }, "hello", 42)
global_vararg_closure(function() end, 112, nil, nil, setfenv)

-- TEST RESULTS
--> "arg" is not available in getfenv()
--> and im not supporting it so fuck off
