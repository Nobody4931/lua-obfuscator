local function fizzbuzz(count)
	for i = 1, count do
		local result = ""
		if (i % 3 == 0) then
			result = result .. "Fizz"
		end
		if (i % 5 == 0) then
			result = result .. "Buzz"
		end
		print(result ~= "" and result or i)
	end
end

fizzbuzz(100)

-- TEST RESULTS:
--> OK
