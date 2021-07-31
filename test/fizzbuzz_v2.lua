local function fizzbuzz(count, names)
	for i = 1, count do
		local result = ""

		for _, info in ipairs(names) do
			local divisor = info[1]
			local name = info[2]

			if (i % divisor == 0) then
				result = result .. name
			end
		end

		print(result ~= "" and result or i)
	end
end

fizzbuzz(105, {
	{ 3, "Fizz" },
	{ 5, "Buzz" },
	{ 7, "Bazz" },
})
