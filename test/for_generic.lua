local data_array = { 1, 2, 3, 4, 5, nil, 7, 8, 9, 10 }

local data_dict = {
	["key_1"] = "value_1";
	["key_2"] = "value_2";
	["key_3"] = "value_3";
}


print("ipairs (array)")
for index, value in ipairs(data_array) do
	print(index, value)
end

print("pairs (array)")
for index, value in pairs(data_array) do
	print(index, value)
end

print("next (array)")
for index, value in next, data_array do
	print(index, value)
end


print("ipairs (dict)")
for index, value in ipairs(data_dict) do
	print(index, value)
end

print("pairs (dict)")
for index, value in pairs(data_dict) do
	print(index, value)
end

print("next (dict)")
for index, value in next, data_dict do
	print(index, value)
end

-- TEST RESULTS
--> OK
