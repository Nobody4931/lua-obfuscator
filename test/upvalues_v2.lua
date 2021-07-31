local function do_upvals()
	local upval_1 = 6
	local upval_2 = 9

	function print_upvals()
		print(upval_1, upval_2)
	end

	print_upvals()

	upval_1 = 42
	upval_2 = 0

	print_upvals()
end

do_upvals()
print_upvals()
