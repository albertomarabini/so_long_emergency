valgrind -s --suppressions=suppressions.supp --leak-check=full --show-leak-kinds=all --log-file=vg.log ../debugger/a.out ./maps/piscine_bonus.ber
#when you uncomment this make sure to not delete your suppressions.supp file
#valgrind -s --suppressions=suppressions.supp --leak-check=full --show-leak-kinds=all --gen-suppressions=all ../debugger/a.out ./maps/piscine_bonus.ber 2> suppressions1.supp
