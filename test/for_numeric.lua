local first = 0
local last = 10
local step = 0.2

print("positive step")
for iterator = first, last, step do
	print(iterator)
end

first = 5
last = -5
step = -0.25

print("negative step")
for iterator = first, last, step do
	print(iterator)
end

-- TEST RESULTS
--> OK
